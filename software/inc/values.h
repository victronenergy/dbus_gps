#ifndef _VALUES_H_
#define _VALUES_H_

#include <velib/types/ve_item.h>
#include <velib/types/variant_print.h>

/**
 * To seperate gui logic / any other logic from the communication
 * the received values are stored in a tree in SI units. This struct
 * contains the field needed to create the tree.
 */
typedef struct
{
	struct VeItem *		item;
	VeVariant *			local;
	char const *		id;
	VeVariantUnitFmt *	fmt;
	un8					timeout;
} ItemInfo;

void valuesInit(void);
void valuesTick(void);

void valuesInitDoneCallback(void);
void valuesInvalidate(void);
void valuesAddItemByInfo(ItemInfo const *itemInfo);
void valuesDisconnectedEvent(void);

#endif
