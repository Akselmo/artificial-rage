#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include "actor.h"
#include "entity.h"
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

typedef struct Scene_Data
{
    Model floorPlane;
    Texture2D floorPlaneTexture;
    Model ceilingPlane;
    Texture2D ceilingPlaneTexture;
    struct Entity_Data *entities;
    struct Actor_Data *actors;
    struct Item_Data *items;
    struct Projectile *projectiles;
    Vector3 position;
    Vector3 startPosition;
    Vector3 endPosition;
    int size;
} Scene_Data;

// Variables
extern struct Scene_Data Scene;

// Functions
Camera Scene_Initialize(void);
Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Scene_Build(void);
void Scene_Update(void);
bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
