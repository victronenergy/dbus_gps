#ifndef _VELIB_CONFIG_APP_H_
#define _VELIB_CONFIG_APP_H_

#define VELIB_EXPECTED_TRACKING_NR			11

#ifdef DEBUG
//#define CFG_ASSERT_ENABLED					1
#endif

/* utils */
#define CFG_WITH_VE_ARG						1

#define CFG_WITH_TASK						1
#define CFG_INIT_DEFAULT_DBUS				1
#define CFG_DBUS_NON_BLOCKING				1
#define CFG_DBUS_ITEM_NO_TYPE_CHANGE		1

/* non blocking version */
#define CFG_WITH_LIBEVENT					1
#define CFG_WITH_TASK_LIBEVENT				1
#define CFG_NO_PREEMPTION					1
#define CFG_EXIT_ON_OOM						1

#define CFG_WITH_STRING						1
#define CFG_VARIANT_HEAP					1
#define CFG_WITH_TASK						1
#define CFG_WITH_VE_ITEM					1
#define CFG_WITH_VE_DBUS_ITEM				1
#define CFG_WITH_VE_DBUS_ITEM_CONSUMER		1
#define CFG_WITH_ITEM_UTILS					1
#define CFG_WITH_VARIANT					1
#define CFG_WITH_VARIANT_PRINT				1
#define CFG_WITH_FLOAT						1

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

