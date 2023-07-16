#pragma once
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "raylib.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Settings_Data
{
	int screenWidth;
	int screenHeight;
	float mouseSensitivity;
	int cameraFOV;
	int maxFPS;
	int keyMoveForward;
	int keyMoveBackward;
	int keyMoveRight;
	int keyMoveLeft;
	int keyFire;
	int keyUse;
	int keyJump;
	int keyWeaponOne;
	int keyWeaponTwo;
	int keyWeaponThree;
	int keyWeaponFour;
	int keyWeaponFive;
} Settings_Data;

// Public variables
extern struct Settings_Data Settings;

// Public functions
void Settings_Initialize(void);

#endif
