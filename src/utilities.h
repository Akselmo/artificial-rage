#pragma once
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

Vector2 Utilities_GetScreenCenter();
BoundingBox Utilities_MakeBoundingBox(Vector3 position, Vector3 size);
Color Utilities_GetLevelPixelColor(Color* levelMapPixels, int x, int width, int y);
bool Utilities_CompareColors(Color color1, Color color2);
int Utilities_PlayAnimation(Model model, ModelAnimation* animations, int frameCounter);

typedef enum AnimationID
{
    IDLE   = 0,
    MOVE   = 1,
    HIT    = 2,
    ATTACK = 3,
    DEATH  = 4,
};

#endif