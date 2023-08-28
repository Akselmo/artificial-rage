#include "item.h"

// Since we use billboarding we dont have to know rotation
Item_Data Item_Add(float pos_x, float pos_y)
{
	Item_Data item              = {
					 .destroyed = false,
	};
	return item;
}

void Item_Update(Item_Data *item)
{
	Item_Draw(item);
	Item_UpdateCollision(item);
}

void Item_Draw(Item_Data *item) {
	//DrawCubeV( BLUE);
}

void Item_UpdateCollision(Item_Data *item)
{
	// Update bounding box positions
}
