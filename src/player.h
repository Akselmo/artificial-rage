#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "entity.h"
#include "raylib.h"

#define PLAYER_ID -1
#define PLAYER_MAX_HEALTH 20
#define PLAYER_CAMERA_MIN_CLAMP 89.0f
#define PLAYER_CAMERA_MAX_CLAMP -89.0f
#define PLAYER_CAMERA_PANNING_DIVIDER 5.1f
#define PLAYER_START_POSITION_Y 0.4f

// Struct for all the camera data
typedef struct Player_CustomCameraData
{
	float targetDistance;
	float playerEyesPosition;
	Vector2 angle;
	float mouseSensitivity;
	float playerSpeed;
} Player_CustomCameraData;

typedef struct Player_Data
{
	int health;
	bool dead;
	Entity_Transform transform;
	float nextFire;
} Player_Data;

// Public variables
extern struct Player_Data *Player;

// Public functions
Camera Player_InitializeCamera(float pos_x, float pos_z);
void Player_Update(Camera *camera);
void Player_SetHealth(int healthToAdd);
void Player_FireWeapon(Camera *camera, Player_CustomCameraData *cameraData);

// Movement keys enum for directions
typedef enum Player_CameraMoveKeys
{
	MOVE_FRONT = 0,
	MOVE_BACK  = 1,
	MOVE_RIGHT = 2,
	MOVE_LEFT  = 3
} Player_CameraMoveKeys;

#endif
