package entity


PlayerEnd :: struct {}


CreatePlayerEnd :: proc(entity: ^Entity) {
	playerStart: Entity = {}
	entity.type = PlayerEnd{}
}

