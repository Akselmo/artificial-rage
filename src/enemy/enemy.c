#include "../../include/raylib.h"
#include "enemy.h"

#define ENEMY_START_POSITION_Y 0.4f

Vector3 enemyPosition;
Vector3 enemySize;

//Since we use billboarding we dont have to know rotation

Enemy AddEnemy(float pos_x, float pos_y)
{
    enemyPosition = (Vector3){pos_x, ENEMY_START_POSITION_Y, pos_y};
    enemySize = (Vector3){0.25f, 0.25f, 0.25f};
    Enemy enemy = {
        .position = enemyPosition,
        .size = enemySize,
        .dead = false,
        .damage = 5,
        .health = 50,
    };
    return enemy;
}

void UpdateEnemy(Enemy enemy)
{
    DrawEnemy(enemy);
    UpdateEnemyCollision(enemy);
}

void DrawEnemy(Enemy enemy)
{
    DrawCubeV(enemy.position, enemy.size, RED);
}

void UpdateEnemyCollision(Enemy enemy)
{
    //update bounding box here
}