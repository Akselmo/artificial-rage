#pragma once
#include "item.h"
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "raylib.h"
#include "entity.h"

// Remember to update this if you add more entities in entity.c
#define ENTITIES_TOTAL 7

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
typedef struct Entity_Data
{
	int id;
	Vector3 position;
	Vector3 size;
	enum Entity_Type type;
	char *fileName;
	BoundingBox boundingBox;
	Model model;
	Item_Data item;
	bool loaded;
} Entity_Data;

extern Entity_Data *Entity_list[ENTITIES_TOTAL];
extern Entity_Data Entity_none;
extern Entity_Data Entity_start;
extern Entity_Data Entity_end;
extern Entity_Data Entity_wall1;
extern Entity_Data Entity_wall2;
extern Entity_Data Entity_enemy;
extern Entity_Data Entity_item;

void Entity_InitList(void);

#endif
