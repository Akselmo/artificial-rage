#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include "entity.h"
#include "game.h"

#include "main.h"
#include "player.h"
#include "projectile.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROJECTILE_AMOUNT 254

typedef struct Scene
{
	char *name;
	int height;
	int width;
	int *data;
	int dataCount;
	int ceilingHeight;
	Model floorPlane;
	Texture2D floorPlaneTexture;
	Model ceilingPlane;
	Texture2D ceilingPlaneTexture;
	int entitiesTotal; // entitiesTotal = (last_entity_id + 1) in entities.json
	struct Entity *entities;
	struct Projectile *projectiles;
	Vector3 position;
	Vector3 startPosition;
	Vector3 endPosition;
	int size;
} Scene;

// Variables
extern struct Scene *scene;

// Functions
Camera Scene_Initialize(void);
Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize);
void Scene_Build(void);
void Scene_Update(void);
bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

#endif
