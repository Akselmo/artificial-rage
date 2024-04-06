#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "animator.h"
#include "raylib.h"
// Defines
#define WALL_MODEL_ID -2
#define ACTOR_POSITION_Y 0.0f
#define ACTOR_GRAVEYARD_POSITION 999.0f
#define ACTOR_MAX_DISTANCE_FROM_PLAYER 1.25f
#define ACTOR_DEFAULT_MOVEMENT_SPEED 0.45f // Lower values result to lower speed
#define ACTOR_DEFAULT_ROTATION_SPEED 3.0f
#define ACTOR_DEFAULT_ANIMATION_SPEED 30.0f // Animation played in FPS
#define ITEM_START_POSITION_Y 0.1f

// Types of entities there are
// TODO: make this more inline with tiled map template so
// that the values always match..
enum Entity_Type
{
	ENTITY_NONE,  // This is ignored in entities.json
	ENTITY_START, // Tile map template index is 0. Here index is 1.
	ENTITY_END,
	ENTITY_WALL_CARGO,
	ENTITY_WALL_CARGO_SCUFFED,
	ENTITY_ENEMY_DEFAULT,
	ENTITY_ITEM_HEALTH_SMALL,
	ENTITY_ITEM_HEALTH_MEDIUM,
	ENTITY_ITEM_HEALTH_LARGE,
	ENTITY_ITEM_CLUTTER,       // an item that provides collision but nothing else
	ENTITY_ITEM_PICKUP_PISTOL, // Gives weapon and some ammo
	ENTITY_ITEM_PICKUP_RIFLE,
	ENTITY_ITEM_PICKUP_SHOTGUN,
	ENTITY_ITEM_PICKUP_RAILGUN,
	ENTITY_ITEM_AMMO_PISTOL, // Gives only ammo
	ENTITY_ITEM_AMMO_RIFLE,
	ENTITY_ITEM_AMMO_SHOTGUN,
	ENTITY_ITEM_AMMO_RAILGUN,
	ENTITY_ITEM_KEY_TELEPORT // Needed to go through ending teleporter
};

// Actors, in this case hostile NPCs. Could be extended for friendlies too!
typedef struct Actor
{
	int health;
	int damage;
	bool dead;
	bool moving;
	bool attacking;
	bool playerSpotted;
	float movementSpeed;
	float rotationSpeed;
	float fireRate;
	float nextFire;
	Animator_Data animator;
	// TODO: actually utilize the tickrate
	float tickRate;
} Actor;

// Items such as health pickups, ammo, etc..
typedef struct Item
{
	bool destroyed;
	bool pickup;
	int value;
} Item;

typedef struct Entity_Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 size;
	float scale;
	BoundingBox boundingBox;
	bool canCollide;
} Entity_Transform;

typedef struct Entity_Model
{
	char *fileName;
	char *textureFileName;
	Model data;
	bool isBillboard;
} Entity_Model;

typedef union Entity_Union
{
	struct Item item;
	struct Actor actor;
} Entity_Union;

typedef struct Entity_Data
{
	enum Entity_Type type;
	union Entity_Union value;
} Entity_Data;

// This is basically a "box" that holds all entities,
// such as actors, scene building blocks, items..
// When the engine finds an entity, it checks what entity it is
// and then handles it accordingly
typedef struct Entity
{
	int id;
	bool active;
	Entity_Transform transform;
	Entity_Model model;
	Entity_Data data;
} Entity;

// Control functions
void Entity_Update(Entity *entity);
void Entity_Draw(Entity *entity);
void Entity_TakeDamage(Entity *entity, int damageAmount);
void Entity_RotateTowards(Entity *entity, Vector3 targetPosition);
void Entity_HandlePlayerPickup(Entity *entity);
// Creation functions
Entity Entity_Create(const enum Entity_Type type, const Vector3 position, const int id);

#endif
