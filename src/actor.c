#include "actor.h"

Actor Actor_Add(const char *modelFileName)
{
	unsigned int animationsCount = 0;

	ModelAnimation *loadedAnimations = LoadModelAnimations(modelFileName, &animationsCount);

	Animator_Animation *animations;
	animations = calloc(animationsCount, sizeof(Animator_Animation));

	Animator_Animation deathAnim = { .animation     = loadedAnimations[DEATH],
		                             .firstFrame    = 0,
		                             .lastFrame     = (loadedAnimations[DEATH].frameCount - 5),
		                             .id            = DEATH,
		                             .interruptable = false,
		                             .loopable      = false };

	Animator_Animation attackAnim = { .animation     = loadedAnimations[ATTACK],
		                              .firstFrame    = 0,
		                              .lastFrame     = loadedAnimations[ATTACK].frameCount,
		                              .id            = ATTACK,
		                              .interruptable = false,
		                              .loopable      = false };

	Animator_Animation idleAnim = { .animation     = loadedAnimations[IDLE],
		                            .firstFrame    = 0,
		                            .lastFrame     = loadedAnimations[IDLE].frameCount,
		                            .id            = IDLE,
		                            .interruptable = true,
		                            .loopable      = true };

	Animator_Animation moveAnim = { .animation     = loadedAnimations[MOVE],
		                            .firstFrame    = 0,
		                            .lastFrame     = loadedAnimations[MOVE].frameCount,
		                            .id            = MOVE,
		                            .interruptable = true,
		                            .loopable      = true };

	animations[DEATH]  = deathAnim;
	animations[ATTACK] = attackAnim;
	animations[IDLE]   = idleAnim;
	animations[MOVE]   = moveAnim;

	Animator_Data animator = { .animations       = animations,
		                       .animationsCount  = animationsCount,
		                       .currentAnimation = animations[IDLE],
		                       .nextFrame        = 0 };

	Actor actor = {
		.dead          = false,
		.moving        = false,
		.attacking     = false,
		.damage        = 5,
		.health        = 15, // Check actor health balance later
		.movementSpeed = ACTOR_DEFAULT_MOVEMENT_SPEED,
		.rotationSpeed = ACTOR_DEFAULT_ROTATION_SPEED,
		.fireRate      = 5.75f,
		.nextFire      = 5.75f,
		.animator      = animator
	};

	return actor;
}
