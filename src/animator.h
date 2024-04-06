#pragma once
#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

#include "raylib.h"

// Animation ID's for enemies. Can be in other order for other 3d models.
enum Animator_EnemyAnimationID
{
	ENEMY_DEATH  = 0,
	ENEMY_ATTACK = 1,
	ENEMY_IDLE   = 2,
	ENEMY_MOVE   = 3
};

typedef struct Animator_Animation
{
	ModelAnimation animation;
	int id;
	int firstFrame; // For trimming purposes. Some animations can have extra t-pose time,
	                // etc...
	int lastFrame;  // This means that 0/maxFrameCount is not always the spot where the
	                // animation starts/stops
	bool loopable;
	bool interruptable;
} Animator_Animation;

typedef struct Animator_Data
{
	Animator_Animation *animations;
	unsigned int animationsCount;
	Animator_Animation currentAnimation;
	int animationFrame;
	float nextFrame;
} Animator_Data;

float Animator_PlayAnimation(Animator_Data *animator, Model *model, const float animationSpeed, float nextFrame);
void Animator_SetAnimation(Animator_Data *animator, const int animationId);
Animator_Data Animator_EnemyAnimations(char *modelFileName);

#endif
