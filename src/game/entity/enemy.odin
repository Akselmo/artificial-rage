package entity
import "core:fmt"
import "core:strings"
import "src:game/utilities"
import rl "vendor:raylib"

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
		if (TestPlayerHit(entity)) {
			enemy.playerSpotted = true
			if (FireAtPlayer(entity, enemy.nextFire)) {
				SetAnimation(&enemy.animator, cast(i32)EnemyAnimationID.ATTACK)
			} else {
				if (UpdatePosition(entity)) {
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
			ACTOR_DEFAULT_ANIMATION_SPEED,
			enemy.animator.nextFrame,
		)
	} else {
		fmt.printfln("Enemy animation %[0]v is not valid!", cast(EnemyAnimationID)enemy.animator.currentAnimation.id)
	}

}

CreateEnemy :: proc(entity: ^Entity) {

	entity.visuals.modelFileName = "./assets/models/enemy.m3d"
	position := rl.Vector3{entity.transform.position.x, ACTOR_POSITION_Y, entity.transform.position.z}

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
		movementSpeed = ACTOR_DEFAULT_MOVEMENT_SPEED,
		rotationSpeed = ACTOR_DEFAULT_ROTATION_SPEED,
		fireRate      = 5.75,
		nextFire      = 5.75,
		animator      = EnemyAnimations(entity.visuals.modelFileName),
	}

	entity.type = enemy

	entity.active = true

}

