package entity

import rl "vendor:raylib"

// just one morph for now
ItemKeyMorph :: enum {
	TELEPORT,
}

ItemKey :: struct {
	keyMorph: ItemKeyMorph,
}

CreateItemKey :: proc(entity: ^Entity, morph: ItemKeyMorph) {
	item := ItemKey{}

	switch (morph) {
	case ItemKeyMorph.TELEPORT:
		entity.visuals.texture = TextureItemPickupKey^
	}

	item.keyMorph = morph
	entity.transform.size = rl.Vector3{1.0, 1.0, 0.01}
	entity.transform.scale = 0.25

	InitializeItem(entity, true, 0.15)
	entity.type = item
	entity.active = true
}

