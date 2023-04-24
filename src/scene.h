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

#define MAX_SCENE_SIZE 16384
#define MAX_PROJECTILE_AMOUNT 254
#define WALL_MODEL_ID -2
#define BLOCKS_TOTAL 6

enum Scene_EntityType
{
    SCENE_NONE,
    SCENE_START,
    SCENE_END,
    SCENE_WALL,
    SCENE_ACTOR,
};

typedef struct Scene_Data
{
    Model floorPlane;
    Texture2D floorPlaneTexture;
    Model ceilingPlane;
    Texture2D ceilingPlaneTexture;
    struct Scene_EntityData *entities;
    struct Actor_Data *actors;
    struct Item_Data *items;
    struct Projectile *projectiles;
    Vector3 position;
    Vector3 startPosition;
    Vector3 endPosition;
    int size;
} Scene_Data;

typedef struct Scene_EntityData
{
    Vector3 position;
    Vector3 size;
    BoundingBox boundingBox;
    Model model;
    int id;
    bool loaded;
} Scene_EntityData;

typedef struct Scene_Entity
{
    Color mapColor;            // The color indicated in the level file, that is assigned to this type of block.
    enum Scene_EntityType type; // The type of the block. Type affects to the behavior of the block.
    char *fileName;            // The name of the file to be loaded. Handled differently per type.
} Scene_Entity;

// Variables
extern struct Scene_Data Scene;

// Functions
Camera Scene_Initialize(void);
Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Scene_Build(void);
void Scene_Update(void);
bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
