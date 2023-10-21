#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "raylib.h"
#include "item.h"
#include "actor.h"
#include "player.h"

// Remember to update this if you add more entities in entity.c
#define WALL_MODEL_ID -2

enum Entity_Type
{
	ENTITY_NONE,
	ENTITY_START,
	ENTITY_END,
	ENTITY_WALL_CARGO,
	ENTITY_WALL_CARGO_SCUFFED,
	ENTITY_ENEMY_DEFAULT,
	ITEM_HEALTH_SMALL,
	ITEM_HEALTH_MEDIUM,
	ITEM_HEALTH_LARGE,
	ITEM_CLUTTER, // an item that provides collision but nothing else
	ITEM_PISTOL,  // Gives weapon and some ammo
	ITEM_RIFLE,
	ITEM_SHOTGUN,
	ITEM_RAILGUN,
	ITEM_PISTOL_AMMO, // Gives only ammo
	ITEM_RIFLE_AMMO,
	ITEM_SHOTGUN_AMMO,
	ITEM_RAILGUN_AMMO,
	ITEM_TELEPORT_KEY // Needed to go through ending teleporter
};

// This is basically a "box" that holds all entities,
// such as actors, scene building blocks, items..
// When the engine finds an entity, it checks what entity it is
// and then handles it accordingly
// TODO: Look into this! could just have one array of all entities, and update only that?
// Then entity could have a struct matching it's type in it, which is used for the corresponding commands
// Wouldn't need to have multiple arrays for items, enemies etc..

typedef struct Entity
{
	int id;
	bool loaded;
	Vector3 position;
	Vector3 rotation;
	Vector3 size;
	float scale;
	BoundingBox boundingBox;
	char *modelFileName;
	char *textureFileName;
	Model model;
	enum Entity_Type type;
	// Only one of these is used per initialization
	struct Item item;
	struct Actor actor;
} Entity;

// Control functions
void Entity_Update(Entity *entity);
void Entity_Draw(Entity *entity);
void Entity_TakeDamage(Entity *entity, int damageAmount);
void Entity_RotateTowards(Entity *entity, Vector3 targetPosition);

// Creation functions
Entity Entity_Create(const enum Entity_Type type, const Vector3 position, const int id);

#endif
