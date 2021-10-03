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
float fireRate = 1.0f;
float nextFire = -1.0f;

typedef struct
{
    int fistKey;
    float fistFirerate;

    int pistolKey;
    int pistolAmmo;
    float pistolFirerate;

    int rifleKey;
    int rifleAmmo;
    float rifleFirerate;

    int shotgunKey;
    int shotgunAmmo;
    float shotgunFirerate;

    int railgunKey;
    int railgunAmmo;
    float railgunFirerate;
} WeaponData;

static WeaponData WEAPONDATA = {
    .fistKey = KEY_ONE,
    .fistFirerate = 0.9f,
    .pistolKey = KEY_TWO,
    .pistolAmmo = 0,
    .pistolFirerate = 0.4f,
    .rifleKey = KEY_THREE,
    .rifleAmmo = 0,
    .rifleFirerate = 0.25f,
    .shotgunKey = KEY_FOUR,
    .shotgunAmmo = 0,
    .shotgunFirerate = 0.75,
    .railgunKey = KEY_FIVE,
    .railgunAmmo = 0,
    .railgunFirerate = 1.0f};

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
    fireRate = WEAPONDATA.fistFirerate;
}

void ChangeWeapon()
{
    int key = 0;
    key = GetKeyPressed();
    if (key == WEAPONDATA.fistKey)
    {
        weaponEquipped = FIST;
        fireRate = WEAPONDATA.fistFirerate;
        printf("Fist equipped\n");
    }
    else if (key == WEAPONDATA.pistolKey)
    {
        weaponEquipped = PISTOL;
        fireRate = WEAPONDATA.pistolFirerate;
    }
    else if (key == WEAPONDATA.rifleKey)
    {
        weaponEquipped = RIFLE;
        fireRate = WEAPONDATA.rifleFirerate;
    }
    else if (key == WEAPONDATA.shotgunKey)
    {
        weaponEquipped = SHOTGUN;
        fireRate = WEAPONDATA.shotgunFirerate;
    }
    else if (key == WEAPONDATA.railgunKey)
    {
        weaponEquipped = RAILGUN;
        fireRate = WEAPONDATA.railgunFirerate;
    }
    //Weapon switching goes here
}




int TestEntityHit(Ray rayCast, int entityAmount)
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
        //See if we need meshes here too
        if (levelData[i].modelId != 0)
        {
            Vector3 pos = levelData[i].levelBlockPosition;
            RayHitInfo hitLevel = GetCollisionRayMesh(rayCast, levelData[i].levelBlockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
            if (hitLevel.hit)
            {
                levelData[i].levelBlockPosition = Vector3Zero();
                if (hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit = levelData[i];
                }
            }
        }


        bool enemyHit = CheckCollisionRayBox(rayCast, enemies[i].boundingBox);
        if (enemyHit)
        {
            if (Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position)) < enemyDistance)
            {
                enemyDistance = Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position));
                enemyDataHit = enemies[i];
            }
        }

    }
    if (enemyDistance < levelDistance)
    {
        //Enemy is closer so return its id
        id = enemyDataHit.id;
        printf("Hit enemy\n");
    }
    else
    {
        //Wall is closer so return its id
        id = levelDataHit.modelId;
        printf("Hit wall\n");
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

void FireWeapon(Vector3 playerPosition, Vector3 target, int levelSize, int entities)
{
    if (WeaponHasAmmo())
    {
        if (nextFire > 0)
        {
            nextFire -= GetFrameTime();
            return;
        }
        else
        {
            Ray rayCast;

            Vector3 v = Vector3Normalize(Vector3Subtract(playerPosition, target));
            rayCast.direction = (Vector3){-1*v.x, -1*v.y, -1*v.z};
            rayCast.position = playerPosition;

            //float levelDistance = TestLevelHit(rayCast);
            int id = TestEntityHit(rayCast, entities);

            if (id != 0)
            {
                printf("id: %d\n", id);
                nextFire = fireRate;
            }
        }
    }
}
