package entity
import rl "vendor:raylib"

WallCargoScuffed :: struct {}

CreateWallCargoScuffed :: proc(entity: ^Entity) {
	entity.type = WallCargoScuffed{}
	entity.visuals.textureFilename = "./assets/textures/wall2.png"
	SharedWallCubeModel(entity)
	entity.active = true
}

