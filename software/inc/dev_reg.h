#ifndef DEV_REG_H
#define DEV_REG_H

#include <velib/base/base.h>

typedef struct
{
	un32 timeOut;
	un32 baudRate;
} DevReg;

extern DevReg devReg;

#endif // DEV_REG_H
