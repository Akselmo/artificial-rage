#include "../../include/raylib.h"
#include "enemy.h"

#define ENEMY_START_POSITION_Y 0.4f

Vector3 enemyPosition;
Vector3 enemySize;

//Since we use billboarding we dont have to know rotation

void AddEnemy(float pos_x, float pos_y)
{
    enemyPosition = (Vector3){pos_x, ENEMY_START_POSITION_Y, pos_y};
    enemySize = (Vector3){0.25f, 0.25f, 0.25f};
}

void DrawEnemy()
{
    DrawCubeV(enemyPosition, enemySize, RED);
}