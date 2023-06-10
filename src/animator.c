#include "animator.h"
#include <stdio.h>

void Animator_SetAnimation(Animator_Data *animator, const int animationId)
{
	const Animator_Animation currentAnimation = animator->currentAnimation;
	const Animator_Animation newAnimation = animator->animations[animationId];

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

	animator->animationFrame = currentAnimation.firstFrame;
	animator->currentAnimation = newAnimation;
}

// TODO: This would need somekind of frameskip feature if FPS is lower than animationspeed. Ideas welcome!
float Animator_PlayAnimation(Animator_Data *animator, const float animationSpeed, float nextFrame)
{

	if (nextFrame > 0)
	{
		nextFrame -= GetFrameTime();
	}
	else
	{
		Animator_Animation currentAnimation = animator->currentAnimation;
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
		UpdateModelAnimation(animator->model, currentAnimation.animation, animator->animationFrame);
		nextFrame = 1.0 / animationSpeed;
	}
	return nextFrame;
}