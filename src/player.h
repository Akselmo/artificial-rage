#ifndef PLAYER
#define PLAYER

#include "include/raylib.h"

#define PLAYER_ID -1

Camera CustomFPSCamera(float pos_x, float pos_z);

void UpdateFPSCamera(Camera *camera);

bool CheckCollision(Camera camera, BoundingBox other);

int GetHealth();

void SetHealth(int healthToAdd);

void PlayerFire(Camera *camera);

BoundingBox GetPlayerBoundingBox();

Vector3 GetPlayerPosition();

#endif
