#include "../../include/raylib.h"

typedef struct Enemy
{
    int health;
    int damage;
    Vector3 position;
    Vector3 size;
    BoundingBox boundingBox;
    bool dead;
} Enemy;

void UpdateEnemies();

Enemy AddEnemy(float pos_x, float pos_y);

void DrawEnemy(Enemy enemy);

bool CheckEnemyCollision(Vector3 entityPos, Vector3 entitySize);
