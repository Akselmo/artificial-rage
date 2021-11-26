#include "include/raylib.h"
#include "include/raymath.h"
#include "utilities.h"
#include <stdio.h>


Vector2 GetScreenCenter()
{
   Vector2 center = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
   return center;
}

BoundingBox MakeBoundingBox(Vector3 position, Vector3 size)
{
    BoundingBox bb = (BoundingBox){(Vector3){position.x - size.x / 2,
                                             position.y - size.y / 2,
                                             position.z - size.z / 2},
                                   (Vector3){position.x + size.x / 2,
                                             position.y + size.y / 2,
                                             position.z + size.z / 2}};
    return bb;
}
