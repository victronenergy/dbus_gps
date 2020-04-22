#include <stdio.h>
#include <stdlib.h>

#include <velib/base/base.h>
#include <velib/base/ve_string.h>
#include <velib/vecan/products.h>
#include <velib/types/ve_item.h>
#include <velib/types/variant_print.h>
#include <velib/types/ve_dbus_item.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_item_utils.h>
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

static un32 timeout;

void valuesInit(void)
{
	VeVariant v;

	timeout = devReg.timeOut * 20;

	gpsInit(&root);

	/* App info */
	veItemCreateBasic(&root, "Mgmt/ProcessName", veVariantStr(&v, pltProgramName()));
	veItemCreateBasic(&root, "Mgmt/ProcessVersion", veVariantStr(&v, pltProgramVersion()));
	veItemCreateBasic(&root, "Mgmt/Connection", veVariantStr(&v, "USB"));
}

void valuesUpdate(void)
{
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
	dbus = veDbusGetDefaultBus();

	if (!dbus) {
		logE(MODULE, "dbus connect failed");
		pltExit(1);
	}

	/* Device found */
	timeout = 0;

	veDbusItemInit(dbus, &root);
	if (!veDbusChangeName(dbus, SERVICE_NAME)) {
		logE(MODULE, "dbus name changed failed");
		pltExit(1);
	}

	logI(MODULE, "connected to dbus");
}

/* Stop service if connection lost */
void gpsDisconnectedEvent(void)
{
	logE(MODULE, "connection timeout");
	pltExit(130);
}
