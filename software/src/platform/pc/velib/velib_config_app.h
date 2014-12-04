#ifndef _VELIB_CONFIG_APP_H_
#define _VELIB_CONFIG_APP_H_

#include <velib/velib_config_app_common.h>

#define CFG_ASSERT_ENABLED					1
#define CFG_WITH_VE_STAMP					1
#define CFG_WITH_VE_STR						1

/* log with timestamps */
#define CFG_WITH_LOG						1
#define CFG_LOG_STAMP_DEC					1

/* compile in serial support */
#define CFG_WITH_SERIAL						1
/* and open the port at startup */
#define CFG_INIT_SERIAL						1

#define CFG_SERIAL_RX_BUFFER_LEN			2048

/* version and banner commandline option */
#define CFG_WITH_VERSION					1

#endif

