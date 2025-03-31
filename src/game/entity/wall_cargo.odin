package entity
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

WallCargo :: struct {
}

CreateWallCargo :: proc(entity: ^Entity) {
	entity.type = WallCargo{}
	entity.visuals.texture = TextureWallCargo^
	SharedWallCubeModel(entity)
	entity.active = true
}

// Shared between wall cubes
SharedWallCubeModel :: proc(entity: ^Entity) {
	cube: rl.Mesh = rl.GenMeshCube(1.0, 1.0, 1.0)
	entity.visuals.model = rl.LoadModelFromMesh(cube)
	entity.transform.rotation = rl.Vector3(0)
	entity.transform.size = rl.Vector3(1)
	entity.transform.scale = 1.0
	entity.transform.boundingBox = utilities.MakeBoundingBox(entity.transform.position, entity.transform.size)
	entity.visuals.model.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture = entity.visuals.texture

}

