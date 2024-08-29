package weapon

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

Data :: struct {
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

CurrentWeapon: i32 = 0
// TODO: Could be a map?
Weapons: [WEAPON_AMOUNT]Data
FrameCounter: i32 = 0
CurrentFrame: i32 = 0
Active: bool = false

Initialize :: proc() {
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

SelectDefault :: proc() {
	CurrentWeapon = Weapons[WeaponID.MELEE].weaponId
}

GetSwitchInput :: proc() {
	key := rl.GetKeyPressed()
	for i := 0; i < WEAPON_AMOUNT; i += 1 {
		if (key == Weapons[i].inputKey) {
			Change(Weapons[i].weaponId)
		}
	}
}

Change :: proc(weaponId: i32) {
	if (Active) {
		CurrentWeapon = weaponId
	}
}

// TODO: TestEntityHit? I think this goes to entity package: GetHitEntityId
// Although that causes cyclical deps so idk
// Hitscan detection could be done in entity package, just as damaging is
// Then we just ask the weapon pckg if its hitscan and the damage it does
// For projectiles, we just check in entity if we collide with a projectile,
// then get the damage value from projectile

HasAmmo :: proc(weaponId: i32) -> bool {

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

Fire :: proc(camera: ^rl.Camera, nextFire: f32) -> f32 {

	if (nextFire > 0.0) {
		nextFire -= rl.GetFrameTime()
	} else {
		wpn := Weapons[CurrentWeapon]
		if (HasAmmo(wpn.weaponId) && !Active) {
			Active = true
			CurrentFrame = wpn.spriteFrame
			rayCast := rl.GetMouseRay(utilities.GetScreenCenter(), &camera)
			//TODO rest of this, needs GetHitEntityId
		}
	}
	return nextFire

}

