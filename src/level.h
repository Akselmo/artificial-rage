#ifndef LEVEL
#define LEVEL

#include "include/raylib.h"
#include "enemy.h"

#define MAX_LEVEL_SIZE 16384

typedef struct
{
    Vector3 levelBlockPosition;
    Model levelBlockModel;
    int modelId;
} LevelData;

Mesh MakeCustomPlaneMesh(float height, float width, float textureSize);

void BuildLevel();

void DrawLevel();

bool CheckLevelCollision(Vector3 entityPos, Vector3 entitySize, int entityId);

Vector3 GetMapPosition();

Vector3 GetLevelStartPosition();

LevelData *GetLevelData();

Enemy *GetEnemies();

BoundingBox MakeBoundingBox(Vector3 position, Vector3 size);

int GetLevelBlockAmount();

#endif
