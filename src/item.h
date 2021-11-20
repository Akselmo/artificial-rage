#ifndef ITEM
#define ITEM

#include "include/raylib.h"

typedef struct Item
{
    int itemtype;
    Vector3 position;
    Vector3 size;
    BoundingBox boundingBox;
    bool destroyed;
} Item;

Item AddItem(float pos_x, float pos_y);

void UpdateItem(Item item);
void DrawItem(Item item);
void UpdateItemCollision(Item item);

#endif
