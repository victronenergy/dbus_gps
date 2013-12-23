#ifndef _VELIB_CONFIG_APP_COMMON_H_
#define _VELIB_CONFIG_APP_COMMON_H_

#define VELIB_EXPECTED_TRACKING_NR			11

#ifdef DEBUG
//#define CFG_ASSERT_ENABLED					1
#endif

// not NMEA2K, dummy to no cause compile errors
#define CFG_PROD_MESSAGE_NUMBER				1

/* utils */
#define CFG_UTILS_TODO_ENABLED				1
#define CFG_WITH_VE_ENDIAN_GENERIC			1
#define CFG_WITH_VE_ARG						1

#ifdef DBUS
# define CFG_WITH_TASK						1
# define CFG_INIT_CTX						1
# define CFG_WITH_CTX						1

# if 1
/* non blocking version */
#  define CFG_WITH_LIBEVENT					1
#  define CFG_WITH_TASK_LIBEVENT			1
#  define CFG_WITH_PTHREADS					1
# else
/* blocking version */
#  define CFG_WITH_TASK_NATIVE				1
# endif

# define CFG_WITH_STRING					1
# define CFG_WITH_TASK						1
# define CFG_WITH_VE_ITEM					1
# define CFG_WITH_VE_DBUS_ITEM				1
# define CFG_WITH_VARIANT					1
# define CFG_WITH_VARIANT_PRINT				1
# define CFG_WITH_FLOAT						1
# define CFG_WITH_VECAN_PRODUCT_NAMES		1

#else
# define CFG_VEBUS_DEVICES					1
#endif

#endif
