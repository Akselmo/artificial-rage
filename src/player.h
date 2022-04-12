#ifndef PLAYER
#define PLAYER

#include "include/raylib.h"

#define PLAYER_ID -1
#define MAX_HEALTH 20

// Struct for all the camera data
typedef struct
{
    float targetDistance;
    float playerEyesPosition;
    Vector2 angle;
    int moveFront, moveBack, moveRight, moveLeft;
    float mouseSensitivity;
    float playerSpeed;
} CameraData;
typedef struct
{
    int playerHealth;
    bool playerDead;
} PlayerData;
static CameraData CAMERA = {
    .targetDistance = 0,
    .playerEyesPosition = 1.85f,
    .angle = {0},
    .mouseSensitivity = 0.003f,
    .playerSpeed = 30.0f,
};
static PlayerData PLAYERDATA = {
    .playerHealth = MAX_HEALTH,
    .playerDead = false,
};
// Movement keys enum for directions
typedef enum
{
    MOVE_FRONT = 0,
    MOVE_BACK = 1,
    MOVE_RIGHT = 2,
    MOVE_LEFT = 3
} CameraMove;
Camera CustomFPSCamera(float pos_x, float pos_z);
void UpdateFPSCamera(Camera* camera);
bool CheckCollision(Camera camera, BoundingBox other);
void PlayerSetHealth(int healthToAdd);
void PlayerFire(Camera* camera);
BoundingBox GetPlayerBoundingBox();
Vector3 GetPlayerPosition();
PlayerData GetPlayerData();

#endif
