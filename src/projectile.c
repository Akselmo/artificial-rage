#include "projectile.h"
#include "level.h"
#include "raylib.h"
#include "raymath.h"
#include "utilities.h"

void Projectile_Create(Ray rayCast, Vector3 size, int damage)
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
                .size          = size,
                .boundingBox   = Utilities_MakeBoundingBox(rayCast.position, projectile.size),
                .damage        = damage,
                .color         = BLUE,
                .speed         = 0.12f * GetFrameTime(),
                .destroyed     = false,
            };
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
        Projectile_Destroy(projectile);
    }

}

void Projectile_Destroy(Projectile* projectile)
{
    // TODO: Destroy the projectile, free it from memory.
    // Could also do object pooling if feeling creative but that's more a "nice-to-have"
    //  Destroy after either collide or after 20 seconds
    Vector3 distanceFromEnd = Vector3Subtract(projectile->position, projectile->endPosition);
    if (fabsf(distanceFromEnd.x) <= 0.01f || fabsf(distanceFromEnd.y) <= 0.01f || fabsf(distanceFromEnd.z) <= 0.01f)
    {
        projectile->destroyed = true;
    }
}
