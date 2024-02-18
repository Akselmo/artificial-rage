#include "settings.h"
#include "raylib.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: List all input keys
// c: int Settings_KeyForward
// h: extern int Settings_KeyForward
// And so on
struct Settings_Data Settings;

// Private
void Settings_Read(Settings_Data *settings);
void Settings_Write(Settings_Data *settings);
Settings_Data Settings_CreateDefault(void);
bool Settings_Parse(Settings_Data *settings, char *key, float value);
const char *SETTINGS_FILENAME = "settings.cfg";

void Settings_Initialize(void)
{
	// Write default settings
	Settings = Settings_CreateDefault();

	if (access(SETTINGS_FILENAME, F_OK) == 0)
	{
		Settings_Read(&Settings);
	}
	else
	{
		Settings_Write(&Settings);
	}

	InitWindow(Settings.screenWidth, Settings.screenHeight, "Artificial Rage");

	SetTargetFPS(Settings.maxFPS);
}

Settings_Data Settings_CreateDefault(void)
{

	Settings_Data data = { .screenWidth      = 800,
		                   .screenHeight     = 600,
		                   .mouseSensitivity = 0.25f,
		                   .cameraFOV        = 90,
		                   .maxFPS           = 200,
		                   .keyMoveForward   = KEY_W,
		                   .keyMoveBackward  = KEY_S,
		                   .keyMoveLeft      = KEY_A,
		                   .keyMoveRight     = KEY_D,
		                   .keyFire          = MOUSE_BUTTON_LEFT,
		                   .keyUse           = KEY_E,
		                   .keyJump          = KEY_SPACE,
		                   .keyWeaponOne     = KEY_ONE,
		                   .keyWeaponTwo     = KEY_TWO,
		                   .keyWeaponThree   = KEY_THREE,
		                   .keyWeaponFour    = KEY_FOUR,
		                   .keyWeaponFive    = KEY_FIVE };
	// clang format on
	// Write settings to datafile here

	return data;
}

void Settings_Read(Settings_Data *settings)
{
	const int bufferLength = Utilities_GetFileCharacterCount(SETTINGS_FILENAME);
	FILE *filePointer      = fopen(SETTINGS_FILENAME, "r");
	if (nullptr == filePointer)
	{
		printf("Failed to open settings file %s \n", SETTINGS_FILENAME);
		printf("Using default settings! \n");
		return;
	}

	printf("Loaded following settings \n");

	char buffer[bufferLength];
	while (fgets(buffer, bufferLength, filePointer))
	{
		char key[bufferLength];
		char value[bufferLength];
		Utilities_ParseKeyValuePair(buffer, key, "=", value);

		if (!Settings_Parse(settings, key, (float)atof(value)))
		{
			printf("Failed to parse settings file key-value: %s - %f \n", key, atof(value));
		}
		else
		{
			printf("Parsed settings file key-value: %s - %f \n", key, atof(value));
		}
	}

	fclose(filePointer);
}

void Settings_Write(Settings_Data *settings)
{

	FILE *filePointer = fopen(SETTINGS_FILENAME, "w");
	if (filePointer == nullptr)
	{
		printf("Failed to open settings file %s \n", SETTINGS_FILENAME);
		return;
	}

	fprintf(filePointer, "screenWidth=%d\n", settings->screenWidth);
	fprintf(filePointer, "screenHeight=%d\n", settings->screenHeight);
	fprintf(filePointer, "mouseSensitivity=%.2f\n", settings->mouseSensitivity);
	fprintf(filePointer, "cameraFOV=%d\n", settings->cameraFOV);
	fprintf(filePointer, "maxFPS=%d\n", settings->maxFPS);
	fprintf(filePointer, "keyMoveForward=%d\n", settings->keyMoveForward);
	fprintf(filePointer, "keyMoveBackward=%d\n", settings->keyMoveBackward);
	fprintf(filePointer, "keyMoveLeft=%d\n", settings->keyMoveLeft);
	fprintf(filePointer, "keyMoveRight=%d\n", settings->keyMoveRight);
	fprintf(filePointer, "keyFire=%d\n", settings->keyFire);
	fprintf(filePointer, "keyUse=%d\n", settings->keyUse);
	fprintf(filePointer, "keyJump=%d\n", settings->keyJump);
	fprintf(filePointer, "keyWeaponOne=%d\n", settings->keyWeaponOne);
	fprintf(filePointer, "keyWeaponTwo=%d\n", settings->keyWeaponTwo);
	fprintf(filePointer, "keyWeaponThree=%d\n", settings->keyWeaponThree);
	fprintf(filePointer, "keyWeaponFour=%d\n", settings->keyWeaponFour);
	fprintf(filePointer, "keyWeaponFive=%d\n", settings->keyWeaponFive);

	fclose(filePointer);
}

bool Settings_Parse(Settings_Data *settings, char *key, float value)
{
	// Excuse the ugly else if ladder
	if (strcmp(key, "screenWidth") == 0)
	{
		settings->screenWidth = (int)value;
		return true;
	}
	else if (strcmp(key, "screenHeight") == 0)
	{
		settings->screenHeight = (int)value;
		return true;
	}
	else if (strcmp(key, "mouseSensitivity") == 0)
	{
		settings->mouseSensitivity = value;
		return true;
	}
	else if (strcmp(key, "cameraFOV") == 0)
	{
		settings->cameraFOV = (int)value;
		return true;
	}
	else if (strcmp(key, "maxFPS") == 0)
	{
		settings->maxFPS = (int)value;
		return true;
	}
	else if (strcmp(key, "keyMoveForward") == 0)
	{
		settings->keyMoveForward = (int)value;
		return true;
	}
	else if (strcmp(key, "keyMoveBackward") == 0)
	{
		settings->keyMoveBackward = (int)value;
		return true;
	}
	else if (strcmp(key, "keyMoveLeft") == 0)
	{
		settings->keyMoveLeft = (int)value;
		return true;
	}
	else if (strcmp(key, "keyMoveRight") == 0)
	{
		settings->keyMoveRight = (int)value;
		return true;
	}
	else if (strcmp(key, "keyFire") == 0)
	{
		settings->keyFire = (int)value;
		return true;
	}
	else if (strcmp(key, "keyUse") == 0)
	{
		settings->keyUse = (int)value;
		return true;
	}
	else if (strcmp(key, "keyJump") == 0)
	{
		settings->keyJump = (int)value;
		return true;
	}
	else if (strcmp(key, "keyWeaponOne") == 0)
	{
		settings->keyWeaponOne = (int)value;
		return true;
	}
	else if (strcmp(key, "keyWeaponTwo") == 0)
	{
		settings->keyWeaponTwo = (int)value;
		return true;
	}
	else if (strcmp(key, "keyWeaponThree") == 0)
	{
		settings->keyWeaponThree = (int)value;
		return true;
	}
	else if (strcmp(key, "keyWeaponFour") == 0)
	{
		settings->keyWeaponFour = (int)value;
		return true;
	}
	else if (strcmp(key, "keyWeaponFive") == 0)
	{
		settings->keyWeaponFive = (int)value;
		return true;
	}
	else
	{
		printf("Failed to parse settings file!\n");
		;
		return false;
	}
}
