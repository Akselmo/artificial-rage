#include "item.h"

// Since we use billboarding we dont have to know rotation
Item Item_Add(void)
{
	Item item = { .destroyed = false };
	return item;
}
