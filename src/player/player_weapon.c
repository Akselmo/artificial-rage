#include "../../include/raylib.h"
#include "../../include/raymath.h"
#include "../settings/settings.h"
#include "../level/level.h"
#include "../enemy/enemy.h"
#include "../utilities/utilities.h"
#include "player.h"
#include "../main.h"
#include <stdio.h>
#include <time.h>

//Fist has always ammo! :)
#define FIST_AMMO 1
#define PISTOL_AMMO_MAX 100
#define RIFLE_AMMO_MAX 200
#define SHOTGUN_AMMO_MAX 50
#define RAILGUN_AMMO_MAX 25

int weaponEquipped;
float weaponFireRate;
int weaponDamage;

typedef struct
{
    int fistKey;
    float fistFirerate;
    int fistDamage;

    int pistolKey;
    int pistolAmmo;
    float pistolFirerate;
    int pistolDamage;

    int rifleKey;
    int rifleAmmo;
    float rifleFirerate;
    int rifleDamage;

    int shotgunKey;
    int shotgunAmmo;
    float shotgunFirerate;
    int shotgunDamage;

    int railgunKey;
    int railgunAmmo;
    float railgunFirerate;
    int railgunDamage;
} WeaponData;

static WeaponData WEAPONDATA =
{
    .fistKey = KEY_ONE,
    .fistFirerate = 0.9f,
    .fistDamage = 5,
    .pistolKey = KEY_TWO,
    .pistolAmmo = 0,
    .pistolFirerate = 0.4f,
    .pistolDamage = 3,
    .rifleKey = KEY_THREE,
    .rifleAmmo = 0,
    .rifleFirerate = 0.25f,
    .rifleDamage = 3,
    .shotgunKey = KEY_FOUR,
    .shotgunAmmo = 0,
    .shotgunFirerate = 0.75,
    .shotgunDamage = 7,
    .railgunKey = KEY_FIVE,
    .railgunAmmo = 0,
    .railgunFirerate = 1.0f,
    .railgunDamage = 30
};

typedef enum
{
    FIST = 1,
    PISTOL = 2,
    RIFLE = 3,
    SHOTGUN = 4,
    RAILGUN = 5
} Weapons;

void InitializeWeaponKeys()
{
    WEAPONDATA.fistKey = GetCustomInput(KEY_ONE);
    WEAPONDATA.pistolKey = GetCustomInput(KEY_TWO);
    WEAPONDATA.rifleKey = GetCustomInput(KEY_THREE);
    WEAPONDATA.shotgunKey = GetCustomInput(KEY_FOUR);
    WEAPONDATA.railgunKey = GetCustomInput(KEY_FIVE);
}

void SelectDefaultWeapon()
{
    weaponEquipped = FIST;
    weaponFireRate = WEAPONDATA.fistFirerate;
    weaponDamage = WEAPONDATA.fistDamage;
}

void ChangeWeapon()
{
    int key = 0;
    key = GetKeyPressed();
    if (key == WEAPONDATA.fistKey)
    {
        weaponEquipped = FIST;
        weaponFireRate = WEAPONDATA.fistFirerate;
        weaponDamage = WEAPONDATA.fistDamage;
        printf("Fist equipped\n");
    }
    else if (key == WEAPONDATA.pistolKey)
    {
        weaponEquipped = PISTOL;
        weaponFireRate = WEAPONDATA.pistolFirerate;
        weaponDamage = WEAPONDATA.pistolDamage;
    }
    else if (key == WEAPONDATA.rifleKey)
    {
        weaponEquipped = RIFLE;
        weaponFireRate = WEAPONDATA.rifleFirerate;
        weaponDamage = WEAPONDATA.rifleDamage;
    }
    else if (key == WEAPONDATA.shotgunKey)
    {
        weaponEquipped = SHOTGUN;
        weaponFireRate = WEAPONDATA.shotgunFirerate;
        weaponDamage = WEAPONDATA.shotgunDamage;
    }
    else if (key == WEAPONDATA.railgunKey)
    {
        weaponEquipped = RAILGUN;
        weaponFireRate = WEAPONDATA.railgunFirerate;
        weaponDamage = WEAPONDATA.railgunDamage;
    }
    //Weapon switching animation goes here
}

int TestEntityHit(Ray rayCast)
{
    int id = 0;
    float distance = 0.0f;
    float levelDistance = INFINITY;
    float enemyDistance = INFINITY;
    int entitiesAmount = GetLevelBlockAmount();
    LevelData *levelData = GetLevelData();
    Enemy *enemies = GetEnemies();
    Enemy enemyDataHit;
    LevelData levelDataHit;

    for (int i = 0; i < entitiesAmount; i++)
    {
        if (levelData[i].modelId != 0)
        {
            Vector3 pos = levelData[i].levelBlockPosition;
            RayCollision hitLevel = GetRayCollisionMesh(rayCast, levelData[i].levelBlockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
            if (hitLevel.hit)
            {
                if (hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit = levelData[i];
                }
            }
        }
        RayCollision enemyHit = GetRayCollisionBox(rayCast, enemies[i].boundingBox);
        if (enemyHit.hit)
        {
            if (!enemies[i].dead)
            {
                if (Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position)) < enemyDistance)
                {
                    enemyDistance = Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position));
                    enemyDataHit = enemies[i];
                }
            }
        }

    }
    if (enemyDistance < levelDistance)
    {
        id = enemyDataHit.id;
    }
    else
    {
        id = 0;
    }
    return id;
}


bool WeaponHasAmmo()
{
    int ammo;
    if (weaponEquipped == FIST)
    {
        return true;
    }

    switch (weaponEquipped)
    {
    case PISTOL:
        ammo = WEAPONDATA.pistolAmmo;
        break;

    case RIFLE:
        ammo = WEAPONDATA.rifleAmmo;
        break;

    case SHOTGUN:
        ammo = WEAPONDATA.shotgunAmmo;
        break;

    case RAILGUN:
        ammo = WEAPONDATA.railgunAmmo;
        break;
    }

    if (ammo > 0)
    {
        ammo--;
        return true;
    }
    else
    {
        return false;
    }
}

float FireWeapon(Camera* camera, float nextFire)
{
    if (WeaponHasAmmo())
    {
        if (nextFire > 0)
        {
            nextFire -= GetFrameTime();
        }
        else
        {
            Ray rayCast = GetMouseRay(GetScreenCenter(), *camera);
            int id = TestEntityHit(rayCast);
            printf("Id hit: %i \n", id);
            if (id != 0 && id != PLAYER_ID)
            {
                TakeDamage(&GetEnemies()[id], weaponDamage);
                printf("Enemy id %d takes %d damage\n", id, weaponDamage);
            }
            nextFire = weaponFireRate;
        }
    }
    return nextFire;
}
