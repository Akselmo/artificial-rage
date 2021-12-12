#include "player.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include <stdio.h>

void DrawPlayerHud(int health, int weaponAmmo, int weapon)
{
    DrawFPS(10, 10);
    char healthText[3];
    sprintf(healthText, "%d", health);
    DrawText(healthText,20,20, 14, RED);

}

