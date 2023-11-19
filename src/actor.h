#pragma once
#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "animator.h"

#include <stdio.h>
#include <stdlib.h>

#define ACTOR_POSITION_Y 0.0f
#define ACTOR_GRAVEYARD_POSITION 999.0f
#define ACTOR_MAX_DISTANCE_FROM_PLAYER 1.25f
#define ACTOR_DEFAULT_MOVEMENT_SPEED 0.45f // Lower values result to lower speed
#define ACTOR_DEFAULT_ROTATION_SPEED 3.0f
#define ACTOR_DEFAULT_ANIMATION_SPEED 30.0f // Animation played in FPS

enum AnimationID
{
	DEATH  = 0,
	ATTACK = 1,
	IDLE   = 2,
	MOVE   = 3
};

// Example: https://www.raylib.com/examples/models/loader.html?name=models_animation
// TODO: Clean up structs, make them into smaller pieces?
typedef struct Actor
{
	int health;
	int damage;
	bool dead;
	bool moving;
	bool attacking;
	bool playerSpotted;
	float movementSpeed;
	float rotationSpeed;
	float fireRate;
	float nextFire;
	Animator_Data animator;
	// TODO: add tickrate and handle it
} Actor;

Actor Actor_CreateEnemy(const char *modelFileName, int health, int damage);

#endif
