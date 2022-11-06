#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "level.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>

#define ENEMY_START_POSITION_Y         0.4f
#define ENEMY_GRAVEYARD_POSITION       999.0f
#define ENEMY_MAX_DISTANCE_FROM_PLAYER 1.25f
#define ENEMY_DEFAULT_MOVEMENT_SPEED   0.45f  // Lower values result to lower speed
#define ENEMY_DEFAULT_ROTATION_SPEED   3.0f
#define ENEMY_ANIMATION_COUNT          4

// Example: https://www.raylib.com/examples/models/loader.html?name=models_animation
typedef struct Enemy_Model
{
        Model model;                 // LoadModel
        ModelAnimation* animations;  // LoadAnimations
        int animationFrame;
        enum AnimationID currentAnimation;
} Enemy_Model;

typedef struct Enemy_Data
{
        int health;
        int damage;
        Vector3 position;
        Vector3 rotation;
        Vector3 size;
        BoundingBox boundingBox;
        struct Enemy_Model model;
        bool dead;
        int id;
        float tickRate;
        float nextTick;
        float movementSpeed;
        float rotationSpeed;
        float fireRate;
        float nextFire;
} Enemy_Data;

Enemy_Data Enemy_Add(float pos_x, float pos_y, int id);
void Enemy_Update(Enemy_Data* enemy);
void Enemy_Draw(Enemy_Data* enemy);
void Enemy_TakeDamage(Enemy_Data* enemy, int damageAmount);
void Enemy_RotateTowards(Enemy_Data* enemy, Vector3 targetPosition);

#endif