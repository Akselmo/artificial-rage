package entity
import "core:c/libc"
import "core:fmt"
import "core:math"
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

WALL_MODEL_ID: i32 : -2
ACTOR_POSITION_Y: f32 : 0.25
ACTOR_GRAVEYARD_POSITION: f32 : 999.0
ACTOR_MAX_DISTANCE_FROM_PLAYER: f32 : 1.25
ACTOR_DEFAULT_MOVEMENT_SPEED: f32 : 0.45 // Lower values result to lower speed
ACTOR_DEFAULT_ROTATION_SPEED: f32 : 3.0
ACTOR_DEFAULT_ANIMATION_SPEED: f32 : 30.0 // Animation played in FPS
ITEM_START_POSITION_Y: f32 : 0.1

Type :: enum {
	NONE, // This is ignored in entities.json
	START, // Tile map template index is 0. Here index is 1.
	END,
	WALL_CARGO,
	WALL_CARGO_SCUFFED,
	ENEMY_DEFAULT,
	ITEM_HEALTH_SMALL,
	ITEM_HEALTH_MEDIUM,
	ITEM_HEALTH_LARGE,
	ITEM_CLUTTER, // an item that provides collision but nothing else
	ITEM_PICKUP_PISTOL, // Gives weapon and some ammo
	ITEM_PICKUP_RIFLE,
	ITEM_PICKUP_SHOTGUN,
	ITEM_PICKUP_RAILGUN,
	ITEM_AMMO_PISTOL, // Gives only ammo
	ITEM_AMMO_RIFLE,
	ITEM_AMMO_SHOTGUN,
	ITEM_AMMO_RAILGUN,
	ITEM_KEY_TELEPORT, // Needed to go through ending teleporter
	PROJECTILE,
}


Item :: struct {
	pickup: bool,
	value:  i32,
}

Transform :: struct {
	position:    rl.Vector3,
	rotation:    rl.Vector3,
	size:        rl.Vector3,
	scale:       f32,
	boundingBox: rl.BoundingBox,
	canCollide:  bool,
}

Model :: struct {
	fileName:        string,
	textureFilename: string,
	data:            rl.Model,
	isBillboard:     bool,
}

EntityData :: union {
	Item,
	Enemy,
	Projectile,
}

Data :: struct {
	type:  Type,
	value: EntityData,
}

Entity :: struct {
	id:        i32,
	active:    bool,
	transform: Transform,
	model:     Model,
	data:      Data,
}

// Amount of entities loaded in a scene
entitiesInScene: [dynamic]Entity

Update :: proc(entity: ^Entity) {
	if (!entity.active) {
		return
	}
	Draw(entity)
	if (entity.data.type != Type.ENEMY_DEFAULT) {
		return
	} else {
		EnemyUpdate(entity)
	}

}

Draw :: proc(entity: ^Entity) {
	rl.DrawModel(entity.model.data, entity.transform.position, entity.transform.scale, rl.WHITE)
	if (entity.model.isBillboard) {
		RotateTowards(entity, Player.transform.position)
	}

}

CreateRay :: proc(entity: ^Entity) -> rl.Ray {
	v := rl.Vector3Normalize(entity.transform.position - Player.transform.position)

	rayCast: rl.Ray = {
		direction = rl.Vector3{-1.0 * v.x, -1.0 * v.y, -1.0 * v.z},
		position  = entity.transform.position,
	}

	return rayCast
}

TestPlayerHit :: proc(entity: ^Entity) -> bool {
	// TODO: this function can be quite heavy, could give it a tickrate?
	//  every 1-2 seconds instead of every frame

	if (entity.data.type != Type.ENEMY_DEFAULT) {
		return false
	}

	if (rl.Vector3Distance(Player.transform.position, entity.transform.position) > 5.0 &&
		   entity.data.value.(Enemy).playerSpotted) {
		return false
	}

	rayCast := CreateRay(entity)

	hitPlayer: bool = false
	levelDistance: f32 = libc.INFINITY
	playerDistance: f32 = libc.INFINITY

	for i := 0; i < len(entitiesInScene); i += 1 {
		ent := entitiesInScene[i]
		if (ent.id != 0 && ent.id != entity.id && ent.model.isBillboard) {
			pos: rl.Vector3 = ent.transform.position
			hitLevel: rl.RayCollision = rl.GetRayCollisionMesh(
				rayCast,
				ent.model.data.meshes[0],
				rl.MatrixTranslate(pos.x, pos.y, pos.z),
			)
			if (hitLevel.hit) {
				if (hitLevel.distance < levelDistance) {
					levelDistance = hitLevel.distance
				}
			}
		}
	}

	playerDistance = rl.Vector3Length(rl.Vector3Subtract(Player.transform.position, rayCast.position))

	hitPlayer = (playerDistance < levelDistance)

	return hitPlayer
}

CheckCollision :: proc(entityPos: rl.Vector3, entitySize: rl.Vector3, entityId: i32) -> bool {
	entityBox := utilities.MakeBoundingBox(entityPos, entitySize)
	for i := 0; i < len(entitiesInScene); i += 1 {
		ent := entitiesInScene[i]
		if (ent.id != entityId && rl.CheckCollisionBoxes(entityBox, ent.transform.boundingBox)) {
			if (ent.transform.canCollide) {
				return true
			} else if (entityId == PLAYER_ID) {
				// TODO: make player type instead of comparing ids
				HandlePlayerPickup(&ent)
			}
		}
	}
	return false
}

UpdatePosition :: proc(entity: ^Entity) -> bool {
	if (entity.data.type != Type.ENEMY_DEFAULT) {
		return false
	}

	moving: bool = true
	// Move entity towards player
	distanceFromPlayer := entity.transform.position - Player.transform.position

	// Check if player can be seen (first racyast hit returns player)
	// If in certain range from player, stop

	if (libc.fabsf(distanceFromPlayer.x) >= ACTOR_MAX_DISTANCE_FROM_PLAYER ||
		   libc.fabsf(distanceFromPlayer.z) >= ACTOR_MAX_DISTANCE_FROM_PLAYER) {

		entityOldPosition: rl.Vector3 = entity.transform.position
		entityNewPosition: rl.Vector3 = {Player.transform.position.x, ACTOR_POSITION_Y, Player.transform.position.z}
		entity.transform.position = rl.Vector3Lerp(
			entity.transform.position,
			entityNewPosition,
			entity.data.value.(Enemy).movementSpeed * rl.GetFrameTime(),
		)
		if (CheckCollision(entity.transform.position, entity.transform.size, entity.id)) {
			entity.transform.position = entityOldPosition
		}
	} else {
		moving = false
	}
	entity.transform.boundingBox = utilities.MakeBoundingBox(entity.transform.position, entity.transform.size)

	return moving
}

// Returns the id of closest item the raycast hits.
// If 0, then it hit either a wall or nothing.
RaycastHitsEntityId :: proc(rayCast: rl.Ray) -> i32 {
	levelDistance: f32 = libc.INFINITY
	enemyDistance: f32 = libc.INFINITY
	entitiesAmount: i32 = cast(i32)len(entitiesInScene)
	hitEnemyData: Entity

	for i: i32 = 0; i < entitiesAmount; i += 1 {
		entity := entitiesInScene[i]
		if (entity.data.type != Type.NONE && !entity.model.isBillboard) {
			if (entity.data.type == Type.ENEMY_DEFAULT) {
				enemyHit: rl.RayCollision = rl.GetRayCollisionBox(rayCast, entity.transform.boundingBox)
				if (enemyHit.hit && !entity.data.value.(Enemy).dead) {

					dist := rl.Vector3Length(rl.Vector3Subtract(entity.transform.position, rayCast.position))
					if (dist < enemyDistance) {
						enemyDistance = dist
						hitEnemyData = entity
					}
				}
			} else {
				pos: rl.Vector3 = entity.transform.position
				if (entity.model.data.meshCount > 0) {
					hitLevel: rl.RayCollision = rl.GetRayCollisionMesh(
						rayCast,
						entity.model.data.meshes[0],
						rl.MatrixTranslate(pos.x, pos.y, pos.z),
					)
					if (hitLevel.hit && hitLevel.distance < levelDistance) {
						levelDistance = hitLevel.distance
					}
				}
			}
		}

	}
	if (enemyDistance < levelDistance) {
		return hitEnemyData.id
	}

	return 0

}

TakeDamage :: proc(entity: ^Entity, damageAmount: i32) {
	if (entity.data.type != Type.ENEMY_DEFAULT) {
		return
	}

	actor := entity.data.value.(Enemy)
	if (actor.dead) {
		if (!actor.playerSpotted) {
			actor.playerSpotted = true
		}

		actor.health -= damageAmount

		fmt.printfln("Entity id %[0]v took %[1]v damage", entity.id, damageAmount)

		if (actor.health <= 0) {
			Destroy(entity)
		}
	}
}

Destroy :: proc(entity: ^Entity) {

	if (entity.data.type == Type.ENEMY_DEFAULT) {
		enemy := &entity.data.value.(Enemy)
		// HACK: Dirty hack to move bounding box outside of map so it cant be collided to.
		// We want to keep entity in the memory so we can use its position to display the
		// corpse/death anim
		deadBoxPos: rl.Vector3 = rl.Vector3 {
			ACTOR_GRAVEYARD_POSITION,
			ACTOR_GRAVEYARD_POSITION,
			ACTOR_GRAVEYARD_POSITION,
		}
		entity.transform.boundingBox = utilities.MakeBoundingBox(deadBoxPos, rl.Vector3Zero())
		enemy.dead = true
	} else {
		entity.active = false
	}
}

FireAtPlayer :: proc(entity: ^Entity, nextFire: f32) -> bool {
	if (entity.data.type != Type.ENEMY_DEFAULT) {
		return false
	}

	enemy := &entity.data.value.(Enemy)
	RotateTowards(entity, Player.transform.position)
	if (nextFire > 0) {
		enemy.nextFire -= rl.GetFrameTime()
		return false
	} else {
		enemy.attacking = true
		ProjectileCreate(CreateRay(entity), rl.Vector3{0.2, 0.2, 0.2}, enemy.damage, entity.id, rl.PURPLE)
		enemy.nextFire = enemy.fireRate
		return true
	}
}

RotateTowards :: proc(entity: ^Entity, targetPosition: rl.Vector3) {
	diff: rl.Vector3 = rl.Vector3Subtract(entity.transform.position, targetPosition)
	y_angle: f32 = -(libc.atan2f(diff.z, diff.x) + math.PI / 2.0)
	newRotation: rl.Vector3 = rl.Vector3{0, y_angle, 0}

	start: rl.Quaternion = rl.QuaternionFromEuler(
		entity.transform.rotation.z,
		entity.transform.rotation.y,
		entity.transform.rotation.x,
	)
	end: rl.Quaternion = rl.QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x)
	slerp: rl.Quaternion = rl.QuaternionSlerp(start, end, entity.data.value.(Enemy).rotationSpeed * rl.GetFrameTime())

	entity.model.data.transform = rl.QuaternionToMatrix(slerp)
	entity.transform.rotation = newRotation
}

HandlePlayerPickup :: proc(entity: ^Entity) {

}

Create :: proc(type: Type, position: rl.Vector3, id: i32) -> Entity {
	entity: Entity = {}

	entity.data.type = type
	entity.id = id
	entity.transform.position = position
	entity.transform.canCollide = true

	#partial switch type {
	case Type.WALL_CARGO:
		entity.model.textureFilename = "./assets/textures/wall1.png"
		CreateWall(&entity)
	case Type.WALL_CARGO_SCUFFED:
		entity.model.textureFilename = "./assets/textures/wall2.png"
		CreateWall(&entity)
	case Type.ENEMY_DEFAULT:
		entity.model.fileName = "./assets/models/enemy.m3d"
		CreateEnemy(&entity)
	case Type.ITEM_HEALTH_SMALL:
		entity.model.textureFilename = "./assets/textures/health_small.png"
		CreateItem(&entity, true, 5)
	case Type.ITEM_HEALTH_MEDIUM:
		entity.model.textureFilename = "./assets/textures/health_medium.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_HEALTH_LARGE:
		entity.model.textureFilename = "./assets/textures/health_large.png"
		CreateItem(&entity, true, 15)
	case Type.ITEM_PICKUP_PISTOL:
		entity.model.textureFilename = "./assets/textures/pistol.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_PICKUP_RIFLE:
		entity.model.textureFilename = "./assets/textures/rifle.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_PICKUP_SHOTGUN:
		entity.model.textureFilename = "./assets/textures/shotgun.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_PICKUP_RAILGUN:
		entity.model.textureFilename = "./assets/textures/railgun.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_AMMO_PISTOL:
		entity.model.textureFilename = "./assets/textures/ammo_pistol.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_AMMO_RIFLE:
		entity.model.textureFilename = "./assets/textures/ammo_rifle.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_AMMO_SHOTGUN:
		entity.model.textureFilename = "./assets/textures/ammo_shotgun.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_AMMO_RAILGUN:
		entity.model.textureFilename = "./assets/textures/ammo_railgun.png"
		CreateItem(&entity, true, 10)
	case Type.ITEM_KEY_TELEPORT:
		entity.model.textureFilename = "./assets/textures/pickup_teleportkey.png"
		CreateItem(&entity, true, 10)
	}

	return entity
}

SetupTransform :: proc(entity: ^Entity, pos: rl.Vector3, rot: rl.Vector3, size: rl.Vector3, scale: f32) {
	entity.transform.boundingBox = utilities.MakeBoundingBox(pos, size)
	entity.transform.position = pos
	entity.transform.rotation = rot
	entity.transform.size = size
	entity.transform.scale = scale
}

// NOTE: Should this be in scene? Idk
CreateWall :: proc(entity: ^Entity) {
	textureImage := rl.LoadImage(strings.clone_to_cstring(entity.model.textureFilename))

	rl.ImageFlipVertical(&textureImage)

	texture := rl.LoadTextureFromImage(textureImage)

	cube: rl.Mesh = rl.GenMeshCube(1.0, 1.0, 1.0)
	entity.model.data = rl.LoadModelFromMesh(cube)
	SetupTransform(entity, entity.transform.position, rl.Vector3Zero(), rl.Vector3One(), 1.0)

	entity.model.data.materials[0].maps[rl.MaterialMapIndex.ALBEDO].texture = texture

	entity.active = true


}

CreateEnemy :: proc(entity: ^Entity) {

	position := rl.Vector3{entity.transform.position.x, ACTOR_POSITION_Y, entity.transform.position.x}

	size := rl.Vector3{0.25, 1.1, 0.25}

	SetupTransform(entity, position, rl.Vector3Zero(), size, 0.5)

	entity.model.data = rl.LoadModel(strings.clone_to_cstring(entity.model.fileName))

	enemy: Enemy = {
		dead          = false,
		moving        = false,
		attacking     = false,
		playerSpotted = false,
		damage        = 2,
		health        = 15,
		movementSpeed = ACTOR_DEFAULT_MOVEMENT_SPEED,
		rotationSpeed = ACTOR_DEFAULT_ROTATION_SPEED,
		fireRate      = 5.75,
		nextFire      = 5.75,
		animator      = EnemyAnimations(entity.model.fileName),
	}

	entity.data.value = enemy

	entity.active = true

}

CreateItem :: proc(entity: ^Entity, pickup: bool, value: i32) {

}

