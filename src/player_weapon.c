#include "include/raylib.h"
#include "include/raymath.h"
#include "player_weapon.h"
#include "settings.h"
#include "level.h"
#include "enemy.h"
#include "utilities.h"
#include "player.h"
#include "main.h"
#include <stdio.h>
#include <time.h>

//Fist has always ammo! :)
#define FIST_AMMO 1
#define PISTOL_AMMO_MAX 100
#define RIFLE_AMMO_MAX 200
#define SHOTGUN_AMMO_MAX 50
#define RAILGUN_AMMO_MAX 25


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
    WEAPONDATA.weaponEquipped = FIST;
    WEAPONDATA.weaponFireRate = WEAPONDATA.fistFirerate;
    WEAPONDATA.weaponDamage = WEAPONDATA.fistDamage;

}

void ChangeWeapon()
{
    int key = 0;
    key = GetKeyPressed();
    if (key == WEAPONDATA.fistKey)
    {
        WEAPONDATA.weaponEquipped = FIST;
        WEAPONDATA.weaponFireRate = WEAPONDATA.fistFirerate;
        WEAPONDATA.weaponDamage = WEAPONDATA.fistDamage;
        WEAPONDATA.currentWeaponAmmo = 0;
        printf("Fist equipped\n");
    }
    else if (key == WEAPONDATA.pistolKey)
    {
        WEAPONDATA.weaponEquipped = PISTOL;
        WEAPONDATA.weaponFireRate = WEAPONDATA.pistolFirerate;
        WEAPONDATA.weaponDamage = WEAPONDATA.pistolDamage;
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.pistolAmmo;
        printf("Pistol equipped\n");
    }
    else if (key == WEAPONDATA.rifleKey)
    {
        WEAPONDATA.weaponEquipped = RIFLE;
        WEAPONDATA.weaponFireRate = WEAPONDATA.rifleFirerate;
        WEAPONDATA.weaponDamage = WEAPONDATA.rifleDamage;
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.rifleAmmo;
    }
    else if (key == WEAPONDATA.shotgunKey)
    {
        WEAPONDATA.weaponEquipped = SHOTGUN;
        WEAPONDATA.weaponFireRate = WEAPONDATA.shotgunFirerate;
        WEAPONDATA.weaponDamage = WEAPONDATA.shotgunDamage;
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.shotgunAmmo;
    }
    else if (key == WEAPONDATA.railgunKey)
    {
        WEAPONDATA.weaponEquipped = RAILGUN;
        WEAPONDATA.weaponFireRate = WEAPONDATA.railgunFirerate;
        WEAPONDATA.weaponDamage = WEAPONDATA.railgunDamage;
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.railgunAmmo;
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

    if (WEAPONDATA.weaponEquipped == FIST)
    {
        return true;
    }

    switch (WEAPONDATA.weaponEquipped)
    {
    case PISTOL:
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.pistolAmmo;
        break;

    case RIFLE:
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.rifleAmmo;
        break;

    case SHOTGUN:
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.shotgunAmmo;
        break;

    case RAILGUN:
        WEAPONDATA.currentWeaponAmmo = WEAPONDATA.railgunAmmo;
        break;
    }

    printf("Ammo: %d \n", WEAPONDATA.currentWeaponAmmo);
//TODO: Is there a better way to do this without so much repetition?
    if (WEAPONDATA.currentWeaponAmmo > 0)
    {
        WEAPONDATA.currentWeaponAmmo--;
        switch (WEAPONDATA.weaponEquipped)
        {
            case PISTOL:
                WEAPONDATA.pistolAmmo--;
                break;

            case RIFLE:
                WEAPONDATA.rifleAmmo--;
                break;

            case SHOTGUN:
                WEAPONDATA.shotgunAmmo--;
                break;

            case RAILGUN:
                WEAPONDATA.railgunAmmo--;
                break;
        }
        return true;
    }
    else
    {
        return false;
    }
}

float FireWeapon(Camera* camera, float nextFire)
{
    if (nextFire > 0)
    {
        nextFire -= GetFrameTime();
    }
    else
    {
        if (WeaponHasAmmo())
        {
            Ray rayCast = GetMouseRay(GetScreenCenter(), *camera);
            int id = TestEntityHit(rayCast);
            printf("Id hit: %i \n", id);
            if (id != 0 && id != PLAYER_ID)
            {
                TakeDamage(&GetEnemies()[id], WEAPONDATA.weaponDamage);
                printf("Enemy id %d takes %d damage\n", id, WEAPONDATA.weaponDamage);
            }
        }
        nextFire = WEAPONDATA.weaponFireRate;
    }
    return nextFire;
}

WeaponData GetWeaponData()
{
    return WEAPONDATA;
}
