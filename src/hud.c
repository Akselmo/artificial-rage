#include "hud.h"
#include "weapon.h"

char Hud_dedText[5]         = "DEAD";
char Hud_emptyAmmoText[2]   = "-";
char Hud_healthText[5]      = "100";
char Hud_weaponAmmoText[10] = "100";

char Hud_weaponNameText[15] = "WeaponName";

void Hud_Draw()
{
    DrawFPS(10, 10);
    sprintf(Hud_healthText, "%d", Player->health);

    char ammo[3];
    char divider[2];
    char maxAmmo[3];

    sprintf(ammo, "%d", WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->ammo);
    sprintf(maxAmmo, "%d", WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->maxAmmo);

    strcpy(Hud_weaponAmmoText, ammo);
    strcat(Hud_weaponAmmoText, "/");
    strcat(Hud_weaponAmmoText, maxAmmo);

    if(Player->health <= 0)
    {
        strcpy(Hud_healthText, Hud_dedText);
    }
    if(WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->ammo)
    {
        if(WeaponDataHolder.currentWeapon == (Weapon_ID)FIST)
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
