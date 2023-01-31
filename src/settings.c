#include "settings.h"

// TODO: List all input keys
// c: int Settings_KeyForward
// h: extern int Settings_KeyForward
// And so on
struct Settings_Data Settings;

// Private
void Settings_Read(Settings_Data* settings);
void Settings_Write(Settings_Data* settings);
Settings_Data Settings_CreateDefault(void);
bool Settings_Parse(Settings_Data* settings, char* key, double value);

void Settings_Initialize(void)
{
    const char* fileName = "settings.txt";

    // Write default settings
    Settings = Settings_CreateDefault();

    if(access(fileName, F_OK) == 0)
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

void Settings_Read(Settings_Data* settings)
{
    const char *fileName = "settings.txt";

    FILE *filePointer = fopen(fileName, "r");
    if (NULL == filePointer) {
        printf("======\n");
        printf("Failed to open settings file %s \n", fileName);
        printf("Using default settings! \n");
        printf("======\n");
        return;
    }

    printf("======\n");
    printf("Loaded following settings \n");

    int bufferLength = 255;
    char buffer[bufferLength];

    while(fgets(buffer, bufferLength, filePointer))
    {
        char *token = strtok(buffer, " ");
        char *key;
        char *value;
        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                key = token;
            }
            else if (i == 1)
            {
                value = token;
            }
            token = strtok(NULL, " ");
        }

    //TODO: Switch case that reads all the k/v pairs and assigns them to correct settings

        if (!Settings_Parse(&Settings, key, atof(value)))
        {
            printf("Failed to parse setting key-value: %s - %f \n", key, atof(value));
        }
        else
        {
            printf("Parsed setting key-value: %s - %f \n", key, atof(value));
        }
    }

    fclose(filePointer);

    printf("======\n");

}

void Settings_Write(Settings_Data* settings)
{
    const char *fileName = "settings.txt";

    FILE *filePointer = fopen(fileName, "w");
    if (filePointer == NULL)
    {
        printf("======\n");
        printf("Failed to open settings file %s \n", fileName);
        printf("======\n");
    }

    fprintf(filePointer, "screenWidth %d \n", settings->screenWidth);
    fprintf(filePointer, "screenHeight %d \n", settings->screenHeight);

    fclose(filePointer);
}

bool Settings_Parse(Settings_Data* settings, char* key, double value)
{
    if (strcmp(key, "screenWidth") == 0)
    {
        settings->screenWidth = value;
        return true;
    }
    else if (strcmp(key, "screenHeight") == 0)
    {
        settings->screenHeight = value;
        return true;
    }
    else
    {
        printf("======\n");
        printf("Failed to parse settings file!\n");
        printf("======\n");
        return false;
    }
}
