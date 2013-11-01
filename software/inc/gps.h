#ifndef GPS_H
#define GPS_H

#include <velib/base/base.h>
#include <velib/types/ve_item.h>

typedef struct
{
	VeItem name;
	VeItem id;
	VeItem instance;
	VeItem connected;
	VeItem hardwareRevision;
	VeItem firmwareVersion;
} ProductInfo;

typedef struct
{
	VeItem longitude;
	VeItem latitude;
} GpsPosition;

typedef struct
{
	ProductInfo	product;

	VeItem		fix;
	VeItem		longitude;
	VeItem		latitude;
	VeItem		variation;
	VeItem		speed;
	VeItem		course;
} Gps;

void gpsInit(VeItem *root);
void gpsUpdate(void);
void gpsTick(void);
void gpsActivity(void);

void gpsFhRxEvent(char c);
void gpsConnectedEvent(void);
void gpsDisconnectedEvent(void);
void gpsFrameEvent(char *sentence, un8 len);

#endif // GPS_H
