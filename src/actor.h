#pragma once
#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "animator.h"
#include "player.h"
#include "projectile.h"
#include "raylib.h"
#include "raymath.h"
#include "scene.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>

#define ACTOR_POSITION_Y               0.0f
#define ACTOR_GRAVEYARD_POSITION       999.0f
#define ACTOR_MAX_DISTANCE_FROM_PLAYER 1.25f
#define ACTOR_DEFAULT_MOVEMENT_SPEED   0.45f  // Lower values result to lower speed
#define ACTOR_DEFAULT_ROTATION_SPEED   3.0f

enum AnimationID
{
    DEATH  = 0,
    ATTACK = 1,
    IDLE   = 2,
    MOVE   = 3
};

// Example: https://www.raylib.com/examples/models/loader.html?name=models_animation
// TODO: Clean up structs, make them into smaller pieces?
typedef struct Actor_Data
{
        int health;
        int damage;
        Vector3 position;
        Vector3 rotation;
        Vector3 size;
        BoundingBox boundingBox;
        Animator_Data animator;
        bool dead;
        bool moving;
        bool attacking;
        int id;
        float tickRate;
        float nextTick;
        float movementSpeed;
        float rotationSpeed;
        float fireRate;
        float nextFire;
} Actor_Data;

Actor_Data Actor_Add(float pos_x, float pos_z, int id, const char* modelFileName);
void Actor_Update(Actor_Data* actor);
void Actor_Draw(Actor_Data* actor);
void Actor_TakeDamage(Actor_Data* actor, int damageAmount);
void Actor_RotateTowards(Actor_Data* actor, Vector3 targetPosition);

#endif
