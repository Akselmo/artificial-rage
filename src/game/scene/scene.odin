package scene

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"
import "core:unicode/utf8"
import "src:game/entity"
import "src:game/utilities"
import rl "vendor:raylib"

MAX_PROJECTILE_AMOUNT: i32 : 254

// do we need this struct? can we just place things in here
Data :: struct
{
	name:                string,
	height:              i32,
	width:               i32,
	squares:             [dynamic]i32,
	ceilingHeight:       i32,
	floorPlane:          rl.Model,
	floorPlaneTexture:   rl.Texture2D,
	ceilingPlane:        rl.Model,
	ceilingPlaneTexture: rl.Texture2D,
	entitiesTotal:       i32, // entitiesTotal = (last_entity_id + 1) in entities.json
	entities:            [dynamic]entity.Entity,
	// projectiles: [dynamic]projectile.Projectile
	position:            rl.Vector3,
	startPosition:       rl.Vector3,
	endPosition:         rl.Vector3,
	size:                i32,
}

level: Data = {}

Initialize :: proc() -> rl.Camera
{
	Build()
	return entity.PlayerInitializeCamera(level.startPosition.x, level.startPosition.z)
}

Build :: proc()
{
	LoadSceneConfig()
	PlaceEntities()
}

LoadSceneConfig :: proc()
{
	fileName := "./assets/levels/level1.cfg"
	data, success := os.read_entire_file(fileName)

	if (!success)
	{
		fmt.printfln("Failed to open level config file %[0]v", fileName)
		return
	}

	sceneConfigText := string(data)
	sceneArr := strings.split_lines(sceneConfigText)
	for item in sceneArr
	{
		keyValuePair := strings.split(item, "=")
		if (len(keyValuePair) < 2)
		{
			continue
		}

		if (ParseConfig(keyValuePair[0], keyValuePair[1]))
		{
			fmt.printfln(
				"Parsed level.cfg key-value: %[0]v - %[1]v",
				keyValuePair[0],
				keyValuePair[1],
			)
		}
	}
}

ParseConfig :: proc(key: string, value: string) -> bool
{
	texturesPath := strings.clone_to_cstring(fmt.aprintf("./assets/textures/%[0]v", value))
	switch key
	{
	case "ceilingtexture":
		level.ceilingPlaneTexture = rl.LoadTexture(texturesPath)
		return true
	case "floortexture":
		level.floorPlaneTexture = rl.LoadTexture(texturesPath)
		return true
	case "name":
		level.name = value
		return true
	case "height":
		level.height = cast(i32)strconv.atoi(value)
		return true
	case "width":
		level.width = cast(i32)strconv.atoi(value)
		return true
	case "data":
		arr := strings.split(value, ",")
		for character in arr
		{
			append(&level.squares, cast(i32)strconv.atoi(character))
		}
		return true
	case:
		return false
	}
}

PlaceEntities :: proc()
{
	mapPosZ := cast(f32)level.height
	mapPosX := cast(f32)level.width

	level.ceilingPlane = rl.LoadModelFromMesh(MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0))
	level.floorPlane = rl.LoadModelFromMesh(MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0))

	level.ceilingPlane.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture =
		level.ceilingPlaneTexture
	level.floorPlane.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture =
		level.floorPlaneTexture

	level.position = rl.Vector3{-mapPosX / 2.0, 0.5, -mapPosZ / 2.0}
	level.size = level.height * level.width

	for e: i32 = 0; e < level.size; e += 1
	{
		entityPosX := e % level.width
		entityPosY := e / level.width
		mx := level.position.x - 0.5 + cast(f32)entityPosX * 1.0
		my := level.position.z - 0.5 + cast(f32)entityPosY * 1.0

		AddEntityToScene(cast(entity.Type)level.squares[e], mx, my, e)
	}

	fmt.printfln("Level has total %[0]v entities", level.size)
}

Update :: proc()
{
	rl.DrawModel(
		level.floorPlane,
		rl.Vector3{level.position.x, 0.0, level.position.z},
		1.0,
		rl.WHITE,
	)

	rl.DrawModelEx(
		level.ceilingPlane,
		rl.Vector3{level.position.x, 1.0, -level.position.z},
		rl.Vector3{-1.0, 0.0, 0.0},
		180.0,
		rl.Vector3{1.0, 1.0, 1.0},
		rl.WHITE,
	)

	//TODO I need to grow level.entities to the level.size
	for i := 0; i < len(level.entities); i += 1
	{
		ent := &level.entities[i]
		if (ent.id != 0)
		{
			entity.Update(ent)
		}
	}
}

UpdateProjectiles :: proc()
{

}

// TODO entity should check the collision, not scene!
// Alternatively we could have "GameUpdater" that brings these two together to avoid cyclical deps
CheckCollision :: proc(entityPos: rl.Vector3, entitySize: rl.Vector3, entityId: i32) -> bool
{
	entityBox := utilities.MakeBoundingBox(entityPos, entitySize)
	for i := 0; i < len(level.entities); i += 1
	{
		ent := level.entities[i]
		if (ent.id != entityId && rl.CheckCollisionBoxes(entityBox, ent.transform.boundingBox))
		{
			if (ent.transform.canCollide)
			{
				return true
			}
			 else if (entityId == entity.PLAYER_ID)
			{
				// TODO: make player type instead of comparing ids
				entity.HandlePlayerPickup(&ent)
			}
		}
	}
	return false
}

MakeCustomPlaneMesh :: proc(height: f32, width: f32, textureSize: f32) -> rl.Mesh
{
	mesh: rl.Mesh = {}

	mesh.vertexCount = 6
	mesh.triangleCount = 2

	verts := [?]f32 {
		0.0,
		0.0,
		0.0,
		width,
		0.0,
		height,
		width,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		height,
		width,
		0.0,
		height,
	}

	mesh.vertices = raw_data(verts[:])

	normals := [?]f32 {
		0.0,
		1.0,
		0.0,
		0.0,
		1.0,
		0.0,
		0.0,
		1.0,
		0.0,
		0.0,
		1.0,
		0.0,
		0.0,
		1.0,
		0.0,
		0.0,
		1.0,
		0.0,
	}

	mesh.normals = raw_data(normals[:])


	texcoords := [?]f32 {
		0.0,
		0.0,
		width / textureSize,
		height / textureSize,
		width / textureSize,
		0.0,
		0.0,
		0.0,
		0.0,
		height / textureSize,
		width / textureSize,
		height / textureSize,
	}

	mesh.texcoords = raw_data(texcoords[:])

	rl.UploadMesh(&mesh, false)

	return mesh

}

AddEntityToScene :: proc(type: entity.Type, mx: f32, my: f32, id: i32)
{
	#partial switch type

	{
	case entity.Type.NONE:
		return
	case entity.Type.START:
		level.startPosition = rl.Vector3{mx, 0.0, my}
	case entity.Type.END:
		level.endPosition = rl.Vector3{mx, 0.0, my}
	case:
		append(&level.entities, entity.Create(type, rl.Vector3{mx, 0.5, my}, id))
	}


}

