#pragma once
#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include "raylib.h"

typedef struct Animator_Animation
{
        ModelAnimation animation;
        bool loopable;
        bool interruptable;
} Animator_Animation;

typedef struct Animator_Data
{
        Animator_Animation* animations;
        unsigned int animationsCount;
        int currentAnimation;
        int animationFrame;
} Animator_Data;

#endif
