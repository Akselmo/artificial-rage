#pragma once
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "enemy.h"
#include "item.h"
#include "main.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL_SIZE 16384

// Structs

typedef struct Level_Data
{
    Vector3 blockPosition;
    Model blockModel;
    int modelId;
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
extern Vector3 Level_mapPosition;
extern Vector3 Level_startPosition;
extern Vector3 Level_endPosition;
extern int Level_mapSize;

// Functions
Mesh Level_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Level_Build();
void Level_Draw();
bool Level_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
