#pragma once
#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "raylib.h"
#include "level.h"

typedef struct Projectile_Data
{
    Vector3 startPosition;
    Vector3 endPosition;
    Vector3 position;
    Model model;
    BoundingBox boundingBox;
    int id;
    int damage;
    bool destroyed;
} Projectile_Data;

void Projectile_Launch(Vector3 startPosition, Vector3  endPosition);
void Projectile_Destroy(Projectile_Data *projectile);

#endif
