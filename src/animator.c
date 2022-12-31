#include "animator.h"
#include <stdio.h>
void Animator_SetAnimation(Animator_Data * animator, const int animationId)
{
    const Animator_Animation currentAnimation = animator->currentAnimation;
    const Animator_Animation newAnimation = animator->animations[animationId];

    if (currentAnimation.id == newAnimation.id)
    {
        return;
    }

    if (!currentAnimation.interruptable)
    {
        if (animator->animationFrame < currentAnimation.animation.frameCount)
        {
            return;
        }
    }

    animator->animationFrame = 0;
    animator->currentAnimation = newAnimation;
}

void Animator_PlayAnimation(Animator_Data* animator, const float animationSpeed)
{

    Animator_Animation currentAnimation = animator->currentAnimation;
    animator->animationFrame += 1;
    if(animator->animationFrame > currentAnimation.animation.frameCount)
    {
        if (currentAnimation.loopable)
        {
            animator->animationFrame = 0;
        }
        else
        {
            // FIXME: This does not stop at the last frame of the animation
            animator->animationFrame = currentAnimation.animation.frameCount;
        }
    }

    UpdateModelAnimation(animator->model,currentAnimation.animation,animator->animationFrame);

}