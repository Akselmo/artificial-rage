#pragma once

#include "raylib.h"

// These are mostly helper functions for animations. This can be eventually turned into
// a class, but for now just using namespace for it is sufficient.
namespace Animator
{
// Animation ID's for enemies. Can be in other order for other 3d models.
enum EnemyAnimationID
{
	ENEMY_DEATH  = 0,
	ENEMY_ATTACK = 1,
	ENEMY_IDLE   = 2,
	ENEMY_MOVE   = 3
};

struct Animation
{
	ModelAnimation animation;
	int id;
	int firstFrame; // For trimming purposes. Some animations can have extra t-pose time,
	                // etc...
	int lastFrame;  // This means that 0/maxFrameCount is not always the spot where the
	                // animation starts/stops
	bool loopable;
	bool interruptable;
};

struct Data
{
	Animation *animations;
	unsigned int animationsCount;
	Animation currentAnimation;
	int animationFrame;
	float nextFrame;
};

float playAnimation(Data *animator, Model *model, const float animationSpeed, float nextFrame);
void setAnimation(Data *animator, const int animationId);
Data enemyAnimations(char *modelFileName);
}; // namespace Animator
