#include "animator.h"
#include <stdlib.h>

void Animator::setAnimation(Animator::Data *animator, const int animationId)
{
	const Animator::Animation currentAnimation = animator->currentAnimation;
	const Animator::Animation newAnimation     = animator->animations[animationId];

	if (currentAnimation.id == newAnimation.id)
	{
		return;
	}

	if (!currentAnimation.interruptable)
	{
		if (animator->animationFrame < currentAnimation.lastFrame)
		{
			return;
		}
	}

	animator->animationFrame   = currentAnimation.firstFrame;
	animator->currentAnimation = newAnimation;
}

// TODO: This would need somekind of frameskip feature if FPS is lower than animationspeed. Ideas welcome!
float Animator::playAnimation(Animator::Data *animator, Model *model, const float animationSpeed, float nextFrame)
{

	if (nextFrame > 0)
	{
		nextFrame -= GetFrameTime();
	}
	else
	{
		Animator::Animation currentAnimation = animator->currentAnimation;
		animator->animationFrame += 1;
		if (animator->animationFrame > currentAnimation.lastFrame)
		{
			if (currentAnimation.loopable)
			{
				animator->animationFrame = currentAnimation.firstFrame;
			}
			else
			{
				animator->animationFrame = currentAnimation.lastFrame;
			}
		}
		UpdateModelAnimation(*model, currentAnimation.animation, animator->animationFrame);
		nextFrame = 1.0 / animationSpeed;
	}
	return nextFrame;
}

Animator::Data Animator::enemyAnimations(char *modelFileName)
{
	int animationsCount              = 0;
	ModelAnimation *loadedAnimations = LoadModelAnimations(modelFileName, &animationsCount);
	Animator::Animation *animations  = (Animator::Animation *)calloc(animationsCount, sizeof(Animator::Animation));

	// TODO: Could move this to animator.c since its more of its thing
	animations[ENEMY_DEATH] = (Animator::Animation){ .animation     = loadedAnimations[ENEMY_DEATH],
		                                             .firstFrame    = 0,
		                                             .lastFrame     = (loadedAnimations[ENEMY_DEATH].frameCount - 5),
		                                             .id            = ENEMY_DEATH,
		                                             .interruptable = false,
		                                             .loopable      = false };

	animations[ENEMY_ATTACK] = (Animator::Animation){ .animation     = loadedAnimations[ENEMY_ATTACK],
		                                              .firstFrame    = 0,
		                                              .lastFrame     = loadedAnimations[ENEMY_ATTACK].frameCount,
		                                              .id            = ENEMY_ATTACK,
		                                              .interruptable = false,
		                                              .loopable      = false };

	animations[ENEMY_IDLE] = (Animator::Animation){ .animation     = loadedAnimations[ENEMY_IDLE],
		                                            .firstFrame    = 0,
		                                            .lastFrame     = loadedAnimations[ENEMY_IDLE].frameCount,
		                                            .id            = ENEMY_IDLE,
		                                            .interruptable = true,
		                                            .loopable      = true };

	animations[ENEMY_MOVE] = (Animator::Animation){ .animation     = loadedAnimations[ENEMY_MOVE],
		                                            .firstFrame    = 0,
		                                            .lastFrame     = loadedAnimations[ENEMY_MOVE].frameCount,
		                                            .id            = ENEMY_MOVE,
		                                            .interruptable = true,
		                                            .loopable      = true };

	Animator::Data data = (Animator::Data){ .animations       = animations,
		                                    .animationsCount  = static_cast<unsigned int>(animationsCount),
		                                    .currentAnimation = animations[ENEMY_IDLE],
		                                    .nextFrame        = 0 };

	return data;
}
