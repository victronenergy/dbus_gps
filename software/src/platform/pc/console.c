#include "dev_reg.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <velib/platform/console.h>
#include <velib/utils/ve_logger.h>
#include <velib/utils/ve_arg.h>

#define MODULE	"console"

/* Global device (app) regs */
DevReg devReg =
{
	0,		// Timeout
	4800	// Baudrate
};

struct option consoleOptions[] =
{
	{"timeout",			required_argument,	0,							't'},
	{"baud",			required_argument,	0,							'b'},
	{0,					0,					0,							0}
};

void consoleUsage(char* program)
{
	printf("%s\n", program);
	printf("\n");
	printf("  -t, --timeout\n");
	printf("   The time in seconds the program waits untill a device is connected. 0 is disabled.\n");
	printf("   If no device is found the program will stop if the timeout is expired.\n");
	printf("\n");
	printf("  -b, --baud\n");
	printf("   The baud rate of the serial port.\n");
	printf("   When omitted the baud rate is set to 4800.\n");
	printf("\n");
	pltOptionsUsage();
	printf("Victron Energy B.V.\n");
}

veBool consoleOption(int flag)
{
	switch(flag)
	{
	case 't':
		errno = 0;
		devReg.timeOut = veArgInt(optarg);
		if (errno == EINVAL) {
			logE(MODULE, "wrong timeout argument");
			pltExit(128);
		}
		break;
	case 'b':
		errno = 0;
		devReg.baudRate = veArgInt(optarg);
		if (errno == EINVAL) {
			logE(MODULE, "wrong baud rate argument");
			pltExit(128);
		}
	}

	return veTrue;
}

veBool consoleArgs(int argc, char *argv[])
{
	VE_UNUSED(argv);

	if (argc != 0)
	{
		printf("error - no arguments are expected - missing a '-' or '--'?");
		return veFalse;
	}

	return veTrue;
}
