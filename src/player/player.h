#ifndef PLAYER
#define PLAYER

#include "../../include/raylib.h"

Camera CustomFPSCamera(float pos_x, float pos_z);

void UpdateFPSCamera(Camera *camera);

bool CheckCollision(Camera camera, BoundingBox other);

int GetHealth();

void SetHealth(int healthToAdd);

void PlayerFire(Camera *camera);

#endif