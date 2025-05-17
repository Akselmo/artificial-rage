package entity

import "src:game/settings"
import rl "vendor:raylib"

WeaponRailgun: WeaponData = {
	name                 = "Railgun",
	inputKey             = settings.keyWeaponFive,
	weaponId             = WeaponType.RAILGUN,
	damage               = 30,
	ammo                 = 10, // TODO: Set to 0 for release!
	fireRate             = 2.6,
	range                = 69,
	pickedUp             = false,
	maxAmmo              = WEAPON_RAILGUN_AMMO_MAX,
	hitscan              = false,
	spriteSpeed          = 10,
	spriteFireFrame      = 1,
	projectileSize       = rl.Vector3{0.05, 0.05, 0.5},
	projectileColor      = rl.YELLOW,
	spritesTotal         = 5,
	spritePositionOffset = rl.Vector2{0.5, 0.1},
}

