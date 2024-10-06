package entity

import rl "vendor:raylib"

ItemHealthSmall :: struct {
	healAmount: i32,
}

CreateItemHealthSmall :: proc(entity: ^Entity) {
	entity.visuals.textureFilename = "./assets/textures/health_small.png"
	entity.transform.size = rl.Vector3{1.0, 1.0, 0.01}
	entity.transform.scale = 0.25

	InitializeItem(entity, true, 0.15)
	entity.type = ItemHealthSmall {
		healAmount = 5,
	}
	entity.active = true
}

