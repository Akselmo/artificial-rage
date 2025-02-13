package entity

import rl "vendor:raylib"

// Any specific styles of items are called morphs
ItemHealthMorph :: enum {
	SMALL,
	MEDIUM,
	LARGE,
}

ItemHealth :: struct {
	healAmount: i32,
}

CreateItemHealth :: proc(entity: ^Entity, morph: ItemHealthMorph) {
	item := ItemHealth{}

	switch (morph) {
	case ItemHealthMorph.SMALL:
		entity.visuals.textureFilename = "./assets/textures/health_small.png"
		item.healAmount = 5
	case ItemHealthMorph.MEDIUM:
		entity.visuals.textureFilename = "./assets/textures/health_medium.png"
		item.healAmount = 10
	case ItemHealthMorph.LARGE:
		entity.visuals.textureFilename = "./assets/textures/health_large.png"
		item.healAmount = 15
	}

	entity.transform.size = rl.Vector3{1.0, 1.0, 0.01}
	entity.transform.scale = 0.25

	InitializeItem(entity, true, 0.15)
	entity.type = item
	entity.active = true
}

