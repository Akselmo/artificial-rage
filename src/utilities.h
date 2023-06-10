#pragma once
#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 16384 // I like big buffers and cant lie

Vector2 Utilities_GetScreenCenter(void);
BoundingBox Utilities_MakeBoundingBox(Vector3 position, Vector3 size);
Color Utilities_GetLevelPixelColor(const Color *levelMapPixels, const int x, const int width, const int y);
bool Utilities_CompareColors(Color color1, Color color2);
float Utilities_MinF(float a, float b);
float Utilities_MaxF(float a, float b);
void Utilities_ParseKeyValuePair(char *buffer, char *key, char *delim, char *value);
int *Utilities_ParseIntArray(char *input, int *outputCount);
int Utilities_GetFileCharacterCount(const char *fileName);
#endif
