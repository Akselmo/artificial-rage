#include "item.h"
#include "include/raylib.h"

#define ITEM_START_POSITION_Y 0.4f

Vector3 itemPosition;
Vector3 itemSize;

// Since we use billboarding we dont have to know rotation

Item AddItem(float pos_x, float pos_y)
{
    itemPosition = (Vector3) {pos_x, ITEM_START_POSITION_Y, pos_y};
    itemSize     = (Vector3) {0.25f, 0.25f, 0.25f};
    Item item    = {
           .position  = itemPosition,
           .size      = itemSize,
           .destroyed = false,
    };
    return item;
}

void UpdateItem(Item item)
{
    DrawItem(item);
    UpdateItemCollision(item);
}

void DrawItem(Item item)
{
    DrawCubeV(item.position, item.size, BLUE);
}

void UpdateItemCollision(Item item)
{
    // Update bounding box positions
}