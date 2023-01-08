#pragma once
#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include "raylib.h"

typedef struct Animator_Animation
{
        ModelAnimation animation;
        int id;
        int firstFrame; // For trimming purposes. Some animations can have extra t-pose time, etc...
        int lastFrame;  // This means that 0/maxFrameCount is not always the spot where the animation starts/stops
        bool loopable;
        bool interruptable;
} Animator_Animation;

typedef struct Animator_Data
{
        Model model;
        Animator_Animation* animations;
        unsigned int animationsCount;
        Animator_Animation currentAnimation;
        int animationFrame;
        float nextFrame;
} Animator_Data;

float Animator_PlayAnimation(Animator_Data * animator, const float animationSpeed, float nextFrame);
void Animator_SetAnimation(Animator_Data * animator, const int animationId);


#endif
