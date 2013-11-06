#include <stdio.h>
#include <stdlib.h>

#include <velib/base/base.h>
#include <velib/base/ve_string.h>
#include <velib/types/ve_item.h>
#include <velib/types/variant_print.h>
#include <velib/types/ve_dbus_item.h>
#include <velib/utils/ve_logger.h>
#include <velib/platform/console.h>
#include <velib/platform/plt.h>

#include "values.h"
#include "serial_hal.h"
#include "gps.h"
#include "dev_reg.h"

#define MODULE			"VALUES"
#define SERVICE_NAME	"com.victronenergy.gps"

/** dbus connection */
struct VeDbus *dbus;

/** The root of the tree to locate values and format them etc */
static VeItem root;

static VeItem processName;
static VeItem processVersion;
static VeItem connection;

static un32 timeout;

static VeVariantUnitFmt none = {0, ""};

static char const *interface(void)
{
	VeVariant variant;

	serialHalInstance(&variant);
	veVariantToN32(&variant);

	if (variant.value.UN32 < 256)
		return "CAN";

	/* Serial ports */
	if (variant.value.UN32 < (256 + 32)) {
#if defined(TARGET_bpp3)
		if (variant.value.UN32 == 256)
			return "VE.Direct port 1";
		if (variant.value.UN32 == 258)
			return "VE.Direct port 2";
#endif
		return "VE.Direct";
	}

	return "USB";
}

void valuesInit(void)
{
	timeout = devReg.timeOut * 20;

	gpsInit(&root);

	/* App info */
	veItemAddChildByUid(&root, "Mgmt/ProcessName", &processName);
	veItemAddChildByUid(&root, "Mgmt/ProcessVersion", &processVersion);
	veItemAddChildByUid(&root, "Mgmt/Connection", &connection);
	veItemSetFmt(&processName, veVariantFmt, &none);
	veItemSetFmt(&processVersion, veVariantFmt, &none);
	veItemSetFmt(&connection, veVariantFmt, &none);
}

void valuesUpdate(void)
{
	gpsUpdate();
	veDbusItemUpdate(dbus);
}

/** 50ms tick to invalidate items if there are not received in time */
void valuesTick(void)
{
	gpsTick();
	veItemTick(&root);

	if (timeout && --timeout == 0) {
		logE(MODULE, "timeout connecting device");
		pltExit(129);
	}
}

void gpsConnectedEvent(void)
{
	VeVariant variant;

#if defined(TARGET_pc)
	dbus = veDbusConnect(DBUS_BUS_SESSION);
#else
	dbus = veDbusConnect(DBUS_BUS_SYSTEM);
#endif

	if (!dbus)
		return;

	/* Device found */
	timeout = 0;

	veDbusItemInit(dbus, &root);
	veDbusChangeName(dbus, SERVICE_NAME);

	logI(MODULE, "connected to dbus");

	veItemOwnerSet(&processName, veVariantStr(&variant, pltProgramName()));
	veItemOwnerSet(&processVersion, veVariantStr(&variant, pltProgramVersion()));
	veItemOwnerSet(&connection, veVariantStr(&variant, interface()));
}

/* Stop service if connection lost */
void gpsDisconnectedEvent(void)
{
	logE(MODULE, "connection timeout");
	pltExit(130);
}
