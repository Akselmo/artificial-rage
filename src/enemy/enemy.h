#include "../../include/raylib.h"

typedef struct Enemy
{
    int health;
    int damage;
    Vector3 startPosition;
    Vector3 enemyHitboxPosition;
    Vector3 enemyHitboxSize;
    BoundingBox enemyBoundingBox;
    bool dead;
} Enemy;

void UpdateEnemies();

bool CheckEnemyCollision(Vector3 entityPos, Vector3 entitySize);
