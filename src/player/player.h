#include"../../include/raylib.h"

Camera CustomFPSCamera(float pos_x, float pos_z);

void UpdateFPSCamera(Camera *camera);

void DrawPlayerHitbox(Camera camera);

bool CheckCollision(Camera camera, BoundingBox other);