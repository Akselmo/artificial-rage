#include"../../include/raylib.h"

void DrawLevel(Model model);

Model BuildLevel();

bool CheckLevelCollision(Vector2 entityPos, float entityRadius);

Vector3 GetMapPosition();

Vector3 GetStartPosition();
