#include "values.h"
#include "version.h"
#include "serial_hal.h"

#include <velib/platform/plt.h>
#include <velib/platform/console.h>
#include <velib/utils/ve_logger.h>

void taskInit(void)
{
	valuesInit();

	if (!serialHalConnect())
		pltExit(-1);

	pltInterruptEnable();
}

void taskUpdate(void)
{
	serialHalUpdate();
}

void taskTick(void)
{
	valuesTick();
}

static char const version[] = VERSION_STR;
char const *pltProgramVersion(void)
{
	return version;
}
