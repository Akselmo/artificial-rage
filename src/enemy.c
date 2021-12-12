#include "include/raylib.h"
#include "include/raymath.h"
#include "utilities.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include <stdio.h>
#include <stdlib.h>

#define ENEMY_START_POSITION_Y 0.4f
#define ENEMY_GRAVEYARD_POSITION_Y 32.0f
#define MAX_DISTANCE_FROM_PLAYER 1.25f

//Prototypes
void UpdateEnemyPosition(Enemy* enemy);
bool TestPlayerHit(Enemy* enemy);
float FireAtPlayer(Enemy* enemy, float nextFire);

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
        .fireRate = 5.75f,
        .nextFire = 0.0f,
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
        enemy->nextFire -= GetFrameTime();
        enemy->nextFire = FireAtPlayer(enemy, enemy->nextFire);
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
        if (fabsf(DistanceFromPlayer.x) >= MAX_DISTANCE_FROM_PLAYER ||
            fabsf(DistanceFromPlayer.z) >= MAX_DISTANCE_FROM_PLAYER)
        {
            Vector3 enemyOldPosition = enemy->position;
            enemy->position = Vector3Lerp(enemy->position, GetPlayerPosition(), enemy->speed);
            if (CheckLevelCollision(enemy->position,enemy->size, enemy->id))
            {
                enemy->position = enemyOldPosition;
            }
        }
    }
    enemy->boundingBox = MakeBoundingBox(enemy->position, enemy->size);
 }

void TakeDamage(Enemy* enemy, int damageAmount)
{
    if (!enemy->dead)
    {
        enemy->health -= damageAmount;
        if (enemy->health <= 0)
        {
            //Dirty hack to move bounding box outside of map so it cant be collided to.
            //We want to keep enemy in the memory so we can use its position to display the corpse/death anim
            Vector3 deadBoxPos = (Vector3){999.0f, 999.0f, 999.0f};
            enemy->boundingBox = MakeBoundingBox(deadBoxPos, Vector3Zero());
            enemy->dead = true;
        }
    }
}

float FireAtPlayer(Enemy* enemy, float nextFire)
{
    if (TestPlayerHit(enemy))
    {
        if (nextFire > 0)
        {
            nextFire -= GetFrameTime();
        }
        else
        {
            //Fire animation should play anyway, we just hit player
            //if the following random check goes through
            if (GetRandomValue(0,10) >= 6)
            {
                int dmg = enemy->damage * -1;
                printf(" Enemy %i Hit player for %i dmg\n", enemy->id, dmg);
                PlayerSetHealth(dmg);
            }
            nextFire = enemy->fireRate;
        }
    }
    return nextFire;
}
