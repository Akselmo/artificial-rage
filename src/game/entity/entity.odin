package entity
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

Transform :: struct {
	position:    rl.Vector3,
	rotation:    rl.Vector3,
	size:        rl.Vector3,
	scale:       f32,
	boundingBox: rl.BoundingBox,
	canCollide:  bool,
	isPickup:    bool,
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
	Enemy,
	Projectile,
	ItemHealth,
	ItemAmmo,
	ItemWeapon,
	ItemKey,
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

// Used when no entity is found
DummyEntity: Entity = {
	id = -10,
	transform = {position = rl.Vector3{999999999, 999999999, 999999999}},
}

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


CheckCollision :: proc(entityPos: rl.Vector3, entitySize: rl.Vector3, entityId: i32) -> bool {
	entityBox := utilities.MakeBoundingBox(entityPos, entitySize)
	for &entity in entitiesInScene {
		if (!entity.active) {
			continue
		}
		if (entity.id != entityId && rl.CheckCollisionBoxes(entityBox, entity.transform.boundingBox)) {
			if (entity.transform.canCollide) {
				return true
			} else if (entityId == PLAYER_ID) {
				// TODO: make player type instead of comparing ids
				HandlePlayerPickup(&entity)
				return false
			}
		}
	}
	return false
}


// Returns the closest entity the player raycast hits
RaycastHitsEntity :: proc(rayCast: rl.Ray) -> ^Entity {
	hitEntity := DummyEntity

	entityDistanceFromPlayer: f32 = math.F32_MAX

	for &entity in entitiesInScene {
		if (!entity.visuals.isBillboard) {
			#partial switch _ in entity.type {
			case Enemy:
				enemy := &entity.type.(Enemy)
				enemyHit: rl.RayCollision = rl.GetRayCollisionBox(rayCast, entity.transform.boundingBox)
				if (enemyHit.hit && !enemy.dead) {
					dist := rl.Vector3Length(entity.transform.position - rayCast.position)
					if (dist < entityDistanceFromPlayer) {
						entityDistanceFromPlayer = dist
						hitEntity = entity
					}
				}
			case:
				pos: rl.Vector3 = entity.transform.position
				if (entity.visuals.model.meshCount > 0) {
					hitLevel: rl.RayCollision = rl.GetRayCollisionMesh(
						rayCast,
						entity.visuals.model.meshes[0],
						rl.MatrixTranslate(pos.x, pos.y, pos.z),
					)
					if (hitLevel.hit && hitLevel.distance < entityDistanceFromPlayer) {
						entityDistanceFromPlayer = hitLevel.distance
						hitEntity = entity
					}
				}
			}
		}
	}
	return GetEntityWithId(hitEntity.id)
}

GetEntityWithId :: proc(id: i32) -> ^Entity {
	for &entity in entitiesInScene {
		if (id == entity.id) {
			return &entity
		}
	}
	return &DummyEntity
}

Destroy :: proc(entity: ^Entity) {
	#partial switch _ in entity.type {
	case Enemy:
		EnemyDestroy(entity)
	case:
		entity.active = false
	}
}


RotateTowards :: proc(entity: ^Entity, targetPosition: rl.Vector3) {
	diff: rl.Vector3 = entity.transform.position - targetPosition
	y_angle: f32 = -(math.atan2(diff.z, diff.x) + math.PI / 2.0)
	newRotation: rl.Vector3 = rl.Vector3{0, y_angle, 0}

	start: rl.Quaternion = rl.QuaternionFromEuler(
		entity.transform.rotation.z,
		entity.transform.rotation.y,
		entity.transform.rotation.x,
	)
	end: rl.Quaternion = rl.QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x)

	slerp: rl.Quaternion

	#partial switch _ in entity.type {
	case Enemy:
		slerp = rl.QuaternionSlerp(start, end, entity.type.(Enemy).rotationSpeed * rl.GetFrameTime())
	case:
		slerp = rl.QuaternionSlerp(start, end, 3.0 * rl.GetFrameTime())
	}

	entity.visuals.model.transform = rl.QuaternionToMatrix(slerp)
	entity.transform.rotation = newRotation
}

HandlePlayerPickup :: proc(entity: ^Entity) {
	destroyed := false
	#partial switch ent in entity.type {
	case ItemHealth:
		if (Player.health < PLAYER_MAX_HEALTH) {
			PlayerSetHealth(ent.healAmount)
			destroyed = true
		}
	case ItemAmmo:
		destroyed = WeaponAddAmmo(ent.ammoAmount, ent.ammoMorph)
	case ItemWeapon:
		pickedUp := WeaponPickup(ent.weaponType)
		ammoAdded := WeaponAddAmmo(ent.ammoAmount, ent.weaponType)
		destroyed = pickedUp || ammoAdded
	case ItemKey:
		destroyed = true
		Player.hasTeleportKey = true
	case:
		fmt.printfln("Unimplemented: %[0]v", entity.type)
	}
	if (destroyed) {
		entity.transform.isPickup = false
		entity.active = false
		Destroy(entity)
	}
}

