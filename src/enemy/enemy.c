#include "../../include/raylib.h"
#include "../../include/raymath.h"
#include"../level/level.h"
#include"../player/player.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

#define ENEMY_START_POSITION_Y 0.4f

//Prototypes
void UpdateEnemyPosition(Enemy enemy);
int TestPlayerHit(Ray rayCast);

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
    };
    return enemy;
}



void UpdateEnemy(Enemy* enemy)
{
    DrawEnemy(*enemy);
    if (enemy->nextTick > 0)
    {
        enemy->nextTick -= GetFrameTime();
    }
    else
    {
        UpdateEnemyPosition(*enemy);
        enemy->nextTick = enemy->tickRate;
    }
}

void DrawEnemy(Enemy enemy)
{
    DrawCubeV(enemy.position, enemy.size, RED);
}

int TestPlayerHit(Ray rayCast)
{
    int id = 0;
    float distance = 0.0f;
    float levelDistance = INFINITY;
    float playerDistance = INFINITY;
    int entitiesAmount = GetLevelBlockAmount();
    Vector3 playerPosition = GetPlayerPosition();
    LevelData *levelData = GetLevelData();
    LevelData levelDataHit;

    for (int i = 0; i < entitiesAmount; i++)
    {
        if (levelData[i].modelId != 0)
        {
            Vector3 pos = levelData[i].levelBlockPosition;
            RayHitInfo hitLevel = GetCollisionRayMesh(rayCast, levelData[i].levelBlockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
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
        id = PLAYER_ID;
        printf("hit player \n");
    }
    else
    {
        //Wall is closer so return its id
        id = levelDataHit.modelId;
    }
    return id;
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
    Vector3 v = Vector3Normalize(Vector3Subtract(enemy.position, playerPosition));
    rayCast.direction = (Vector3){-1.0f*v.x, -1.0f*v.y, -1.0f*v.z};
    rayCast.position = enemy.position;
    DrawRay(rayCast, GREEN);
    TestPlayerHit(rayCast);
}
