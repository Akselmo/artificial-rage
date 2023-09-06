#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_

#include "raylib.h"

#define ITEM_START_POSITION_Y 0.4f

enum Item_Type
{
	ITEM_HEALTH_SMALL,
	ITEM_HEALTH_MEDIUM,
	ITEM_HEALTH_BIG,
};

typedef struct Item
{
	enum Item_Type type;
	bool destroyed;
} Item;

Item Item_Add(float pos_x, float pos_y);
void Item_Update(Item *item);
void Item_Draw(Item *item);
void Item_UpdateCollision(Item *item);

#endif
