#include "item.h"

// Since we use billboarding we dont have to know rotation
Item_Data Item_Add(float pos_x, float pos_y)
{
    const Vector3 Item_position = (Vector3) { pos_x, ITEM_START_POSITION_Y, pos_y };
    const Vector3 Item_size     = (Vector3) { 0.25f, 0.25f, 0.25f };
    Item_Data item              = {
                     .position  = Item_position,
                     .size      = Item_size,
                     .destroyed = false,
    };
    return item;
}

void Item_Update(Item_Data* item)
{
    Item_Draw(item);
    Item_UpdateCollision(item);
}

void Item_Draw(Item_Data* item)
{
    DrawCubeV(item->position, item->size, BLUE);
}

void Item_UpdateCollision(Item_Data* item)
{
    // Update bounding box positions
}