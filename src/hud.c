#include "hud.h"

void Hud_HealthText(char *text);
void Hud_WeaponText(char *text);

void Hud_Draw(void)
{
	DrawFPS(10, 10);

	// Draw weapons first
	Weapon_DrawSprite();

	// Draw health text
	if (Player->health <= 0)
	{
		Hud_HealthText("DEAD");
	}
	else
	{
		char *Hud_healthText = calloc(50, sizeof(char));
		snprintf(Hud_healthText, 5, "%d", Player->health);
		Hud_HealthText(Hud_healthText);
		free(Hud_healthText);
	}

	// Draw weapon ammo text
	if (WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo)
	{
		if (WeaponHolder.currentWeapon == (Weapon_ID)MELEE)
		{
			Hud_WeaponText("-");
		}
		else
		{
			char *Hud_weaponAmmoText = calloc(50, sizeof(char));
			snprintf(
				Hud_weaponAmmoText,
				50,
				"%d / %d",
				WeaponHolder.Weapons[WeaponHolder.currentWeapon]->ammo,
				WeaponHolder.Weapons[WeaponHolder.currentWeapon]->maxAmmo
			);
			Hud_WeaponText(Hud_weaponAmmoText);
			free(Hud_weaponAmmoText);
		}
	}
}

void Hud_HealthText(char *text) { DrawText(text, 50, GetScreenHeight() - 20, 20, RED); }

void Hud_WeaponText(char *text) { DrawText(text, GetScreenWidth() - 100, GetScreenHeight() - 20, 20, RED); }
