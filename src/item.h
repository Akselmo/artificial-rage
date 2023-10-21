#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_

#include "raylib.h"

#define ITEM_START_POSITION_Y 0.4f

typedef struct Item
{
	bool destroyed;
} Item;

Item Item_Add(void);

// TODO: Item could have its types from entity_type
// then make a function that is done whenever player walks over the item
// check what type is and act accordingly
#endif
