package entity

import "core:fmt"
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

InitializeItem :: proc(entity: ^Entity, isPickup: bool, yPos: f32) {

	entity.transform.isPickup = isPickup
	if (isPickup) {
		entity.visuals.isBillboard = true
		entity.transform.canCollide = false
	}

	entity.transform.position = rl.Vector3{entity.transform.position.x, yPos, entity.transform.position.z}
	entity.transform.rotation = rl.Vector3{0, 0, 0}
	if (entity.transform.scale == 0) {
		fmt.printfln("Entity $[0]v scale is 0, did you forget to set the scale?", entity.type)
	}

	textureName := strings.clone_to_cstring(entity.visuals.textureFilename)
	defer delete(textureName)
	textureImage := rl.LoadImage(textureName)
	rl.ImageFlipVertical(&textureImage)

	texture := rl.LoadTextureFromImage(textureImage)

	cube := rl.GenMeshCube(entity.transform.size.x, entity.transform.size.y, entity.transform.size.z)

	entity.visuals.model = rl.LoadModelFromMesh(cube)

	entity.transform.boundingBox = utilities.MakeBoundingBox(entity.transform.position, entity.transform.size)

	entity.visuals.model.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture = texture

}

