#include"../../include/raylib.h"

#define MAX_LEVEL_SIZE 16384

typedef struct {
    Vector3 levelBlockPosition;
    Model levelBlockModel;
    int modelId;
} LevelData;

void DrawLevel();

void BuildLevel();

bool CheckLevelCollision(Vector2 entityPos, float entityRadius);

Vector3 GetMapPosition();

Vector3 GetStartPosition();

LevelData* GetLevelData();