package entity

import rl "vendor:raylib"
import "src:game/settings"

WeaponMelee : WeaponData = {
    name                 = "Fists",
    inputKey             = settings.keyWeaponOne,
    weaponId             = WeaponID.MELEE,
    damage               = 5,
    ammo                 = WEAPON_MELEE_AMMO_MAX, // Unlimited ammo for your fists!
    fireRate             = 1.25,
    range                = 2.0,
    pickedUp             = true, // You always have your fists with you
    maxAmmo              = WEAPON_MELEE_AMMO_MAX,
    hitscan              = true,
    spriteSpeed          = 10,
    spriteFireFrame      = 1,
    projectileSize       = rl.Vector3{ 0, 0, 0 },
    projectileColor      = rl.WHITE,
    spritesTotal         = 7,
    spritePositionOffset = rl.Vector2{ 0.5, 1.0 }
};
