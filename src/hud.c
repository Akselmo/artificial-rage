#include "hud.h"
#include "weapon.h"

void Hud_Draw(void)
{
    const char Hud_dedText[6]       = "DEAD";
    const char Hud_emptyAmmoText[4] = "-";
    char Hud_healthText[6]          = "";
    char Hud_weaponAmmoText[12]     = "";
    char Hud_weaponNameText[16]     = "WeaponName";  // TODO: add weapon name to hud

    DrawFPS(10, 10);
    sprintf(Hud_healthText, "%d", Player->health);

    char ammo[4];
    char divider[3];
    char maxAmmo[4];

    sprintf(ammo, "%d", WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo);
    sprintf(maxAmmo, "%d", WeaponHolder.Weapons[WeaponHolder.currentWeapon]->maxAmmo);

    strcpy(Hud_weaponAmmoText, ammo);
    strcat(Hud_weaponAmmoText, "/");
    strcat(Hud_weaponAmmoText, maxAmmo);

    if(Player->health <= 0)
    {
        strcpy(Hud_healthText, Hud_dedText);
    }
    if(WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo)
    {
        if(WeaponHolder.currentWeapon == (Weapon_ID)MELEE)
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
    Weapon_DrawSprite();
}
