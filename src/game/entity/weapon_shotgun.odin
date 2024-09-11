package entity

import rl "vendor:raylib"
import "src:game/settings"

WeaponShotgun : WeaponData = {
    name                 = "Shotgun",
    inputKey             = settings.keyWeaponFour,
    weaponId             = WeaponID.SHOTGUN,
    damage               = 7,
    ammo                 = 40, // TODO: Set to 0 for release!
    fireRate             = 1.5,
    range                = 6.0,
    pickedUp             = false,
    maxAmmo              = WEAPON_SHOTGUN_AMMO_MAX,
    hitscan              = false,
    spriteSpeed          = 10,
    spriteFireFrame      = 2,
    projectileSize       = rl.Vector3{ 0.3, 0.05, 0.05 },
    projectileColor      = rl.RED,
    spritesTotal         = 5,
    spritePositionOffset = rl.Vector2{ -0.6, 1.0 }
}
