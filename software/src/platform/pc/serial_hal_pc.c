#include <velib/base/base.h>
#include <velib/platform/console.h>
#include <velib/platform/plt.h>
#include <velib/platform/serial.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_assert.h>
#include <velib/utils/ve_todo.h>

#include "gps.h"
#include "dev_reg.h"

#define MODULE "Serial"

/* The serial port */
static VeSerialPort veSerialPort;

#if CFG_WITH_PTHREADS
/* Rx buffer. Note that it is shared between main loop and interrupt context */
static un8 rxBuffer[CFG_SERIAL_RX_BUFFER_LEN];
static unsigned int rxHead;
static unsigned int rxTail;
static int interruptLevel = 1;

void serialHalUpdate(void)
{
	un8 c;

	pltInterruptSuspend();

	/* Until it is empty */
	while (rxTail != rxHead) {
		c = rxBuffer[rxTail];

		/* Forward to protocol handlers */
		gpsFhRxEvent(c);

		if (++rxTail >= ARRAY_LENGTH(rxBuffer))
			rxTail = 0;
	}

	pltInterruptResume();
}

/* @note Interrupt context */
static void rxCallback(struct VeSerialPortS* port, un8 const* buf, un32 len)
{
	VE_UNUSED(port);

	while (len--) {
		if (++rxHead >= ARRAY_LENGTH(rxBuffer))
			rxHead = 0;

		/*
		 * note: the old data is thrown away in case of a buffer overflow,
		 * to make sure there is enough space to sync again, and subsequent
		 * characters are not dropped again directly.
		 */
		if (rxHead == rxTail)
			logW(MODULE, "rx buffer overflow");

		rxBuffer[rxHead] = *buf++;
	}
}

#else

static int interruptLevel = 0;

void serialHalUpdate(void) {}

static void rxCallback(struct VeSerialPortS* port, un8 const* buf, un32 len)
{
	while (len--)
		gpsFhRxEvent(*buf++);
}

#endif

static void handleSerialEvent(struct VeSerialPortS *port, VeSerialEvent event, char const *descr)
{
	if (event == VE_SERIAL_EV_ERROR) {
		logE("serial", "%s, bailing out", descr);
		pltExit(1);
	}
}

veBool serialHalConnect(void)
{
	veSerialPort.baudrate = devReg.baudRate;
	veSerialPort.dev = pltGetSerialDevice();
	veSerialPort.rxCallback = rxCallback;
	veSerialPort.intLevel = interruptLevel;

	if (!veSerialOpen(&veSerialPort, NULL))
		return veFalse;

	veSerialEventCallback(&veSerialPort, handleSerialEvent);
	return veSerialSetEol(&veSerialPort, '\n');
}

VeVariant *serialHalInstance(VeVariant *var)
{
	return veSerialGetDeviceNumber(&veSerialPort, var);
}
