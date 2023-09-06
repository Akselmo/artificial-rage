#include "item.h"

// Since we use billboarding we dont have to know rotation
Item Item_Add(float pos_x, float pos_y)
{
	Item item = {
		.destroyed = false,
	};
	return item;
}

void Item_Update(Item *item)
{
	Item_Draw(item);
	Item_UpdateCollision(item);
}

void Item_Draw(Item *item)
{
	// DrawCubeV( BLUE);
}

void Item_UpdateCollision(Item *item)
{
	// Update bounding box positions
}
