package entity
import rl "vendor:raylib"

WallCargoScuffed :: struct {
}

CreateWallCargoScuffed :: proc(entity: ^Entity) {
	entity.type = WallCargoScuffed{}
	entity.visuals.texture = TextureWallCargoScuffed^
	SharedWallCubeModel(entity)
	entity.active = true
}

