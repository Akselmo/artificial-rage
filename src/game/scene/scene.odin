package scene

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"
import "core:unicode/utf8"
import "src:game/entity"
import rl "vendor:raylib"

MAX_PROJECTILE_AMOUNT: i32 : 254

// do we need this struct? can we just place things in here
Data :: struct
{
	name:                string,
	height:              i32,
	width:               i32,
	square:              [dynamic]i32,
	squareCount:         i32,
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
		arr := fmt.aprint(value, sep = ",")
		// we need to convert runes to string
		for d in arr
		{
			r: []rune = {d}
			s := utf8.runes_to_string(r)
			append(&level.square, cast(i32)strconv.atoi(s))
		}
		return true
	case:
		return false
	}
}

PlaceEntities :: proc()
{

}

Update :: proc()
{

}

UpdateProjectiles :: proc()
{

}

CheckCollision :: proc(entityPos: rl.Vector3, entitySize: rl.Vector3, entityId: i32) -> bool
{
	return false
}

MakeCustomPlaneMesh :: proc(height: f32, width: f32, textureSize: f32)
{
	//return rl.Mesh
}

// Could be moved to utils
AllocateMeshData :: proc(mesh: ^rl.Mesh, triangleCount: i32)
{

}

AddEntityToScene :: proc(type: entity.Type, mx: f32, my: f32, id: i32)
{

}

