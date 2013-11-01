#include <velib/base/base.h>
#include <velib/platform/plt.h>
#include <velib/platform/serial.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_assert.h>
#include <velib/utils/ve_todo.h>

#include "gps.h"

#define MODULE "Serial"

/* The serial port */
static VeSerialPort veSerialPort;

/* Rx buffer. Note that it is shared between main loop and interrupt context */
static un8 rxBuffer[CFG_SERIAL_RX_BUFFER_LEN];
static un8 rxHead;
static un8 rxTail;

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
static void rxCallback(struct VeSerialPortS* port, un8 byte)
{
	un8 rxNext;

	VE_UNUSED(port);

	rxNext = rxHead + 1;
	if (rxNext >= ARRAY_LENGTH(rxBuffer))
		rxNext = 0;

	/* Check if buffer is full */
	veAssert(rxNext != rxTail);

	/* Append */
	if (rxNext != rxTail) {
		rxBuffer[rxHead] = byte;
		rxHead = rxNext;
		veTodo();
	}
}

veBool serialHalConnect(void)
{
	veSerialPort.baudrate = 4800;
	veSerialPort.dev = pltGetSerialDevice();
	veSerialPort.rxCallback = rxCallback;
	veSerialPort.intLevel = 1;

	return veSerialOpen(&veSerialPort, NULL);
}

VeVariant *serialHalInstance(VeVariant *var)
{
	return veSerialGetDeviceNumber(&veSerialPort, var);
}
