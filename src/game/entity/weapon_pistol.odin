package entity

import "src:game/settings"
import rl "vendor:raylib"

WeaponPistol: WeaponData = {
	name                 = "Pistol",
	inputKey             = settings.keyWeaponTwo,
	weaponId             = WeaponType.PISTOL,
	damage               = 3,
	ammo                 = 30,
	fireRate             = 1.0,
	range                = 8.0,
	pickedUp             = true, // You also always have your trusty pistol with you
	maxAmmo              = WEAPON_PISTOL_AMMO_MAX,
	hitscan              = false,
	spriteSpeed          = 10,
	spriteFireFrame      = 2,
	projectileSize       = rl.Vector3{0.05, 0.05, 0.2},
	projectileColor      = rl.GREEN,
	spritesTotal         = 5,
	spritePositionOffset = rl.Vector2{0.2, 0.17},
}

