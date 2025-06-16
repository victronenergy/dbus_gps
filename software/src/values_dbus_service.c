#include <stdio.h>
#include <stdlib.h>

#include <velib/base/base.h>
#include <velib/base/ve_string.h>
#include <velib/vecan/products.h>
#include <velib/types/ve_item.h>
#include <velib/types/variant_print.h>
#include <velib/types/ve_dbus_item.h>
#include <velib/types/ve_str.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_item_utils.h>
#include <velib/platform/console.h>
#include <velib/platform/plt.h>

#include "values.h"
#include "serial_hal.h"
#include "gps.h"
#include "dev_reg.h"

#define MODULE			"VALUES"

/** dbus connection */
struct VeDbus *dbus;

/** The root of the tree to locate values and format them etc */
static VeItem root;

static un32 timeout;

char const *serialPortShort(void)
{
	char const *device = pltGetSerialDevice();

	if (strstr(device, "/dev/tty") == device)
		device += strlen("/dev/tty");

	return device;
}

char const *serialPort(void)
{
	char const *device = pltGetSerialDevice();

	if (strstr(device, "/dev/tty") == device)
		device += strlen("/dev/");

	return device;
}

void valuesInit(void)
{
	VeVariant v;
	VeStr s;

	timeout = devReg.timeOut * 20;

	gpsInit(&root);

	/* App info */
	veItemCreateBasic(&root, "Mgmt/ProcessName", veVariantStr(&v, pltProgramName()));
	veItemCreateBasic(&root, "Mgmt/ProcessVersion", veVariantStr(&v, pltProgramVersion()));
	veItemCreateBasic(&root, "Mgmt/Connection", veVariantStr(&v, "USB"));
	veItemCreateProductId(&root, VE_PROD_ID_NMEA_0183_GPS);

	veStrNewFormat(&s, "%s (%s)", devReg.name, serialPortShort());
	veItemCreateBasic(&root, "ProductName", veVariantHeapStr(&v, veStrCStr(&s)));
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
	VeVariant v;
	VeStr s;
	sn32 instance;

	dbus = veDbusGetDefaultBus();

	if (!dbus) {
		logE(MODULE, "dbus connect failed");
		pltExit(1);
	}

	veDbusSetListeningDbus(dbus);
	veStrNewFormat(&s, "vegps_%s", serialPort());
	instance = veDbusGetVrmDeviceInstance(veStrCStr(&s), "gps", 0);
	veStrFree(&s);
	if (instance < 0) {
		logE(MODULE, "getting a VRM instance failed");
		pltExit(1);
	}
	veItemCreateBasic(&root, "DeviceInstance", veVariantSn32(&v, instance));

	/* Device found */
	timeout = 0;

	veDbusItemInit(dbus, &root);
	veStrNewFormat(&s, "com.victronenergy.gps.ve_%s", serialPort());
	if (!veDbusChangeName(dbus, veStrCStr(&s))) {
		logE(MODULE, "dbus name changed failed");
		pltExit(1);
	}
	veStrFree(&s);

	logI(MODULE, "connected to dbus");
}

/* Stop service if connection lost */
void gpsDisconnectedEvent(void)
{
	logE(MODULE, "connection timeout");
	pltExit(130);
}
