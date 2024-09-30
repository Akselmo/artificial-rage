package entity
import "core:c/libc"
import "core:fmt"
import "core:math"
import "core:math/linalg"
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

// Holds all the shared entity structs like transform etc..
// TODO: move these things to their own files, use tagged unions
// fix the update proc

WALL_MODEL_ID: i32 : -2
ACTOR_POSITION_Y: f32 : 0.0
ACTOR_GRAVEYARD_POSITION: f32 : 999.0
ACTOR_MAX_DISTANCE_FROM_PLAYER: f32 : 1.25
ACTOR_DEFAULT_MOVEMENT_SPEED: f32 : 0.45 // Lower values result to lower speed
ACTOR_DEFAULT_ROTATION_SPEED: f32 : 3.0
ACTOR_DEFAULT_ANIMATION_SPEED: f32 : 60.0 // Animation played in FPS
ITEM_START_POSITION_Y: f32 : 0.1

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

Visuals :: struct {
	modelFileName:   string,
	textureFilename: string,
	model:           rl.Model,
	isBillboard:     bool,
}

Audio :: struct {
	fileName: string,
	volume:   f32,
	loop:     bool,
}

Type :: union {
	PlayerStart,
	PlayerEnd,
	WallCargo,
	WallCargoScuffed,
	Item,
	Enemy,
	Projectile,
}

//NOTE: maybe the entity could only hold id, active, transform and type?
Entity :: struct {
	id:        i32,
	active:    bool,
	transform: Transform,
	visuals:   Visuals,
	audio:     Audio,
	type:      Type,
}

// Amount of entities loaded in a scene
entitiesInScene: [dynamic]Entity

Update :: proc(entity: ^Entity) {
	if (!entity.active) {
		return
	}
	Draw(entity)

	#partial switch _ in entity.type {
	case Enemy:
		EnemyUpdate(entity)
	}

}

Draw :: proc(entity: ^Entity) {
	rl.DrawModel(entity.visuals.model, entity.transform.position, entity.transform.scale, rl.WHITE)
	if (entity.visuals.isBillboard) {
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

	ent, ok := entity.type.(Enemy)
	if (!ok) {
		return false
	}

	if (rl.Vector3Distance(Player.transform.position, entity.transform.position) > 5.0 &&
		   entity.type.(Enemy).playerSpotted) {
		return false
	}

	rayCast := CreateRay(entity)

	hitPlayer: bool = false
	levelDistance: f32 = libc.INFINITY
	playerDistance: f32 = libc.INFINITY

	for i := 0; i < len(entitiesInScene); i += 1 {
		ent := entitiesInScene[i]
		if (ent.id != 0 && ent.id != entity.id && ent.visuals.isBillboard) {
			pos: rl.Vector3 = ent.transform.position
			hitLevel: rl.RayCollision = rl.GetRayCollisionMesh(
				rayCast,
				ent.visuals.model.meshes[0],
				rl.MatrixTranslate(pos.x, pos.y, pos.z),
			)
			if (hitLevel.hit) {
				if (hitLevel.distance < levelDistance) {
					levelDistance = hitLevel.distance
				}
			}
		}
	}

	playerDistance = rl.Vector3Length(Player.transform.position - rayCast.position)

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
	ent, ok := entity.type.(Enemy)

	if (!ok) {
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
		entity.transform.position = linalg.lerp(
			entity.transform.position,
			entityNewPosition,
			entity.type.(Enemy).movementSpeed * rl.GetFrameTime(),
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
		if (!entity.visuals.isBillboard) {
			ent, ok := entity.type.(Enemy)
			if (ok) {
				enemyHit: rl.RayCollision = rl.GetRayCollisionBox(rayCast, entity.transform.boundingBox)
				if (enemyHit.hit && !entity.type.(Enemy).dead) {

					dist := rl.Vector3Length(entity.transform.position - rayCast.position)
					if (dist < enemyDistance) {
						enemyDistance = dist
						hitEnemyData = entity
					}
				}
			} else {
				pos: rl.Vector3 = entity.transform.position
				if (entity.visuals.model.meshCount > 0) {
					hitLevel: rl.RayCollision = rl.GetRayCollisionMesh(
						rayCast,
						entity.visuals.model.meshes[0],
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
	ent, ok := entity.type.(Enemy)
	if (!ok) {
		return
	}

	actor := &entity.type.(Enemy)
	if (!actor.dead) {
		if (!actor.playerSpotted) {
			actor.playerSpotted = true
		}

		actor.health -= damageAmount

		fmt.printfln("Entity id %[0]v took %[1]v damage, health left: %[2]v", entity.id, damageAmount, actor.health)

		if (actor.health <= 0) {
			Destroy(entity)
		}
	}
}

Destroy :: proc(entity: ^Entity) {

	fmt.printfln("Entity dead")
	ent, ok := entity.type.(Enemy)
	if (ok) {
		enemy := &entity.type.(Enemy)
		// HACK: Dirty hack to move bounding box outside of map so it cant be collided to.
		// We want to keep entity in the memory so we can use its position to display the
		// corpse/death anim
		deadBoxPos: rl.Vector3 = rl.Vector3 {
			ACTOR_GRAVEYARD_POSITION,
			ACTOR_GRAVEYARD_POSITION,
			ACTOR_GRAVEYARD_POSITION,
		}
		entity.transform.boundingBox = utilities.MakeBoundingBox(deadBoxPos, rl.Vector3(0))
		enemy.dead = true
	} else {
		entity.active = false
	}
}

FireAtPlayer :: proc(entity: ^Entity, nextFire: f32) -> bool {
	ent, ok := entity.type.(Enemy)
	if (!ok) {
		return false
	}

	enemy := &entity.type.(Enemy)
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
	diff: rl.Vector3 = entity.transform.position - targetPosition
	y_angle: f32 = -(libc.atan2f(diff.z, diff.x) + math.PI / 2.0)
	newRotation: rl.Vector3 = rl.Vector3{0, y_angle, 0}

	start: rl.Quaternion = rl.QuaternionFromEuler(
		entity.transform.rotation.z,
		entity.transform.rotation.y,
		entity.transform.rotation.x,
	)
	end: rl.Quaternion = rl.QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x)
	slerp: rl.Quaternion = rl.QuaternionSlerp(start, end, entity.type.(Enemy).rotationSpeed * rl.GetFrameTime())

	entity.visuals.model.transform = rl.QuaternionToMatrix(slerp)
	entity.transform.rotation = newRotation
}

HandlePlayerPickup :: proc(entity: ^Entity) {

}

Create :: proc(type: i32, position: rl.Vector3, id: i32) -> Entity {
	entity: Entity = {}

	entity.id = id
	entity.transform.position = position
	entity.transform.canCollide = true

	// TODO create the things


	switch type {
	case 0:
		//empty
		break
	case 1:
		//start
		CreatePlayerStart(&entity)
	case 2:
		//end
		CreatePlayerEnd(&entity)
	case 3:
		//wall1
		CreateWallCargo(&entity)
	case 4:
		//wall2
		CreateWallCargoScuffed(&entity)
	case 5:
		//enemy
		CreateEnemy(&entity)
	case 6: //health_small
	case 7: //health_medium
	case 8: //health_large
	case 9: //clutter
	case 10: //pickup_pistol
	case 11: //pickup_rifle
	case 12: //pickup_shotgun
	case 13: //pickup_railgun
	case 14: //ammo_pistol
	case 15: //ammo_rifle
	case 16: //ammo_shotgun
	case 17: //ammo_railgun
	case 18: //pickup_teleportkey

	}


	return entity
}

