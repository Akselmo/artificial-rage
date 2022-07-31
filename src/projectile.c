#include "projectile.h"
#include "enemy.h"
#include "level.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"

//Prototypes
void Projectile_DestroyOverTime(Projectile* projectile);

void Projectile_Create(Ray rayCast, Vector3 size, int damage, int ownerId)
{
    // TODO: Lerp between start and end position
    // Projectile does not need to know if it hits or not, that is calculated from
    // entities themselves.
    for(int i = 1; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        if(Level_projectiles[i].id != i || Level_projectiles[i].destroyed == true)
        {
            Projectile projectile = {
                .startPosition = rayCast.position,
                .endPosition   = Vector3Add(rayCast.position, Vector3Scale(rayCast.direction, PROJECTILE_TRAVEL_DISTANCE)),
                .position      = projectile.startPosition,
                .id            = i,
                .ownerId       = ownerId,
                .size          = size,
                .boundingBox   = Utilities_MakeBoundingBox(rayCast.position, projectile.size),
                .damage        = damage,
                .color         = BLUE,
                .speed         = 0.12f * GetFrameTime(),
                .destroyed     = false,
            };
            printf("Projectile id created %d\n", projectile.id);
            Level_projectiles[i] = projectile;
            break;
        }
    }
}

void Projectile_Update(Projectile* projectile)
{
    if(!projectile->destroyed && projectile->id != 0)
    {
        DrawCubeV(projectile->position, projectile->size, projectile->color);
        // Lerp projectile here (both model and boundingbox)
        projectile->position = Vector3Lerp(projectile->position, projectile->endPosition, projectile->speed);
        Projectile_DestroyOverTime(projectile);
        Projectile_CheckCollision(projectile);
    }
}

void Projectile_CheckCollision(Projectile* projectile)
{
    //Check against the owner of the projectile and the entity id. if theres a match, ignore it, unless its a wall
    // Otherwise tell the entity they've been hit and give them damage
    BoundingBox projectileBox = Utilities_MakeBoundingBox(projectile->position, projectile->size);
    for (int i = 0; i < Level_mapSize; i++)
    {
        
        //Test hitting against wall
        if(CheckCollisionBoxes(projectileBox, Level_data[i].blockBoundingBox) && Level_data[i].modelId != projectile->ownerId)
        {
           Projectile_Destroy(projectile);
           return;
        }
        //Against enemy except if owned by enemy
        else if(CheckCollisionBoxes(projectileBox, Level_enemies[i].boundingBox) && Level_enemies[i].id != projectile->ownerId)
        {
            Enemy_TakeDamage(&Level_enemies[i], projectile->damage);
            Projectile_Destroy(projectile);
            return;
        }
        //Against player except if owned by player
        else if (CheckCollisionBoxes(projectileBox, GetPlayerBoundingBox()) && PLAYER_ID != projectile->ownerId)
        {
            Player_SetHealth(-1*projectile->damage);
            Projectile_Destroy(projectile);
            return;
        }
    }

}

//Rename to projectile_DestroyOverTime
void Projectile_DestroyOverTime(Projectile* projectile)
{
    // TODO: Destroy after 20 seconds
}

//Rename to projectile_destroy
void Projectile_Destroy(Projectile* projectile)
{
    printf("Projectile id %d destroyed\n", projectile->id);
    projectile->destroyed = true;
    projectile            = NULL;
    free(projectile);
}