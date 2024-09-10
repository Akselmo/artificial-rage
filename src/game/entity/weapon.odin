package entity

// Weapon, while not strictly an entity, is best handled as such
// since they're used to interact with entities

import "core:fmt"
import "src:game/settings"
import "src:game/utilities"
import rl "vendor:raylib"

WEAPON_AMOUNT: i32 : 5
WEAPON_MELEE_AMMO_MAX: i32 : 10
WEAPON_PISTOL_AMMO_MAX: i32 : 100
WEAPON_RIFLE_AMMO_MAX: i32 : 200
WEAPON_SHOTGUN_AMMO_MAX: i32 : 50
WEAPON_RAILGUN_AMMO_MAX: i32 : 25

WeaponID :: enum {
	MELEE   = 0,
	PISTOL  = 1,
	RIFLE   = 2,
	SHOTGUN = 3,
	RAILGUN = 4,
}

WeaponData :: struct {
	name:                 string,
	weaponId:             i32,
	inputKey:             i32,
	damage:               i32,
	ammo:                 i32,
	maxAmmo:              i32,
	fireRate:             f32,
	range:                f32,
	pickedUp:             bool,
	hitscan:              bool,
	spriteTexture:        rl.Texture2D,
	spritesTotal:         i32,
	spriteSpeed:          i32,
	spriteFireFrame:      i32,
	spritePositionOffset: rl.Vector2,
	projectileSize:       rl.Vector3,
	projectileColor:      rl.Color,
}

WeaponCurrent: i32 = 0
// TODO: Could be a map?
Weapons: [WEAPON_AMOUNT]WeaponData
WeaponFrameCounter: i32 = 0
WeaponCurrentFrame: i32 = 0
Active: bool = false

WeaponInitialize :: proc() {
	// Initialize keys
	Weapons[WeaponID.MELEE].inputKey = settings.keyWeaponOne
	Weapons[WeaponID.PISTOL].inputKey = settings.keyWeaponTwo
	Weapons[WeaponID.RIFLE].inputKey = settings.keyWeaponThree
	Weapons[WeaponID.SHOTGUN].inputKey = settings.keyWeaponFour
	Weapons[WeaponID.RAILGUN].inputKey = settings.keyWeaponFive

	// Initialize sprites
	Weapons[WeaponID.MELEE].spriteTexture = rl.LoadTexture("./assets/weapons/melee.png")
	Weapons[WeaponID.PISTOL].spriteTexture = rl.LoadTexture("./assets/weapons/pistol.png")
	Weapons[WeaponID.RIFLE].spriteTexture = rl.LoadTexture("./assets/weapons/rifle.png")
	Weapons[WeaponID.SHOTGUN].spriteTexture = rl.LoadTexture("./assets/weapons/shotgun.png")
	Weapons[WeaponID.RAILGUN].spriteTexture = rl.LoadTexture("./assets/weapons/railgun.png")
}

WeaponSelectDefault :: proc() {
	WeaponCurrent = Weapons[WeaponID.MELEE].weaponId
}

WeaponGetSwitchInput :: proc() {
	key := rl.GetKeyPressed()
	for i: i32 = 0; i < WEAPON_AMOUNT; i += 1 {
		if (cast(i32)key == Weapons[i].inputKey) {
			WeaponChange(Weapons[i].weaponId)
		}
	}
}

WeaponChange :: proc(weaponId: i32) {
	if (Active) {
		WeaponCurrent = weaponId
	}
}

WeaponHasAmmo :: proc(weaponId: WeaponID) -> bool {

	// Fists always have ammo :D
	if (weaponId == WeaponID.MELEE) {
		return true
	}
	wpn := Weapons[weaponId]
	fmt.printfln("Ammo: %[0]v", wpn.ammo)
	if (wpn.ammo > 0) {
		return true
	}
	return false
}


WeaponFire :: proc(oldFireTime: f32, camera: ^rl.Camera) -> f32 {

	nextFireTime := oldFireTime
	if (nextFireTime > 0.0) {
		nextFireTime -= rl.GetFrameTime()
	} else {
		wpn := Weapons[WeaponCurrent]
		if (WeaponHasAmmo(cast(WeaponID)wpn.weaponId) && !Active) {
			Active = true
			WeaponCurrentFrame = wpn.spriteFireFrame

			rayCast := rl.GetMouseRay(utilities.GetScreenCenter(), camera^)
			entityHitId := RaycastHitsEntityId(rayCast)
			if (wpn.hitscan) {
				// hitscan
				entity := entitiesInScene[entityHitId]
				if (entity.id != 0 && entity.id != PLAYER_ID && entity.data.type == Type.ENEMY_DEFAULT) {
					TakeDamage(&entity, wpn.damage)
				}
			} else {
				// projectile
				// Move raycast start position a bit further from player if firing a projectile
				rayCast.position = rl.Vector3Add(rayCast.position, rl.Vector3Scale(rayCast.direction, 0.1))
				ProjectileCreate(rayCast, wpn.projectileSize, wpn.damage, PLAYER_ID, wpn.projectileColor)
			}
		}
		nextFireTime := wpn.fireRate
	}
	return nextFireTime

}

