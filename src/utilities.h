#pragma once
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

Vector2 Utilities_GetScreenCenter(void);
BoundingBox Utilities_MakeBoundingBox(Vector3 position, Vector3 size);
Color Utilities_GetLevelPixelColor(const Color* levelMapPixels, const int x, const int width, const int y);
bool Utilities_CompareColors(Color color1, Color color2);
#endif
