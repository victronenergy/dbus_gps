#ifndef _VELIB_CONFIG_APP_H_
#define _VELIB_CONFIG_APP_H_

#include <velib/velib_config_app_common.h>
#include <velib/canhw/canhw_driver_config.h>

#ifdef NMEA2K
#define CFG_WITH_CANHW						1
#define CFG_WITH_TIMER						1
#define CFG_WITH_J1939_SF_MEM				1
#define CFG_WITH_J1939_DEVICE				1
#define CFG_WITH_FP_DEF						1
#define CFG_WITH_VE_TIMER					1
#define CFG_WITH_TASK						1
#define CFG_WITH_CANHW_DRIVER				1
#endif

#define CFG_ASSERT_ENABLED					1
#define CFG_WITH_VE_STAMP					1
#define CFG_WITH_VE_STR						1

/* log with timestamps and mk2 */
#define CFG_WITH_LOG						1
#define CFG_LOG_STAMP_DEC					1
#define CFG_WITH_VE_RAND					1
#define CFG_WITH_TIMER						1

/* compile in mk2 serial support */
#define CFG_WITH_SERIAL						1
/* and open the port at startup */
#define CFG_INIT_SERIAL						1

#define CFG_SERIAL_RX_BUFFER_LEN			2048

/* version and banner commandline option */
#define CFG_WITH_VERSION					1

#endif

