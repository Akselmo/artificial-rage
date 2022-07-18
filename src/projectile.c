#include "projectile.h"
#include "level.h"
#include "raylib.h"
#include "utilities.h"

void Projectile_Create(Ray rayCast, Vector3 size, int damage)
{
    // TODO: Lerp between start and end position
    // Projectile does not need to know if it hits or not, that is calculated from
    // entities themselves.
    for(int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        if(Level_projectiles[i] == NULL)
        {
            Projectile projectile;

            printf("Creating new projectile with id %d\n", i);
            projectile.startPosition = rayCast.position;
            projectile.endPosition =
                Vector3Add(rayCast.position, Vector3Scale(rayCast.direction, PROJECTILE_TRAVEL_DISTANCE));
            projectile.id          = i;
            projectile.boundingBox = Utilities_MakeBoundingBox(rayCast.position, size);
            projectile.damage      = damage;

            Level_projectiles[i] = &projectile;
        }
    }
}

void Projectile_Update(Projectile* projectile)
{
    // Lerp projectile here (both model and boundingbox)
}

void Projectile_Destroy(Projectile* projectile)
{
    // TODO: Destroy the projectile, free it from memory.
    // Could also do object pooling if feeling creative but that's more a "nice-to-have"
    //  Destroy after either collide or after 20 seconds
}
