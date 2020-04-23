#include <velib/base/base.h>
#include <velib/platform/console.h>
#include <velib/platform/plt.h>
#include <velib/platform/serial.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_assert.h>
#include <velib/utils/ve_todo.h>

#include "gps.h"
#include "dev_reg.h"
#include "serial_hal.h"

#define MODULE "Serial"

/* The serial port */
static VeSerialPort veSerialPort;
static int interruptLevel = 0;

static void rxCallback(struct VeSerialPortS* port, un8 const* buf, un32 len)
{
	VE_UNUSED(port);

	while (len--)
		gpsFhRxEvent(*buf++);
}

static void handleSerialEvent(struct VeSerialPortS *port, VeSerialEvent event, char const *descr)
{
	VE_UNUSED(port);

	if (event == VE_SERIAL_EV_ERROR) {
		logE("serial", "%s, bailing out", descr);
		pltExit(1);
	}
}

veBool serialHalConnect(void)
{
	veSerialPort.baudrate = devReg.baudRate;
	veSerialPort.dev = (char *) pltGetSerialDevice();
	veSerialPort.rxCallback = rxCallback;
	veSerialPort.intLevel = interruptLevel;

	if (!veSerialOpen(&veSerialPort, NULL))
		return veFalse;

	veSerialEventCallback(&veSerialPort, handleSerialEvent);
	return veSerialSetEol(&veSerialPort, '\n');
}
