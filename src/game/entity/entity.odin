package entity
import "core:c/libc"
import "core:fmt"
import rl "vendor:raylib"

WALL_MODEL_ID: i32 : -2
ACTOR_POSITION_Y: f32 : 0.0
ACTOR_GRAVEYARD_POSITION: f32 : 999.0
ACTOR_MAX_DISTANCE_FROM_PLAYER: f32 : 1.25
ACTOR_DEFAULT_MOVEMENT_SPEED: f32 : 0.45 // Lower values result to lower speed
ACTOR_DEFAULT_ROTATION_SPEED: f32 : 3.0
ACTOR_DEFAULT_ANIMATION_SPEED: f32 : 30.0 // Animation played in FPS
ITEM_START_POSITION_Y: f32 : 0.1

Type :: enum
{
	ENTITY_NONE, // This is ignored in entities.json
	ENTITY_START, // Tile map template index is 0. Here index is 1.
	ENTITY_END,
	ENTITY_WALL_CARGO,
	ENTITY_WALL_CARGO_SCUFFED,
	ENTITY_ENEMY_DEFAULT,
	ENTITY_ITEM_HEALTH_SMALL,
	ENTITY_ITEM_HEALTH_MEDIUM,
	ENTITY_ITEM_HEALTH_LARGE,
	ENTITY_ITEM_CLUTTER, // an item that provides collision but nothing else
	ENTITY_ITEM_PICKUP_PISTOL, // Gives weapon and some ammo
	ENTITY_ITEM_PICKUP_RIFLE,
	ENTITY_ITEM_PICKUP_SHOTGUN,
	ENTITY_ITEM_PICKUP_RAILGUN,
	ENTITY_ITEM_AMMO_PISTOL, // Gives only ammo
	ENTITY_ITEM_AMMO_RIFLE,
	ENTITY_ITEM_AMMO_SHOTGUN,
	ENTITY_ITEM_AMMO_RAILGUN,
	ENTITY_ITEM_KEY_TELEPORT, // Needed to go through ending teleporter
}


Actor :: struct
{
	health:        i32,
	damage:        i32,
	dead:          bool,
	moving:        bool,
	attacking:     bool,
	playerSpotted: bool,
	movementSpeed: f32,
	rotationSpeed: f32,
	fireRate:      f32,
	nextFire:      f32,
	// Animator
	tickRate:      f32,
}

Item :: struct
{
	pickup: bool,
	value:  i32,
}

Transform :: struct
{
	position:    rl.Vector3,
	rotation:    rl.Vector3,
	size:        rl.Vector3,
	scale:       f32,
	boundingBox: rl.BoundingBox,
	canCollide:  bool,
}

Model :: struct
{
	fileName:        string,
	textureFilename: string,
	data:            rl.Model,
	isBillboard:     bool,
}

EntityData :: union
{
	Item,
	Actor,
}

Data :: struct
{
	type:  Type,
	value: EntityData,
}

Entity :: struct
{
	id:        i32,
	active:    bool,
	transform: Transform,
	model:     Model,
	data:      Data,
}

Update :: proc(entity: ^Entity)
{
	Draw(entity)

	if (entity.data.type != Type.ENTITY_ENEMY_DEFAULT)
	{
		return
	}
}

Draw :: proc(entity: ^Entity)
{

}

CreateRay :: proc(entity: ^Entity) -> rl.Ray
{
	v := rl.Vector3Normalize(entity.transform.position - Player.transform.position)

	rayCast: rl.Ray = \
	{
		direction = rl.Vector3{-1.0 * v.x, -1.0 * v.y, -1.0 * v.z},
		position  = entity.transform.position,
	}

	return rayCast
}

TestPlayerHit :: proc(entity: ^Entity) -> bool
{
	// TODO: this function can be quite heavy, could give it a tickrate?
	//  every 1-2 seconds instead of every frame

	if (entity.data.type != Type.ENTITY_ENEMY_DEFAULT)
	{
		return false
	}

	if (rl.Vector3Distance(Player.transform.position, entity.transform.position) > 5.0 &&
		   entity.data.value.(Actor).playerSpotted)
	{
		return false
	}

	rayCast := CreateRay(entity)

	hitPlayer: bool = false
	levelDistance: f32 = libc.INFINITY
	playerDistance: f32 = libc.INFINITY
	//entitiesAmount : i32 : scene.size
	//TODO need to do scene for this
	return false
}

UpdatePosition :: proc(entity: ^Entity) -> bool
{
	if (entity.data.type != Type.ENTITY_ENEMY_DEFAULT)
	{
		return false
	}

	moving: bool = true
	// Move entity towards player
	distanceFromPlayer := entity.transform.position - Player.transform.position

	// Check if player can be seen (first racyast hit returns player)
	// If in certain range from player, stop

	if (libc.fabsf(distanceFromPlayer.x) >= ACTOR_MAX_DISTANCE_FROM_PLAYER ||
		   libc.fabsf(distanceFromPlayer.z) >= ACTOR_MAX_DISTANCE_FROM_PLAYER)
	{
		entityOldPosition: rl.Vector3 = entity.transform.position
		entityNewPosition: rl.Vector3 = {
			Player.transform.position.x,
			ACTOR_POSITION_Y,
			Player.transform.position.z,
		}
		entity.transform.position = rl.Vector3Lerp(
			entity.transform.position,
			entityNewPosition,
			entity.data.value.(Actor).movementSpeed * rl.GetFrameTime(),
		)

		// if scene check collision etc etc
		return false
	}

	return false
}


TakeDamage :: proc(entity: ^Entity, damageAmount: i32)
{
	if (entity.data.type != Type.ENTITY_ENEMY_DEFAULT)
	{
		return
	}

	actor := entity.data.value.(Actor)
	if (actor.dead)
	{
		if (!actor.playerSpotted)
		{
			actor.playerSpotted = true
		}

		actor.health -= damageAmount

		fmt.printfln("Entity id %[0]v took %[1]v damage", entity.id, damageAmount)

		if (actor.health <= 0)
		{
			Destroy(entity)
		}
	}
}

Destroy :: proc(entity: ^Entity)
{

}

FireAtPlayer :: proc(entity: ^Entity, nextFire: f32) -> bool
{
	if (entity.data.type != Type.ENTITY_ENEMY_DEFAULT)
	{
		return false
	}
	return false
}

RotateTowards :: proc(entity: ^Entity, targetPosition: rl.Vector3)
{

}

HandlePlayerPickup :: proc(entity: ^Entity)
{

}

Create :: proc(type: Type, position: rl.Vector3, id: i32) //-> Entity
{

}

SetupTransform :: proc(
	entity: ^Entity,
	pos: rl.Vector3,
	rot: rl.Vector3,
	size: rl.Vector3,
	scale: f32,
)
{

}

// NOTE: Should this be in scene? Idk
CreateWall :: proc(entity: ^Entity)
{

}

CreateEnemy :: proc(entity: ^Entity)
{

}

CreateItem :: proc(entity: ^Entity, pickup: bool, value: i32)
{

}

