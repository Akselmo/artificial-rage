#include "item.h"

// Since we use billboarding we dont have to know rotation
Item Item_Add(const int value)
{
	Item item = { .destroyed = false, .value = value };
	return item;
}
