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
        Model planeFloor;
        Model planeCeiling;
        struct Level_BlockData* blockData;
        struct Enemy_Data* enemies;
        struct Item_Data* items;
        struct Projectile* projectiles;
        Vector3 mapPosition;
        Vector3 startPosition;
        Vector3 endPosition;
        int mapSize;
} Level_Data;

typedef struct Level_BlockData
{
        Vector3 blockPosition;
        Vector3 blockSize;
        BoundingBox blockBoundingBox;
        Model blockModel;
        int modelId;
        bool loaded;
} Level_BlockData;

typedef struct Level_BlockType
{
        Color NONE;
        Color startColor;
        Color endColor;
        Color wallColor;
        Color enemyColor;
} Level_BlockType;

// Variables
extern struct Level_Data Level_data;

// Functions
Camera Level_Initialize();
Mesh Level_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Level_Build();
void Level_Update();
bool Level_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
