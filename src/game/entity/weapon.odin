package entity

// Weapon, while not strictly an entity, is best handled as such
// since they're used to interact with entities

import "core:fmt"
import "core:math"
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
	weaponId:             WeaponID,
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
	spriteSpeed:          f32,
	spriteFireFrame:      i32,
	spritePositionOffset: rl.Vector2,
	projectileSize:       rl.Vector3,
	projectileColor:      rl.Color,
}

WeaponCurrent: WeaponID = WeaponID.MELEE
// TODO: Could be a map?
Weapons: [WEAPON_AMOUNT]WeaponData
WeaponFrameCounter: i32 = 0
WeaponCurrentFrame: i32 = 0
WeaponActive: bool = false

WeaponInitialize :: proc() {
	// Initialize base weapon items
	Weapons[WeaponID.MELEE] = WeaponMelee
	Weapons[WeaponID.PISTOL] = WeaponPistol
	Weapons[WeaponID.RIFLE] = WeaponRifle
	Weapons[WeaponID.SHOTGUN] = WeaponShotgun
	Weapons[WeaponID.RAILGUN] = WeaponRailgun

	// Initialize sprites
	Weapons[WeaponID.MELEE].spriteTexture = rl.LoadTexture("./assets/weapons/melee.png")
	Weapons[WeaponID.PISTOL].spriteTexture = rl.LoadTexture("./assets/weapons/pistol.png")
	Weapons[WeaponID.RIFLE].spriteTexture = rl.LoadTexture("./assets/weapons/rifle.png")
	Weapons[WeaponID.SHOTGUN].spriteTexture = rl.LoadTexture("./assets/weapons/shotgun.png")
	Weapons[WeaponID.RAILGUN].spriteTexture = rl.LoadTexture("./assets/weapons/railgun.png")
}

WeaponSelectDefault :: proc() {
	WeaponCurrent = WeaponID.MELEE
}

WeaponGetSwitchInput :: proc() {
	key := rl.GetKeyPressed()
	if (key == rl.KeyboardKey.KEY_NULL) {
		return
	}
	for i: i32 = 0; i < WEAPON_AMOUNT; i += 1 {
		if (cast(i32)key == Weapons[i].inputKey) {
			WeaponChange(Weapons[i].weaponId)
			return
		}
	}
}

WeaponChange :: proc(weaponId: WeaponID) {
	if (!WeaponActive) {
		WeaponCurrent = weaponId
	}
}

WeaponHasAmmo :: proc(weaponId: WeaponID) -> bool {

	// Fists always have ammo :D
	if (weaponId == WeaponID.MELEE) {
		return true
	}
	wpn := Weapons[weaponId]
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
		wpn := &Weapons[WeaponCurrent]
		if (WeaponHasAmmo(wpn.weaponId) && !WeaponActive) {
			WeaponActive = true
			WeaponCurrentFrame = wpn.spriteFireFrame

			rayCast := rl.GetMouseRay(utilities.GetScreenCenter(), camera^)
			if (wpn.hitscan) {
				// hitscan
				entityHit := RaycastHitsEntity(rayCast)
				distanceFromPlayer := rl.Vector3Distance(Player.transform.position, entityHit.transform.position)
				if (distanceFromPlayer < 1.0) {
					#partial switch _ in entityHit.type {
					case Enemy:
						EnemyTakeDamage(entityHit, wpn.damage)
					}
				}
			} else {
				// projectile
				// Move raycast start position a bit further from player if firing a projectile
				rayCast.position = rayCast.position + (rayCast.direction * 0.1)
				ProjectileCreate(rayCast, wpn.projectileSize, wpn.damage, PLAYER_ID, wpn.projectileColor)
			}
			if (wpn.weaponId != WeaponID.MELEE) {
				wpn.ammo -= 1
			}
		}
		nextFireTime := wpn.fireRate
	}
	return nextFireTime

}

WeaponDrawSprite :: proc() {
	wpn := Weapons[WeaponCurrent]

	frameWidth: f32 = cast(f32)wpn.spriteTexture.width / cast(f32)wpn.spritesTotal
	frameHeight: f32 = cast(f32)wpn.spriteTexture.height
	origin: rl.Vector2 = {frameWidth / 2.0, frameHeight}

	scale: f32 = math.min(frameWidth * 2.0 / frameWidth, frameHeight * 2.0 / frameHeight)
	posX: f32 = utilities.GetScreenCenter().x - (frameWidth * wpn.spritePositionOffset.x)
	posY: f32 = cast(f32)rl.GetScreenHeight() - (frameHeight * wpn.spritePositionOffset.y)

	sourceRec: rl.Rectangle = {0.0, 0.0, frameWidth, frameHeight}
	destRec: rl.Rectangle = {posX, posY, frameWidth * scale, frameHeight * scale}

	if (WeaponActive) {
		WeaponFrameCounter += 1
		if (cast(f32)WeaponFrameCounter >= cast(f32)rl.GetFPS() / (wpn.spriteSpeed / wpn.fireRate)) {
			WeaponCurrentFrame += 1

			if (WeaponCurrentFrame >= wpn.spritesTotal) {
				WeaponCurrentFrame = 0
				WeaponActive = false
			}
			WeaponFrameCounter = 0
		}
	}

	sourceRec.x = frameWidth * cast(f32)WeaponCurrentFrame

	rl.DrawTexturePro(wpn.spriteTexture, sourceRec, destRec, origin, 0, rl.WHITE)
}

