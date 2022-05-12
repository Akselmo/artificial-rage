#include "include/raylib.h"
#include "include/raymath.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

char dedText[5]         = "DEAD";
char emptyAmmoText[2]   = "-";
char healthText[5]      = "100";
char weaponAmmoText[5]  = "123";
char weaponNameText[15] = "WeaponName";

void DrawPlayerHud(int health, int weaponAmmo, int weapon)
{
    DrawFPS(10, 10);
    sprintf(healthText, "%d", health);
    sprintf(weaponAmmoText, "%d", weaponAmmo);
    if(health <= 0)
    {
        strcpy(healthText, dedText);
    }
    if(weaponAmmo)
    {
        strcpy(weaponAmmoText, emptyAmmoText);
    }
    DrawText(healthText, 50, GetScreenHeight() - 20, 20, RED);
    DrawText(weaponAmmoText, GetScreenWidth() - 50, GetScreenHeight() - 20, 20, RED);
}