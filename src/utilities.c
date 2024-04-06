#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector2 Utilities_GetScreenCenter(void)
{
	const Vector2 center = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	return center;
}

BoundingBox Utilities_MakeBoundingBox(const Vector3 position, const Vector3 size)
{
	BoundingBox bb = (BoundingBox){
		(Vector3){ position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2 },
		(Vector3){ position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2 }
	};
	return bb;
}

Color Utilities_GetLevelPixelColor(const Color *levelMapPixels, const int x, const int width, const int y)
{
	Color pixelColor = {
		levelMapPixels[y * width + x].r, levelMapPixels[y * width + x].g, levelMapPixels[y * width + x].b, 255
	};

	return pixelColor;
}

bool Utilities_CompareColors(const Color color1, const Color color2)
{
	return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b);
}

float Utilities_MinF(float a, float b) { return (a > b) ? b : a; }

float Utilities_MaxF(float a, float b) { return (a < b) ? b : a; }

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
		token = strtok(nullptr, delim);
	}
	// Remove \r and \n from end of string
	value[strcspn(value, "\r\n")] = '\0';
}

int *Utilities_ParseIntArray(char *input, int *outputCount)
{
	// Create tokens array
	char tokens[strlen(input) + 1];
	strcpy(tokens, input);

	// Get the amount of delimiters, count starts from 1 due to last item not having a delimiter
	int count = 1;
	for (unsigned long i = 0; i < strlen(input); i++)
	{
		if (input[i] == ',')
		{
			count++;
		}
	}

	// Get integers from tokens
	int tokenCount = 0;
	int *output    = calloc(count, sizeof(int));
	for (char *token = strtok(tokens, ","); token != nullptr; token = strtok(nullptr, ","))
	{
		// Add integers to output
		output[tokenCount] = atoi(token);
		tokenCount++;
	}
	// Return the count of tokens in output
	*outputCount = tokenCount;
	return output;
}

int Utilities_GetFileCharacterCount(const char *fileName)
{
	FILE *filePointer = fopen(fileName, "r");
	if (nullptr == filePointer)
	{
		printf("Failed to get character amount from file: %s \n", fileName);
		fclose(filePointer);
		return -1;
	}

	int count = 0;
	int c;
	for (c = getc(filePointer); c != EOF; c = getc(filePointer))
	{
		count++;
	}

	fclose(filePointer);

	return count;
}
