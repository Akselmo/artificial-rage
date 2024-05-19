#pragma once

#include "raylib.h"

namespace Utilities
{

#define DEFAULT_BUFFER_SIZE 16384 // I like big buffers and cant lie

Vector2 getScreenCenter(void);
BoundingBox makeBoundingBox(Vector3 position, Vector3 size);
Color getLevelPixelColor(const Color *levelMapPixels, const int x, const int width, const int y);
bool compareColors(Color color1, Color color2);
float minF(float a, float b);
float maxF(float a, float b);
void parseKeyValuePair(char *buffer, char *key, char *delim, char *value);
int *parseIntArray(char *input, int *outputCount);
int getFileCharacterCount(const char *fileName);

}; // namespace Utilities
