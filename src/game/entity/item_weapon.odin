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
		entity.visuals.textureFilename = "./assets/textures/pistol.png"
		item.ammoAmount = 5
	case WeaponType.RIFLE:
		entity.visuals.textureFilename = "./assets/textures/rifle.png"
		item.ammoAmount = 15
	case WeaponType.SHOTGUN:
		entity.visuals.textureFilename = "./assets/textures/shotgun.png"
		item.ammoAmount = 2
	case WeaponType.RAILGUN:
		entity.visuals.textureFilename = "./assets/textures/railgun.png"
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

