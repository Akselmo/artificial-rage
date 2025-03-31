package entity

import "core:fmt"
import rl "vendor:raylib"

ItemAmmo :: struct {
	ammoMorph:  WeaponType,
	ammoAmount: i32,
}

CreateItemAmmo :: proc(entity: ^Entity, morph: WeaponType) {
	item := ItemAmmo{}

	switch (morph) {
	case WeaponType.PISTOL:
		entity.visuals.texture = TextureItemAmmoPistol^
		item.ammoAmount = 10
	case WeaponType.RIFLE:
		entity.visuals.texture = TextureItemAmmoRifle^
		item.ammoAmount = 20
	case WeaponType.SHOTGUN:
		entity.visuals.texture = TextureItemAmmoShotgun^
		item.ammoAmount = 5
	case WeaponType.RAILGUN:
		entity.visuals.texture = TextureItemAmmoRailgun^
		item.ammoAmount = 1
	case WeaponType.MELEE:
		fmt.printfln("How did you pick up ammo for fists????")
	}

	item.ammoMorph = morph

	entity.transform.size = rl.Vector3{1.0, 1.0, 0.01}
	entity.transform.scale = 0.25

	InitializeItem(entity, true, 0.15)
	entity.type = item
	entity.active = true
}

