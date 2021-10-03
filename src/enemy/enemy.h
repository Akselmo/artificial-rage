#ifndef ENEMY
#define ENEMY

#include "../../include/raylib.h"

typedef struct Enemy
{
    int health;
    int damage;
    Vector3 position;
    Vector3 size;
    BoundingBox boundingBox;
    bool dead;
    int id;
} Enemy;

Enemy AddEnemy(float pos_x, float pos_y, int id);

void UpdateEnemy(Enemy enemy);
void DrawEnemy(Enemy enemy);

#endif
