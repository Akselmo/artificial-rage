#pragma once
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "raylib.h"

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

Settings_Data Settings_Read(void);
void Settings_Write(Settings_Data* settings);
Settings_Data Settings_CreateDefault(void);

#endif
