#include "../../include/raylib.h"
#include "../../include/raymath.h"
#include"../level/level.h"
#include"../player/player.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

#define ENEMY_START_POSITION_Y 0.4f
#define MAX_DISTANCE_FROM_PLAYER 0.85f

//Prototypes
void UpdateEnemyPosition(Enemy* enemy);
bool TestPlayerHit(Enemy* enemy);

//Since we use billboarding we dont have to know rotation

Enemy AddEnemy(float pos_x, float pos_y, int id)
{
    Vector3 enemyPosition = (Vector3){pos_x, ENEMY_START_POSITION_Y, pos_y};
    Vector3 enemySize = (Vector3){0.25f, 0.8f, 0.25f};
    float randomTickRate = ((float)rand()/(float)(RAND_MAX))*2;

    Enemy enemy = {
        .position = enemyPosition,
        .size = enemySize,
        .dead = false,
        .damage = 5,
        .health = 50,
        .boundingBox = MakeBoundingBox(enemyPosition, enemySize),
        .id = id,
        .tickRate = randomTickRate,
        .nextTick = -1.0f,
        .speed = 0.01,
    };
    return enemy;
}



void UpdateEnemy(Enemy* enemy)
{
    if (!enemy->dead)
    {
        DrawEnemy(*enemy);
        if (enemy->nextTick > 0)
        {
            enemy->nextTick -= GetFrameTime();
        }
        else
        {
            enemy->nextTick = enemy->tickRate;
        }
        UpdateEnemyPosition(enemy);
    }
}

void DrawEnemy(Enemy enemy)
{
    DrawCubeV(enemy.position, enemy.size, RED);
}

bool TestPlayerHit(Enemy* enemy)
{

    Ray rayCast;
    BoundingBox playerBb = GetPlayerBoundingBox();
    Vector3 playerPosition = GetPlayerPosition();
    Vector3 v = Vector3Normalize(Vector3Subtract(enemy->position, playerPosition));
    rayCast.direction = (Vector3){-1.0f*v.x, -1.0f*v.y, -1.0f*v.z};
    rayCast.position = enemy->position;

    bool hitPlayer = false;
    float distance = 0.0f;
    float levelDistance = INFINITY;
    float playerDistance = INFINITY;
    int entitiesAmount = GetLevelBlockAmount();
    LevelData *levelData = GetLevelData();
    LevelData levelDataHit;

    for (int i = 0; i < entitiesAmount; i++)
    {
        if (levelData[i].modelId != 0)
        {
            Vector3 pos = levelData[i].levelBlockPosition;
            RayCollision hitLevel = GetRayCollisionMesh(rayCast, levelData[i].levelBlockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
            if (hitLevel.hit)
            {
                if (hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit = levelData[i];
                }
            }
        }
    }

    playerDistance = Vector3Length(Vector3Subtract(playerPosition, rayCast.position));

    if (playerDistance < levelDistance)
    {
        //Player is closer
        hitPlayer = true;
    }
    else
    {
        //Wall/other entity is closer so we didnt hit player
        hitPlayer = false;
    }
    return hitPlayer;
}

void UpdateEnemyPosition(Enemy* enemy)
{
    //Move enemy towards player:
    //- Check if player can be seen (first raycast hit returns player)
    //- If in certain range from player, stop
    //- If cant see player, stop
    //- When stopped, fire
    Vector3 DistanceFromPlayer = Vector3Subtract(enemy->position, GetPlayerPosition());
    if (TestPlayerHit(enemy))
    {
        if (fabsf(DistanceFromPlayer.x) >= MAX_DISTANCE_FROM_PLAYER || fabsf(DistanceFromPlayer.z) >= MAX_DISTANCE_FROM_PLAYER)
        {
            Vector3 enemyOldPosition = enemy->position;
            enemy->position = Vector3Lerp(enemy->position, GetPlayerPosition(), enemy->speed);
            if (CheckLevelCollision(enemy->position,enemy->size, enemy->id))
            {
                enemy->position = enemyOldPosition;
            }
        }
    }
 }


