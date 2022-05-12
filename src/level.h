#ifndef LEVEL
#define LEVEL

#include "enemy.h"
#include "include/raylib.h"

#define MAX_LEVEL_SIZE 16384

typedef struct
{
    Vector3 levelBlockPosition;
    Model levelBlockModel;
    int modelId;
} LevelData;

typedef struct
{
    Color NONE;
    Color startColor;
    Color endColor;
    Color wallColor;
    Color enemyColor;
} LevelBlockType;

static LevelBlockType LEVEL_BLOCKS = {.startColor = {0, 255, 0},
                                      .endColor   = {0, 0, 255},
                                      .wallColor  = {255, 255, 255},
                                      .enemyColor = {255, 0, 0},
                                      .NONE       = {0, 0, 0}};

Mesh MakeCustomPlaneMesh(float height, float width, float textureSize);
void BuildLevel();
void DrawLevel();
bool CheckLevelCollision(Vector3 entityPos, Vector3 entitySize, int entityId);
Vector3 GetMapPosition();
Vector3 GetLevelStartPosition();
LevelData* GetLevelData();
Enemy* GetEnemies();
int GetLevelBlockAmount();

#endif