#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "raylib.h"
#include "item.h"
#include "actor.h"
#include "player.h"

// Remember to update this if you add more entities in entity.c
#define ENTITIES_TOTAL 7
#define WALL_MODEL_ID -2

enum Entity_Type
{
	SCENE_NONE,
	SCENE_START,
	SCENE_END,
	SCENE_WALL,
	SCENE_ACTOR,
	SCENE_ITEM
};

// This is basically a "box" that holds all entities,
// such as actors, scene building blocks, items..
// When the engine finds an entity, it checks what entity it is
// and then handles it accordingly
// TODO: Look into this! could just have one array of all entities, and update only that?
// Then entity could have a struct matching it's type in it, which is used for the corresponding commands
// Wouldn't need to have multiple arrays for items, enemies etc..
typedef struct EntityTemplate
{
	enum Entity_Type type;
	char *modelFileName;
	char *textureFileName;
} EntityTemplate;

typedef struct Entity
{
	int id;
	bool loaded;
	Vector3 position;
	Vector3 rotation;
	Vector3 size;
	float scale;
	BoundingBox boundingBox;
	char *fileName;
	Model model;
	enum Entity_Type type;
	// Only one of these is used per initialization
	struct Item item;
	struct Actor actor;
} Entity;

extern struct EntityTemplate *EntityTemplate_list[ENTITIES_TOTAL];
extern struct EntityTemplate EntityTemplate_none;
extern struct EntityTemplate EntityTemplate_start;
extern struct EntityTemplate EntityTemplate_end;
extern struct EntityTemplate EntityTemplate_wall1;
extern struct EntityTemplate EntityTemplate_wall2;
extern struct EntityTemplate EntityTemplate_enemy;
extern struct EntityTemplate EntityTemplate_item;

// Control functions
void Entity_InitList(void);
void Entity_Update(Entity *entity);
void Entity_Draw(Entity *entity);
void Entity_TakeDamage(Entity *entity, int damageAmount);
void Entity_RotateTowards(Entity *entity, Vector3 targetPosition);

// Creation functions
Entity Entity_CreateEnemy(const Vector3 position, const int id, const char *modelFileName);
Entity Entity_CreateWall(const char *textureFileName, const Vector3 position);

#endif
