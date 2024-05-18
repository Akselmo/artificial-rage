#include "hud.h"
#include "player.h"
#include "raylib.h"
#include "weapon.h"
#include <stdio.h>
#include <stdlib.h>

Hud::Hud() {}

void Hud::draw(void)
{
	DrawFPS(10, 10);

	// Draw weapons first
	Weapon_DrawSprite();

	// Draw health text
	if (Player->health <= 0)
	{
		Hud::healthText("DEAD");
	}
	else
	{
		char *healthText = (char *)calloc(50, sizeof(char));
		snprintf(healthText, 5, "%d", Player->health);
		Hud::healthText(healthText);
		free(healthText);
	}

	// Draw weapon ammo text
	if (WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo)
	{
		if (WeaponHolder.currentWeapon == (Weapon_ID)MELEE)
		{
			Hud::weaponText("-");
		}
		else
		{
			char *weaponAmmoText = (char *)calloc(50, sizeof(char));
			snprintf(
				weaponAmmoText,
				50,
				"%d / %d",
				WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo,
				WeaponHolder.Weapons[WeaponHolder.currentWeapon]->maxAmmo
			);
			Hud::weaponText(weaponAmmoText);
			free(weaponAmmoText);
		}
	}
}

void Hud::healthText(char *text) { DrawText(text, 50, GetScreenHeight() - 20, 20, RED); }

void Hud::weaponText(char *text) { DrawText(text, GetScreenWidth() - 100, GetScreenHeight() - 20, 20, RED); }
