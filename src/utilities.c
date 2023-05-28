#include "utilities.h"

Vector2 Utilities_GetScreenCenter(void)
{
    const Vector2 center = {GetScreenWidth() / 2, GetScreenHeight() / 2};
    return center;
}

BoundingBox Utilities_MakeBoundingBox(const Vector3 position, const Vector3 size)
{
    BoundingBox bb = (BoundingBox){
        (Vector3){position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2},
        (Vector3){position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2}};
    return bb;
}

Color Utilities_GetLevelPixelColor(const Color *levelMapPixels, const int x, const int width, const int y)
{
    Color pixelColor = {levelMapPixels[y * width + x].r, levelMapPixels[y * width + x].g, levelMapPixels[y * width + x].b, 255};

    return pixelColor;
}

bool Utilities_CompareColors(const Color color1, const Color color2)
{
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}

float Utilities_MinF(float a, float b)
{
    return (a > b) ? b : a;
}

float Utilities_MaxF(float a, float b)
{
    return (a < b) ? b : a;
}

void Utilities_ParseKeyValuePair(char *input, char *key, char *delim, char *value)
{
    char *token = strtok(input, delim);
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            strcpy(key, token);
        }
        else if (i == 1)
        {
            strcpy(value, token);
        }
        token = strtok(NULL, delim);
    }
    // Remove \r and \n from end of string
    value[strcspn(value, "\r\n")] = 0;
}

void Utilities_ParseIntArray(char *input, const int size, int *output)
{
    // Allocates the output with size, then parses the integer array with "," as delimiter
    // Thanks to: https://stackoverflow.com/a/29589157 !
    // NOTE: Feel free to improve this, however this seems to work nicely.
    output = calloc(size, sizeof(int));
    char *end = input;
    int i = 0;
    while(*end)
    {
        int n = strtol(input, &end, 10);
        while (*end == ',')
        {
            end++;
        }
        input = end;
        // Assign the parsed numbers to the output array
        output[i] = n;
        i++;
    }
}

int Utilities_GetFileCharacterCount(const char *fileName)
{
    FILE *filePointer = fopen(fileName, "r");
    if (NULL == filePointer)
    {
        printf("Failed to get character amount from file: %s \n", fileName);
        return -1;
    }

    int count = 0;
    char c;
    for (c = getc(filePointer); c != EOF; c = getc(filePointer))
    {
        count++;
    }

    fclose(filePointer);

    return count;
}
