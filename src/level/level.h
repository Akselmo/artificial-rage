#include"../../include/raylib.h"

#define MAX_LEVEL_SIZE 16384

typedef struct {
    Vector3 levelBlockPosition;
    Model levelBlockModel;
    int modelId;
} LevelData;
Mesh MakeCustomPlaneMesh(float height, float width, float textureSize);
void DrawLevel();

void BuildLevel();

bool CheckLevelCollision(Vector3 entityPos, Vector3 entitySize);

Vector3 GetMapPosition();

Vector3 GetStartPosition();

LevelData* GetLevelData();

int GetLevelBlockAmount();

