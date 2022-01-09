#include "player.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include <stdio.h>
#include <string.h>

char dedText[5] = "DEAD";
char healthText[5] = "100";

void DrawPlayerHud(int health, int weaponAmmo, int weapon)
{
    DrawFPS(10, 10);
    sprintf(healthText, "%d", health);
    if (health <= 0)
    {
        strcpy(healthText,dedText);
    }
    DrawText(healthText,10,GetScreenHeight()-20, 20, RED);
}

