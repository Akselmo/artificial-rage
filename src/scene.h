#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include "actor.h"
#include "game.h"
#include "item.h"
#include "main.h"
#include "player.h"
#include "projectile.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCENE_SIZE        16384
#define MAX_PROJECTILE_AMOUNT 254
#define WALL_MODEL_ID         -2

// Structs
typedef struct Scene_Data
{
        Model floorPlane;
        Model ceilingPlane;
        struct Scene_BlockData* blocks;
        struct Actor_Data* actors;
        struct Item_Data* items;
        struct Projectile* projectiles;
        Vector3 position;
        Vector3 startPosition;
        Vector3 endPosition;
        int size;
} Scene_Data;

typedef struct Scene_BlockData
{
        Vector3 position;
        Vector3 size;
        BoundingBox boundingBox;
        Model model;
        int id;
        bool loaded;
} Scene_BlockData;

typedef struct Level_BlockType
{
        Color NONE;
        Color startColor;
        Color endColor;
        Color wallColor;
        Color actorColor;
} Level_BlockType;

// Variables
extern struct Scene_Data Scene_data;

// Functions
Camera Scene_Initialize(void);
Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Scene_Build(void);
void Scene_Update(void);
bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
