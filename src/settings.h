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
        int maxFPS;  // Add custom inputs as well
} Settings_Data;

// Public variables
extern struct Settings_Data Settings;

// Public functions
void Settings_Initialize(void);
int Settings_GetCustomInput(int key);

Settings_Data Settings_Read(void);
void Settings_Write(Settings_Data* settings);
Settings_Data Settings_CreateDefault(void);

#endif
