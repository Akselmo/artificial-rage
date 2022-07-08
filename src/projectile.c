#include "projectile.h"

Projectile_Data Projectile_Launch(Vector3 startPosition, Vector3 endPosition)
{
    //TODO: Lerp between start and end position
    //Projectile does not need to know if it hits or not, that is calculated from
    //entities themselves.
}

void Projectile_Destroy(Projectile_Data *projectile)
{
    //TODO: Destroy the projectile, free it from memory.
    //Could also do object pooling if feeling creative but that's more a "nice-to-have"
}
