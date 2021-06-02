#include"../../include/raylib.h"


typedef struct {
    Vector3 levelBlockPosition;
    Model levelBlockModel;
} LevelData;

void DrawLevel();

void BuildLevel();

bool CheckLevelCollision(Vector2 entityPos, float entityRadius);

Vector3 GetMapPosition();

Vector3 GetStartPosition();
