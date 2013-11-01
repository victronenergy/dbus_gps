#ifndef SERIAL_HAL_H
#define SERIAL_HAL_H

#include <velib/base/base.h>
#include <velib/types/variant.h>

void		serialHalUpdate(void);
veBool		serialHalConnect(void);
VeVariant *	serialHalInstance(VeVariant *var);

#endif // SERIAL_HAL_H
