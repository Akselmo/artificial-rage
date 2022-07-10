#include "projectile.h"
#include "level.h"

void Projectile_Launch(Vector3 startPosition, Vector3 endPosition)
{
    //TODO: Lerp between start and end position
    //Projectile does not need to know if it hits or not, that is calculated from
    //entities themselves.
    Projectile_Data projectile;
    
    for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        if (Level_projectiles[i] != NULL)
        {
            printf("Hello \n");
            //Level_projectiles[i] = projectile;
        }
    }
}

void Projectile_Destroy(Projectile_Data *projectile)
{
    //TODO: Destroy the projectile, free it from memory.
    //Could also do object pooling if feeling creative but that's more a "nice-to-have"
    // Destroy after either collide or after 20 seconds
}
