#include "include/raylib.h"
#include "include/raymath.h"
#include "utilities.h"
#include <stdio.h>


Vector2 GetScreenCenter()
{
   Vector2 center = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
   return center;
}

