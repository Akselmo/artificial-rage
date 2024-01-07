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
#define ITEM_START_POSITION_Y 0.4f

// Types of entities there are
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

// Animation ID's for enemies. Can be in other order for other 3d models.
enum AnimationID
{
	DEATH  = 0,
	ATTACK = 1,
	IDLE   = 2,
	MOVE   = 3
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
	int value;
} Item;

typedef struct Entity_Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 size;
	float scale;
	BoundingBox boundingBox;
} Entity_Transform;

typedef struct Entity_Model
{
	char *fileName;
	char *textureFileName;
	Model data;
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
	bool loaded;
	Entity_Transform transform;
	Entity_Model model;
	Entity_Data data;
} Entity;

// Control functions
void Entity_Update(Entity *entity);
void Entity_Draw(Entity *entity);
void Entity_TakeDamage(Entity *entity, int damageAmount);
void Entity_RotateTowards(Entity *entity, Vector3 targetPosition);

// Creation functions
Entity Entity_Create(const enum Entity_Type type, const Vector3 position, const int id);

#endif
