#pragma once
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "raylib.h"

// Public variables
extern float Settings_MouseSensitivity;
extern float Settings_CameraFov;

// Public functions
void Settings_Initialize(void);
int Settings_GetCustomInput(int key);

#endif
