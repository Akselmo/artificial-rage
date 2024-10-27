package entity


PlayerStart :: struct {}


CreatePlayerStart :: proc(entity: ^Entity) {
	playerStart: Entity = {}
	entity.type = PlayerStart{}
}

