#ifndef VERSION_H
#define VERSION_H

/* Firmware version */
#define FIRMWARE_VERSION			0x000111

#define n2c(a,n)	(((((un32)(a)>>n)& 0xF) > 9 ? 'a'-10 : '0') + (((un32)(a)>>n)& 0xF))

// NMEA 2000 expects a string version. To makes sure it is the same as the
// numeric value it is build from it. The string is build at complile time.
// Leading zero's are ignored, i.o.w. 0x000203 becomes "2.03"
// Get the characters first..

#if ((FIRMWARE_VERSION >> 20) & 0xF) != 0		// 6th char not 0
#define V6		n2c(FIRMWARE_VERSION, 20),
#define V5 		n2c(FIRMWARE_VERSION, 16),'.'
#define V4		n2c(FIRMWARE_VERSION, 12),

#else											// 6th char 0
#define V6

#if ((FIRMWARE_VERSION >> 16) & 0xF) != 0		// 5th char not 0
#define V5 		n2c(FIRMWARE_VERSION, 16),'.',
#define V4		n2c(FIRMWARE_VERSION, 12),
#else											// 5th char 0 as well
#define V5

// Ignore 4th char when zero.
#if ((FIRMWARE_VERSION >> 12) & 0xF) != 0		// 4th char 0
#define V4		n2c(FIRMWARE_VERSION, 12),
#else											// 4th char 0 as well
#define V4
#endif											// 4th char not 0
#endif											// 5th char not 0
#endif											// 6th char not 0

#define V3 		n2c(FIRMWARE_VERSION, 8)
#define V2		n2c(FIRMWARE_VERSION, 4),
#define V1 		n2c(FIRMWARE_VERSION, 0)

// The complete version string.
// (some comma's are missing on purpose since they are included in the value,
// since they might no be set at all)
#define VERSION_STR		{V6 V5 V4 V3,'.', V2 V1, 0}

#endif // VERSION_H
