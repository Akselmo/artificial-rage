#include "settings.h"

// TODO: List all input keys
// c: int Settings_KeyForward
// h: extern int Settings_KeyForward
// And so on
struct Settings_Data Settings;

void Settings_Initialize(void)
{
    Settings = Settings_Read();

    InitWindow(Settings.screenWidth, Settings.screenHeight, "Artificial Rage");

    SetTargetFPS(Settings.maxFPS);
}

Settings_Data Settings_CreateDefault(void)
{
    // clang-format off
    Settings_Data data = {
        .screenWidth        = 800,
        .screenHeight       = 600,
        .mouseSensitivity   = 0.25f,
        .cameraFOV          = 90,
        .maxFPS             = 200,
        .keyMoveForward     = KEY_W,
        .keyMoveBackward    = KEY_S,
        .keyMoveLeft        = KEY_A,
        .keyMoveRight       = KEY_D,
        .keyFire            = MOUSE_BUTTON_LEFT,
        .keyUse             = KEY_E,
        .keyJump            = KEY_SPACE,
        .keyWeaponOne       = KEY_ONE,
        .keyWeaponTwo       = KEY_TWO,
        .keyWeaponThree     = KEY_THREE,
        .keyWeaponFour      = KEY_FOUR,
        .keyWeaponFive      = KEY_FIVE
    };
    // clang format on
    // Write settings to datafile here

    return data;
}

Settings_Data Settings_Read(void)
{
    // if file not found, create defaults, else read from file
    return Settings_CreateDefault();
}

void Settings_Write(Settings_Data* settings)
{
}
