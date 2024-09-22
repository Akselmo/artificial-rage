package entity
import "core:fmt"
import "core:strings"
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
	enemyAnimationsCount: i32 = 0
	enemyLoadedAnimations := rl.LoadModelAnimations(cstringName, &enemyAnimationsCount)
	enemyAnimations: [4]Animation

	enemyAnimations[EnemyAnimationID.DEATH] = Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.DEATH],
		firstFrame    = 0,
		lastFrame     = (enemyLoadedAnimations[EnemyAnimationID.DEATH].frameCount - 5),
		id            = cast(i32)EnemyAnimationID.DEATH,
		interruptable = false,
		loopable      = false,
	}

	enemyAnimations[EnemyAnimationID.ATTACK] = Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.ATTACK],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.ATTACK].frameCount,
		id            = cast(i32)EnemyAnimationID.ATTACK,
		interruptable = false,
		loopable      = false,
	}

	enemyAnimations[EnemyAnimationID.IDLE] = Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.IDLE],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.IDLE].frameCount,
		id            = cast(i32)EnemyAnimationID.IDLE,
		interruptable = true,
		loopable      = true,
	}

	enemyAnimations[EnemyAnimationID.MOVE] = Animation {
		animation     = enemyLoadedAnimations[EnemyAnimationID.MOVE],
		firstFrame    = 0,
		lastFrame     = enemyLoadedAnimations[EnemyAnimationID.MOVE].frameCount,
		id            = cast(i32)EnemyAnimationID.MOVE,
		interruptable = true,
		loopable      = true,
	}

	enemyAnimator: Animator = Animator {
		animations       = enemyAnimations[:],
		animationsCount  = 4,
		currentAnimation = enemyAnimations[EnemyAnimationID.IDLE],
		nextFrame        = 0.0,
	}

	return enemyAnimator
}

EnemyUpdate :: proc(entity: ^Entity) {
	enemy := &entity.data.value.(Enemy)
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
	enemy.animator.nextFrame = PlayAnimation(
		&enemy.animator,
		&entity.model.data,
		ACTOR_DEFAULT_ANIMATION_SPEED,
		enemy.animator.nextFrame,
	)

}

