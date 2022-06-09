#ifndef ENEMY
#define ENEMY

#include "raylib.h"

#define ENEMY_DEFAULT_SPEED 0.45f  // Lower values result to lower speed

typedef struct Enemy
{
    int health;
    int damage;
    Vector3 position;
    Vector3 size;
    BoundingBox boundingBox;
    bool dead;
    int id;
    float tickRate;
    float nextTick;
    float speed;
    float fireRate;
    float nextFire;
} Enemy;
Enemy AddEnemy(float pos_x, float pos_y, int id);
void UpdateEnemy(Enemy* enemy);
void DrawEnemy(Enemy enemy);
void TakeDamage(Enemy* enemy, int damageAmount);

#endif