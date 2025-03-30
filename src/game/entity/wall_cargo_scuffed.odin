package entity
import rl "vendor:raylib"

WallCargoScuffed :: struct {}

CreateWallCargoScuffed :: proc(entity: ^Entity) {
	entity.type = WallCargoScuffed{}
	SharedWallCubeModel(entity)
	entity.visuals.model.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture = TextureWallCargoScuffed^
	entity.active = true
}
