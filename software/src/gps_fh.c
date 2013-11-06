#include "gps.h"

#include <velib/base/base.h>
#include <velib/utils/ve_logger.h>

#define MODULE "GpsFh"

/* Hex nibble to binairy value */
#define n2b(a)	\
	( ((a)>='0' && (a)<='9') ?  (a)-'0' : ( ((a)>='A' && (a)<='F') ? ((a)-'A')+10 : 0xFF ) )

/* Common case statenents for detecting frame errors, used in multiple states */
#define CASE_FRAME_ERROR							\
	case '$':										\
	case '\r':										\
	case '\n':										\
		error(GPS_ERR_FRAME_ERROR, &c);				\
		setState( (c == '$') ? RECEIVING : WAIT);	\
		break;

typedef enum
{
	GPS_ERR_NO_ERROR,
	GPS_ERR_FRAME_ERROR,
	GPS_ERR_CHECKSUM_ERROR,
	GPS_ERR_BUFFER_OVERFLOW
} GpsError;

typedef struct
{
	struct
	{
		un32 frameErrors;
		un32 checksumErrors;
		un32 bufferOverflows;
	} errors;
} GpsFhStats;
static GpsFhStats stats;

typedef enum
{
	WAIT,
	RECEIVING,
	CHECKSUM_L,
	CHECKSUM_H
} State;
static State state;

static un8 checksum;

/* A sentence may contain up to 80 characters plus "$" and CR/LF. */
#define SENTENCE_LEN		80		/* excluding *<checksum> */
static char sentence[SENTENCE_LEN];
static char *ptr;

static void error(GpsError e, void *ctx)
{
	logI(MODULE, "sentence dump: %.*s", ptr - sentence, sentence);

	switch (e)
	{
	case GPS_ERR_FRAME_ERROR:
		stats.errors.frameErrors++;
		logW(MODULE, "gps error: GPS_ERR_FRAME_ERROR. Unexpected "
				"character ´%c´ (0x%02x)", *(char *)ctx, *(un8 *)ctx);
		return;
	case GPS_ERR_CHECKSUM_ERROR:
		stats.errors.checksumErrors++;
		logW(MODULE, "gps error: GPS_ERR_CHECKSUM_ERROR. "
				"Checksum is %02x", *(un8 *)ctx);
		return;
	case GPS_ERR_BUFFER_OVERFLOW:
		stats.errors.bufferOverflows++;
		logW(MODULE, "gps error: GPS_ERR_BUFFER_OVERFLOW");
		return;
	default:
		logW(MODULE, "gps error: unknown error");
	}
}

static void setState(State next)
{
	switch (next)
	{
	case RECEIVING:
		checksum = 0;
		ptr = sentence;
		break;
	default:
		;
	}
	state = next;
}

/* @note main context */
void gpsFhRxEvent(char c)
{
	switch (state)
	{
	case WAIT:
		if (c == '$')
			setState(RECEIVING);
		break;
	case RECEIVING:
		switch (c)
		{
		CASE_FRAME_ERROR
		case '*':
			setState(CHECKSUM_H);
			break;
		default:
			checksum ^= c;
			if (ptr < &sentence[SENTENCE_LEN])
				*ptr++ = c;
		}
		break;
	case CHECKSUM_H:
		switch (c)
		{
		CASE_FRAME_ERROR
		default:
			checksum ^= n2b(c) << 4;
			setState(CHECKSUM_L);
		}
		break;
	case CHECKSUM_L:
		switch (c)
		{
		CASE_FRAME_ERROR
		default:
			checksum ^= n2b(c);
			setState(WAIT);
			if (checksum != 0) {
				error(GPS_ERR_CHECKSUM_ERROR, &checksum);
				break;
			}
			if (ptr >= &sentence[SENTENCE_LEN]) {
				error(GPS_ERR_BUFFER_OVERFLOW, NULL);
				break;
			}
			*ptr = 0;

			gpsActivity();
			gpsFrameEvent(sentence, ptr - sentence);
		}
		break;
	}
}
