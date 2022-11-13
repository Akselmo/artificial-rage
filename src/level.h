#pragma once
#include "projectile.h"
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "enemy.h"
#include "item.h"
#include "main.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL_SIZE        16384
#define MAX_PROJECTILE_AMOUNT 254
#define WALL_MODEL_ID         -2

// Structs

typedef struct Level_Data
{
        Vector3 blockPosition;
        Vector3 blockSize;
        BoundingBox blockBoundingBox;
        Model blockModel;
        int modelId;
        bool loaded;
} Level_Data;

typedef struct Level_BlockType
{
        Color NONE;
        Color startColor;
        Color endColor;
        Color wallColor;
        Color enemyColor;
} Level_BlockType;

// Variables
extern struct Level_Data* Level_data;
extern struct Enemy_Data* Level_enemies;
extern struct Item_Data* Level_items;
extern struct Projectile* Level_projectiles;
extern Vector3 Level_mapPosition;
extern Vector3 Level_startPosition;
extern Vector3 Level_endPosition;
extern int Level_mapSize;

// Functions
Mesh Level_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Level_Build();
void Level_Update();
bool Level_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
