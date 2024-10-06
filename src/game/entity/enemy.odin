package entity
import "core:fmt"
import "core:math"
import "core:math/linalg"
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

ENEMY_POSITION_Y: f32 : 0.0
ENEMY_GRAVEYARD_POSITION: f32 : 999.0
ENEMY_MAX_DISTANCE_FROM_PLAYER: f32 : 1.25
ENEMY_DEFAULT_MOVEMENT_SPEED: f32 : 0.45 // Lower values result to lower speed
ENEMY_DEFAULT_ROTATION_SPEED: f32 : 3.0
ENEMY_DEFAULT_ANIMATION_SPEED: f32 : 60.0 // Animation played in FPS

EnemyAnimationID :: enum {
	DEATH  = 0,
	ATTACK = 1,
	IDLE   = 2,
	MOVE   = 3,
}

Enemy :: struct {
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
	animator:      Animator,
	tickRate:      f32,
}

EnemyAnimations :: proc(modelFileName: string) -> Animator {
	cstringName := strings.clone_to_cstring(modelFileName)
	defer delete(cstringName)
	enemyAnimationsCount: i32 = 0
	enemyLoadedAnimations := rl.LoadModelAnimations(cstringName, &enemyAnimationsCount)

	enemyAnimator: Animator = Animator {
		animationsCount = enemyAnimationsCount,
		animationFrame  = 0,
		nextFrame       = 0.0,
	}

	// NOTE: make sure to append them in right order!

	deathAnim := Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.DEATH],
		firstFrame    = 0,
		lastFrame     = (enemyLoadedAnimations[EnemyAnimationID.DEATH].frameCount - 5),
		id            = cast(i32)EnemyAnimationID.DEATH,
		interruptable = false,
		loopable      = false,
	}
	append(&enemyAnimator.animations, deathAnim)


	attackAnim := Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.ATTACK],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.ATTACK].frameCount,
		id            = cast(i32)EnemyAnimationID.ATTACK,
		interruptable = false,
		loopable      = false,
	}
	append(&enemyAnimator.animations, attackAnim)


	idleAnim := Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.IDLE],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.IDLE].frameCount,
		id            = cast(i32)EnemyAnimationID.IDLE,
		interruptable = true,
		loopable      = true,
	}
	append(&enemyAnimator.animations, idleAnim)


	moveAnim := Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.MOVE],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.MOVE].frameCount,
		id            = cast(i32)EnemyAnimationID.MOVE,
		interruptable = true,
		loopable      = true,
	}

	append(&enemyAnimator.animations, moveAnim)

	enemyAnimator.currentAnimation = enemyAnimator.animations[cast(i32)EnemyAnimationID.IDLE]

	// animations seem to be valid here
	return enemyAnimator
}

EnemyUpdate :: proc(entity: ^Entity) {
	enemy := &entity.type.(Enemy)
	if (!enemy.dead) {
		if (EnemyTestPlayerHit(entity)) {
			enemy.playerSpotted = true
			if (EnemyFireAtPlayer(entity, enemy.nextFire)) {
				SetAnimation(&enemy.animator, cast(i32)EnemyAnimationID.ATTACK)
			} else {
				if (EnemyUpdatePosition(entity)) {
					SetAnimation(&enemy.animator, cast(i32)EnemyAnimationID.MOVE)
				} else {
					SetAnimation(&enemy.animator, cast(i32)EnemyAnimationID.IDLE)
				}
			}
			enemy.nextFire -= rl.GetFrameTime()
		}
	} else {
		SetAnimation(&enemy.animator, cast(i32)EnemyAnimationID.DEATH)
	}

	if (rl.IsModelAnimationValid(entity.visuals.model, enemy.animator.currentAnimation.animation)) {
		enemy.animator.nextFrame = PlayAnimation(
			&enemy.animator,
			&entity.visuals,
			ENEMY_DEFAULT_ANIMATION_SPEED,
			enemy.animator.nextFrame,
		)
	} else {
		fmt.printfln("Enemy animation %[0]v is not valid!", cast(EnemyAnimationID)enemy.animator.currentAnimation.id)
	}

}

CreateEnemy :: proc(entity: ^Entity) {

	entity.visuals.modelFileName = "./assets/models/enemy.m3d"
	position := rl.Vector3{entity.transform.position.x, ENEMY_POSITION_Y, entity.transform.position.z}

	size := rl.Vector3{0.25, 1.1, 0.25}

	entity.transform.position = position
	entity.transform.rotation = rl.Vector3(0)
	entity.transform.size = size
	entity.transform.scale = 0.5
	entity.transform.boundingBox = utilities.MakeBoundingBox(position, size)


	filename := strings.clone_to_cstring(entity.visuals.modelFileName)
	defer delete(filename)
	entity.visuals.model = rl.LoadModel(filename)

	enemy: Enemy = {
		dead          = false,
		moving        = false,
		attacking     = false,
		playerSpotted = false,
		damage        = 2,
		health        = 15,
		movementSpeed = ENEMY_DEFAULT_MOVEMENT_SPEED,
		rotationSpeed = ENEMY_DEFAULT_ROTATION_SPEED,
		fireRate      = 5.75,
		nextFire      = 5.75,
		animator      = EnemyAnimations(entity.visuals.modelFileName),
	}

	entity.type = enemy

	entity.active = true

}

EnemyTestPlayerHit :: proc(entity: ^Entity) -> bool {
	// TODO: this function can be quite heavy, could give it a tickrate?
	//  every 1-2 seconds instead of every frame

	if (rl.Vector3Distance(Player.transform.position, entity.transform.position) > 5.0 &&
		   entity.type.(Enemy).playerSpotted) {
		return false
	}

	rayCast := CreateRay(entity)

	hitPlayer: bool = false
	levelDistance: f32 = math.F32_MAX
	playerDistance: f32 = math.F32_MAX

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

EnemyUpdatePosition :: proc(entity: ^Entity) -> bool {

	moving: bool = true
	// Move entity towards player
	distanceFromPlayer := entity.transform.position - Player.transform.position

	// Check if player can be seen (first racyast hit returns player)
	// If in certain range from player, stop

	if (math.abs(distanceFromPlayer.x) >= ENEMY_MAX_DISTANCE_FROM_PLAYER ||
		   math.abs(distanceFromPlayer.z) >= ENEMY_MAX_DISTANCE_FROM_PLAYER) {

		entityOldPosition: rl.Vector3 = entity.transform.position
		entityNewPosition: rl.Vector3 = {Player.transform.position.x, ENEMY_POSITION_Y, Player.transform.position.z}
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

EnemyTakeDamage :: proc(entity: ^Entity, damageAmount: i32) {

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

EnemyDestroy :: proc(entity: ^Entity) {

	enemy := &entity.type.(Enemy)
	// HACK: Dirty hack to move bounding box outside of map so it cant be collided to.
	// We want to keep entity in the memory so we can use its position to display the
	// corpse/death anim
	deadBoxPos: rl.Vector3 = rl.Vector3{ENEMY_GRAVEYARD_POSITION, ENEMY_GRAVEYARD_POSITION, ENEMY_GRAVEYARD_POSITION}
	entity.transform.boundingBox = utilities.MakeBoundingBox(deadBoxPos, rl.Vector3(0))
	enemy.dead = true
}

EnemyFireAtPlayer :: proc(entity: ^Entity, nextFire: f32) -> bool {

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

