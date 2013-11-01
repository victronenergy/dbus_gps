#include <velib/base/base.h>
#include <velib/base/ve_string.h>
#include <velib/types/ve_item.h>
#include <velib/utils/ve_logger.h>

#include "values.h"
#include "gps.h"

#include <string.h>
#include <errno.h>
#include <math.h>
#include <time.h>

#ifdef WIN32
#include <stdio.h>
#else
#include <stdlib.h>
#endif

#define MODULE	"GPS"

/* ASCII digit to binairy value */
#define a2b(a)						((a) - '0')

/* GPRMS field number */
#define GPS_RMC_UTC_TIME			1
#define GPS_RMC_STATUS				2
#define GPS_RMC_LATITUDE			3
#define GPS_RMC_LAT_NORTH_SOUTH		4
#define GPS_RMC_LONGITUDE			5
#define GPS_RMC_LONG_EAST_WEST		6
#define GPS_RMC_SPEED				7
#define GPS_RMC_TRUE_COURSE			8
#define GPS_RMC_UTC_DATE			9
#define GPS_RMC_VARIATION			10
#define GPS_RMC_VAR_EAST_WEST		11
#define GPS_RMC_MODE				12

typedef void (*GpsSentenceParser)(un8 index, char *value);

#define F_CONNECTED					1
static un8 flags;

typedef struct
{
	VeVariant fix;
	VeVariant timestamp;
	VeVariant latitude;
	VeVariant longitude;
	VeVariant variation;
	VeVariant speed;
	VeVariant course;

	struct tm time;
} GpsData;
static GpsData local;

/* Container for VeItems */
static Gps gps;

static VeVariantUnitFmt none	= {	0,	""		};
static VeVariantUnitFmt deg		= {	5,	"deg"	};
static VeVariantUnitFmt course	= {	2,	"deg"	};
static VeVariantUnitFmt speed	= {	2,	"m/s"	};

static ItemInfo const itemInfo[] =
{
	{	&gps.product.connected,			NULL,				"Connected",			&none,		0	},
	{	&gps.product.name,				NULL,				"ProductName",			&none,		0	},
	{	&gps.product.id,				NULL,				"ProductId",			&none,		0	},
	{	&gps.product.firmwareVersion,	NULL,				"FirmwareVersion",		&none,		0	},
	{	&gps.product.hardwareRevision,	NULL,				"HardwareVersion",		&none,		0	},
	{	&gps.product.instance,			NULL,				"DeviceInstance",		&none,		0	},
	{	&gps.fix,						NULL,				"Fix",					&none,		5	},
	{	&gps.latitude,					&local.latitude,	"Position/Latitude",	&deg,		5	},
	{	&gps.longitude,					&local.longitude,	"Position/Longitude",	&deg,		5	},
	{	&gps.variation,					&local.variation,	"MagneticVariation",	&deg,		5	},
	{	&gps.speed,						&local.speed,		"Speed",				&speed,		5	},
	{	&gps.course,					&local.course,		"Course",				&course,	5	}
};

void gpsInit(VeItem *root)
{
	un8 i;
	for (i = 0; i < ARRAY_LENGTH(itemInfo); i++) {
		veItemAddChildByUid(root, itemInfo[i].id, itemInfo[i].item);
		veItemSetFmt(itemInfo[i].item, veVariantFmt, itemInfo[i].fmt);
		veItemSetTimeout(itemInfo[i].item, itemInfo[i].timeout);
	}
}

void gpsUpdate(void)
{
	un8 i;
	VeVariant variant;

	if ((flags & F_CONNECTED) == 0)
		return;

	veItemOwnerSet(&gps.fix, &local.fix);
	veItemOwnerSet(&gps.product.connected, veVariantUn32(&variant, 1));

	/* Fix? */
	if (gps.fix.variant.value.UN32 == 0)
		return;

	/* Copy local values to items */
	for (i = 0; i < ARRAY_LENGTH(itemInfo); i++) {
		if (itemInfo[i].local)
			veItemOwnerSet(itemInfo[i].item, itemInfo[i].local);
	}
}

void gpsTick(void)
{

}

/* Convert DDDMM.MMMM to degrees */
static float toDeg(float value)
{
	double degrees;

	/* Extract degrees */
	modf(value / 100, &degrees);

	/* Get MM.MMMM part and convert to degrees */
	return (float)fmod(value, 100) * 60 / 3600 + degrees;
}

/*
 * RMC Recommended Minimum Navigation Information
 *
 * 1         2 3       4 5        6 7   8   9    10  11
 * |         | |       | |        | |   |   |    |   |
 * hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a

 * 1) Time (UTC)
 * 2) Status, V = Navigation receiver warning
 * 3) Latitude
 * 4) N or S
 * 5) Longitude
 * 6) E or W
 * 7) Speed over ground, knots
 * 8) Track made good, degrees true
 * 9) Date, ddmmyy
 * 10) Magnetic Variation, degrees
 * 11) E or W
*/
static void parseGPRMC(un8 index, char *value)
{
	if (*value == 0)
		return;

	switch (index)
	{
	case GPS_RMC_STATUS:
		/* A: fix; V: no fix */
		veVariantUn8(&local.fix, value[0] == 'A');
		return;

	case GPS_RMC_LATITUDE:
		errno = 0;
		veVariantFloat(&local.latitude, toDeg((float)atof(value)));
		if (errno)
			veVariantInvalidate(&local.latitude);
		return;

	case GPS_RMC_LAT_NORTH_SOUTH:
		/* North is positive, south is negative */
		if (value[0] == 'S')
			local.latitude.value.Float *= -1;
		return;

	case GPS_RMC_LONGITUDE:
		errno = 0;
		veVariantFloat(&local.longitude, toDeg((float)atof(value)));
		if (errno)
			veVariantInvalidate(&local.longitude);
		return;

	case GPS_RMC_LONG_EAST_WEST:
		/* East is positive, west is negative */
		if (value[0] == 'W')
			local.longitude.value.Float *= -1;
		return;

	case GPS_RMC_VARIATION:
		errno = 0;
		veVariantFloat(&local.variation, toDeg((float)atof(value)));
		if (errno)
			veVariantInvalidate(&local.variation);
		return;

	case GPS_RMC_VAR_EAST_WEST:
		/* East is positive, west is negative */
		if (value[0] == 'W')
			local.variation.value.Float *= -1;
		logI(MODULE, "variation %f", local.variation.value.Float);
		return;

	case GPS_RMC_UTC_TIME:
		memset(&local.time, 0, sizeof(struct tm));
		local.time.tm_hour = a2b(*value++) * 10;
		local.time.tm_hour += a2b(*value++);
		local.time.tm_min = a2b(*value++) * 10;
		local.time.tm_min += a2b(*value++);
		local.time.tm_sec = a2b(*value++ ) * 10;
		local.time.tm_sec += a2b(*value++);
		return;

	case GPS_RMC_UTC_DATE:
		local.time.tm_mday = a2b(*value++) * 10;
		local.time.tm_mday += a2b(*value++);
		local.time.tm_mon = a2b(*value++) * 10;
		local.time.tm_mon += a2b(*value++) - 1;		/* 0-11 */
		local.time.tm_year = a2b(*value++) * 10;
		local.time.tm_year += a2b(*value++) + 100;	/* Since 1900 */
		veVariantUn32(&local.timestamp, (un32)mktime(&local.time));
		return;

	case GPS_RMC_SPEED:
		errno = 0;
		veVariantFloat(&local.speed, (float)atof(value));
		if (errno) {
			veVariantInvalidate(&local.speed);
			return;
		}
		/* Knots to m/s */
		local.speed.value.Float += 0.51;
		return;

	case GPS_RMC_TRUE_COURSE:
		errno = 0;
		veVariantFloat(&local.course, (float)atof(value));
		if (errno)
			veVariantInvalidate(&local.course);
		return;
	}
}

/* @note main context */
void gpsFrameEvent(char *sentence, un8 len)
{
	char *ptr;
	char *value;
	un8 index;

	GpsSentenceParser parser = NULL;

	//logI(MODULE, "%s", sentence);

	if (ve_strnicmp("GPRMC", sentence, 5) == 0)
		parser = parseGPRMC;

	if (parser) {
		index = 0;
		value = ptr = sentence;
		while (*ptr && len) {
			/* Scan for comma */
			if (*ptr == ',') {
				*ptr = 0;

				/* Skip identifier */
				if (index)
					parser(index, value);

				/* Next field */
				index++;
				value = ptr + 1; /* Skip null terminator */
			}
			ptr++, len--;
		}
	}
}

void gpsActivity(void)
{
	if (flags & F_CONNECTED)
		return;

	flags |= F_CONNECTED;
	gpsConnectedEvent();
}
