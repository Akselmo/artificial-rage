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

#endif
