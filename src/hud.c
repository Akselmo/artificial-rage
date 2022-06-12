#include "hud.h"

char Hud_dedText[5]         = "DEAD";
char Hud_emptyAmmoText[2]   = "-";
char Hud_healthText[5]      = "100";
char Hud_weaponAmmoText[10] = "100";

char Hud_weaponNameText[15] = "WeaponName";

void Hud_Draw(int health, int weaponAmmo, int weaponMaxAmmo, int weapon)
{
    DrawFPS(10, 10);
    sprintf(Hud_healthText, "%d", health);

    char ammo[3];
    char divider[2];
    char maxAmmo[3];

    sprintf(ammo, "%d", weaponAmmo);
    sprintf(maxAmmo, "%d", weaponMaxAmmo);

    strcpy(Hud_weaponAmmoText, ammo);
    strcat(Hud_weaponAmmoText, "/");
    strcat(Hud_weaponAmmoText, maxAmmo);

    if(health <= 0)
    {
        strcpy(Hud_healthText, Hud_dedText);
    }
    if(weaponAmmo)
    {
        if(weapon == (Weapon_ID)FIST)
        {
            strcpy(Hud_weaponAmmoText, Hud_emptyAmmoText);
        }
        else
        {
            strcpy(Hud_weaponAmmoText, Hud_weaponAmmoText);
        }
    }
    DrawText(Hud_healthText, 50, GetScreenHeight() - 20, 20, RED);
    DrawText(Hud_weaponAmmoText, GetScreenWidth() - 70, GetScreenHeight() - 20, 20, RED);
}