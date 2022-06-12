#include "enemy.h"

// Private functions
void Enemy_UpdatePosition(Enemy_Data* enemy);
bool Enemy_TestPlayerHit(Enemy_Data* enemy);
float Enemy_FireAtPlayer(Enemy_Data* enemy, float nextFire);

// Since we use billboarding we dont have to know rotation
Enemy_Data Enemy_Add(float pos_x, float pos_y, int id)
{
    Vector3 enemyPosition = (Vector3) {pos_x, ENEMY_START_POSITION_Y, pos_y};
    Vector3 enemySize     = (Vector3) {0.25f, 0.8f, 0.25f};
    float randomTickRate  = ((float)rand() / (float)(RAND_MAX)) * 2;
    Enemy_Data enemy           = {
                  .position    = enemyPosition,
                  .size        = enemySize,
                  .dead        = false,
                  .damage      = 5,
                  .health      = 50,
                  .boundingBox = Utilities_MakeBoundingBox(enemyPosition, enemySize),
                  .id          = id,
                  .tickRate    = randomTickRate,
                  .nextTick    = -1.0f,
                  .speed       = 0.01f,
                  .fireRate    = 5.75f,
                  .nextFire    = 0.0f,
    };
    return enemy;
}

void Enemy_Update(Enemy_Data* enemy)
{
    if(!enemy->dead)
    {
        Enemy_Draw(*enemy);
        if(enemy->nextTick > 0)
        {
            enemy->nextTick -= GetFrameTime();
        }
        else
        {
            enemy->nextTick = enemy->tickRate;
        }
        Enemy_UpdatePosition(enemy);
        enemy->nextFire -= GetFrameTime();
        enemy->nextFire = Enemy_FireAtPlayer(enemy, enemy->nextFire);
    }
}

void Enemy_Draw(Enemy_Data enemy)
{
    DrawCubeV(enemy.position, enemy.size, RED);
}

bool Enemy_TestPlayerHit(Enemy_Data* enemy)
{

    Ray rayCast;
    BoundingBox playerBb   = GetPlayerBoundingBox();
    Vector3 playerPosition = GetPlayerPosition();
    Vector3 v              = Vector3Normalize(Vector3Subtract(enemy->position, playerPosition));
    rayCast.direction      = (Vector3) {-1.0f * v.x, -1.0f * v.y, -1.0f * v.z};
    rayCast.position       = enemy->position;

    bool hitPlayer       = false;
    float distance       = 0.0f;
    float levelDistance  = INFINITY;
    float playerDistance = INFINITY;
    int entitiesAmount   = Level_mapSize;
    Level_Data* levelData = Level_data;
    Level_Data levelDataHit;

    for(int i = 0; i < entitiesAmount; i++)
    {
        if(levelData[i].modelId != 0)
        {
            Vector3 pos           = levelData[i].blockPosition;
            RayCollision hitLevel = GetRayCollisionMesh(rayCast,
                                                        levelData[i].blockModel.meshes[0],
                                                        MatrixTranslate(pos.x, pos.y, pos.z));
            if(hitLevel.hit)
            {
                if(hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit  = levelData[i];
                }
            }
        }
    }

    playerDistance = Vector3Length(Vector3Subtract(playerPosition, rayCast.position));

    if(playerDistance < levelDistance)
    {
        // Player is closer
        hitPlayer = true;
    }
    else
    {
        // Wall/other entity is closer so we didnt hit player
        hitPlayer = false;
    }
    return hitPlayer;
}

void Enemy_UpdatePosition(Enemy_Data* enemy)
{
    // Move enemy towards player:
    //- Check if player can be seen (first raycast hit returns player)
    //- If in certain range from player, stop
    //- If cant see player, stop
    //- When stopped, fire
    enemy->speed               = ENEMY_DEFAULT_SPEED * GetFrameTime();
    Vector3 DistanceFromPlayer = Vector3Subtract(enemy->position, GetPlayerPosition());
    if(Enemy_TestPlayerHit(enemy))
    {
        if(fabsf(DistanceFromPlayer.x) >= ENEMY_MAX_DISTANCE_FROM_PLAYER ||
           fabsf(DistanceFromPlayer.z) >= ENEMY_MAX_DISTANCE_FROM_PLAYER)
        {
            Vector3 enemyOldPosition = enemy->position;
            enemy->position = Vector3Lerp(enemy->position, GetPlayerPosition(), enemy->speed);
            if(Level_CheckCollision(enemy->position, enemy->size, enemy->id))
            {
                enemy->position = enemyOldPosition;
            }
        }
    }
    enemy->boundingBox = Utilities_MakeBoundingBox(enemy->position, enemy->size);
}

void Enemy_TakeDamage(Enemy_Data* enemy, int damageAmount)
{
    if(!enemy->dead)
    {
        enemy->health -= damageAmount;
        if(enemy->health <= 0)
        {
            // Dirty hack to move bounding box outside of map so it cant be collided to.
            // We want to keep enemy in the memory so we can use its position to display the
            // corpse/death anim
            Vector3 deadBoxPos = (Vector3) {
                ENEMY_GRAVEYARD_POSITION,
                ENEMY_GRAVEYARD_POSITION,
                ENEMY_GRAVEYARD_POSITION
            };
            enemy->boundingBox = Utilities_MakeBoundingBox(deadBoxPos, Vector3Zero());
            enemy->dead        = true;
        }
    }
}

float Enemy_FireAtPlayer(Enemy_Data* enemy, float nextFire)
{
    if(Enemy_TestPlayerHit(enemy))
    {
        if(nextFire > 0)
        {
            nextFire -= GetFrameTime();
        }
        else
        {
            // Fire animation should play anyway, we just hit player
            // if the following random check goes through
            if(GetRandomValue(0, 10) >= 6)
            {
                int dmg = enemy->damage * -1;
                printf(" Enemy_Data %i Hit player for %i dmg\n", enemy->id, dmg);
                Player_SetHealth(dmg);
            }
            nextFire = enemy->fireRate;
        }
    }
    return nextFire;
}