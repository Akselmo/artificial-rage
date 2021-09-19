#include "../../include/raylib.h"
#include "../../include/raymath.h"
#include"../level/level.h"
#include"../player/player.h"
#include "enemy.h"
#include <stdio.h>

#define ENEMY_START_POSITION_Y 0.4f

Vector3 enemyPosition;
Vector3 enemySize;

//Prototypes
void UpdateEnemyPosition(enemy);

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
        .boundingBox = MakeBoundingBox(enemyPosition, enemySize),
    };
    return enemy;
}



void UpdateEnemy(Enemy enemy)
{
    DrawEnemy(enemy);
    UpdateEnemyPosition(enemy);
}

void DrawEnemy(Enemy enemy)
{
    DrawCubeV(enemy.position, enemy.size, RED);
}

void UpdateEnemyPosition(Enemy enemy)
{
    //Move enemy towards player:
    //- Check if player can be seen (first raycast hit returns player)
    //- If in certain range from player, stop
    //- If cant see player, stop
    //- When stopped, fire
    Ray rayCast;
    BoundingBox playerBb = GetPlayerBoundingBox();
    Vector3 playerPosition = GetPlayerPosition();
    Vector3 v = Vector3Normalize(Vector3Subtract(enemyPosition, playerPosition));
    rayCast.direction = v;
    rayCast.position = enemyPosition;
    DrawRay(rayCast, GREEN);  
    if (CheckCollisionRayBox(rayCast, playerBb))
    {
        printf(" Enemy sees player :) ");
    }

}