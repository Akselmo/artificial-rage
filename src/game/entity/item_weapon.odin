package entity

import "core:fmt"
import rl "vendor:raylib"

ItemWeapon :: struct {
	weaponType: WeaponType,
	ammoAmount: i32,
}

CreateItemWeaponPickup :: proc(entity: ^Entity, morph: WeaponType) {
	item := ItemWeapon{}

	switch (morph) {
	case WeaponType.PISTOL:
		entity.visuals.texture = TextureItemPickupPistol^
		item.ammoAmount = 5
	case WeaponType.RIFLE:
		entity.visuals.texture = TextureItemPickupRifle^
		item.ammoAmount = 15
	case WeaponType.SHOTGUN:
		entity.visuals.texture = TextureItemPickupShotgun^
		item.ammoAmount = 2
	case WeaponType.RAILGUN:
		entity.visuals.texture = TextureItemPickupRailgun^
		item.ammoAmount = 1
	case WeaponType.MELEE:
		fmt.printfln("How did you pick up fists from the ground????")
	}

	item.weaponType = morph

	entity.transform.size = rl.Vector3{1.0, 1.0, 0.01}
	entity.transform.scale = 0.25

	InitializeItem(entity, true, 0.15)
	entity.type = item
	entity.active = true
}

