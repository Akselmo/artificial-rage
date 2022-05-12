#include "weapon.h"
#include "enemy.h"
#include "include/raylib.h"
#include "include/raymath.h"
#include "level.h"
#include "main.h"
#include "player.h"
#include "settings.h"
#include "utilities.h"
#include <stdio.h>
#include <time.h>

// Fist has always ammo! :)
#define FIST_AMMO        1
#define PISTOL_AMMO_MAX  100
#define RIFLE_AMMO_MAX   200
#define SHOTGUN_AMMO_MAX 50
#define RAILGUN_AMMO_MAX 25

void InitializeWeaponKeys()
{
    PLAYER_WEAPON_HOLDER.FIST.inputKey    = GetCustomInput(KEY_ONE);
    PLAYER_WEAPON_HOLDER.PISTOL.inputKey  = GetCustomInput(KEY_TWO);
    PLAYER_WEAPON_HOLDER.RIFLE.inputKey   = GetCustomInput(KEY_THREE);
    PLAYER_WEAPON_HOLDER.SHOTGUN.inputKey = GetCustomInput(KEY_FOUR);
    PLAYER_WEAPON_HOLDER.RAILGUN.inputKey = GetCustomInput(KEY_FIVE);
}

void SelectDefaultWeapon()
{
    PLAYER_WEAPON_HOLDER.currentWeapon         = FIST;
    PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.FIST.fireRate;
    PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.FIST.damage;
}

// TODO: Check if weapon is equipped
void ChangeWeapon()
{
    int key = 0;
    key     = GetKeyPressed();
    if(key == PLAYER_WEAPON_HOLDER.FIST.inputKey)
    {
        PLAYER_WEAPON_HOLDER.currentWeapon         = FIST;
        PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.FIST.fireRate;
        PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.FIST.damage;
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo     = 9999;
        printf("Fist equipped\n");
    }
    else if(key == PLAYER_WEAPON_HOLDER.PISTOL.inputKey)
    {
        PLAYER_WEAPON_HOLDER.currentWeapon         = PISTOL;
        PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.PISTOL.fireRate;
        PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.PISTOL.damage;
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo     = PLAYER_WEAPON_HOLDER.PISTOL.ammo;
        printf("Pistol equipped\n");
    }
    else if(key == PLAYER_WEAPON_HOLDER.RIFLE.inputKey)
    {
        PLAYER_WEAPON_HOLDER.currentWeapon         = RIFLE;
        PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.RIFLE.fireRate;
        PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.RIFLE.damage;
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo     = PLAYER_WEAPON_HOLDER.RIFLE.ammo;
    }
    else if(key == PLAYER_WEAPON_HOLDER.SHOTGUN.inputKey)
    {
        PLAYER_WEAPON_HOLDER.currentWeapon         = SHOTGUN;
        PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.SHOTGUN.fireRate;
        PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.SHOTGUN.damage;
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo     = PLAYER_WEAPON_HOLDER.SHOTGUN.ammo;
    }
    else if(key == PLAYER_WEAPON_HOLDER.RAILGUN.inputKey)
    {
        PLAYER_WEAPON_HOLDER.currentWeapon         = RAILGUN;
        PLAYER_WEAPON_HOLDER.currentWeaponFirerate = PLAYER_WEAPON_HOLDER.RAILGUN.fireRate;
        PLAYER_WEAPON_HOLDER.currentWeaponDamage   = PLAYER_WEAPON_HOLDER.RAILGUN.damage;
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo     = PLAYER_WEAPON_HOLDER.RAILGUN.ammo;
    }
    // Weapon switching animation goes here
}

int TestEntityHit(Ray rayCast)
{
    int id               = 0;
    float distance       = 0.0f;
    float levelDistance  = INFINITY;
    float enemyDistance  = INFINITY;
    int entitiesAmount   = GetLevelBlockAmount();
    LevelData* levelData = GetLevelData();
    Enemy* enemies       = GetEnemies();
    Enemy enemyDataHit;
    LevelData levelDataHit;

    for(int i = 0; i < entitiesAmount; i++)
    {
        if(levelData[i].modelId != 0)
        {
            Vector3 pos           = levelData[i].levelBlockPosition;
            RayCollision hitLevel = GetRayCollisionMesh(rayCast,
                                                        levelData[i].levelBlockModel.meshes[0],
                                                        MatrixTranslate(pos.x, pos.y, pos.z));
            if(hitLevel.hit)
            {
                if(hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit  = levelData[i];
                }
            }
        }
        RayCollision enemyHit = GetRayCollisionBox(rayCast, enemies[i].boundingBox);
        if(enemyHit.hit)
        {
            if(!enemies[i].dead)
            {
                if(Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position)) <
                   enemyDistance)
                {
                    enemyDistance =
                        Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position));
                    enemyDataHit = enemies[i];
                }
            }
        }
    }
    if(enemyDistance < levelDistance)
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

    if(PLAYER_WEAPON_HOLDER.currentWeapon == FIST)
    {
        return true;
    }

    switch(PLAYER_WEAPON_HOLDER.currentWeapon)
    {
        case PISTOL:
            PLAYER_WEAPON_HOLDER.currentWeaponAmmo = PLAYER_WEAPON_HOLDER.PISTOL.ammo;
            break;

        case RIFLE:
            PLAYER_WEAPON_HOLDER.currentWeaponAmmo = PLAYER_WEAPON_HOLDER.RIFLE.ammo;
            break;

        case SHOTGUN:
            PLAYER_WEAPON_HOLDER.currentWeaponAmmo = PLAYER_WEAPON_HOLDER.SHOTGUN.ammo;
            break;

        case RAILGUN:
            PLAYER_WEAPON_HOLDER.currentWeaponAmmo = PLAYER_WEAPON_HOLDER.RAILGUN.ammo;
            break;
    }

    printf("Ammo: %d \n", PLAYER_WEAPON_HOLDER.currentWeaponAmmo);
    // TODO: Is there a better way to do this without so much repetition?
    if(PLAYER_WEAPON_HOLDER.currentWeaponAmmo > 0)
    {
        PLAYER_WEAPON_HOLDER.currentWeaponAmmo--;
        switch(PLAYER_WEAPON_HOLDER.currentWeapon)
        {
            case PISTOL:
                PLAYER_WEAPON_HOLDER.PISTOL.ammo--;
                break;

            case RIFLE:
                PLAYER_WEAPON_HOLDER.RIFLE.ammo--;
                break;

            case SHOTGUN:
                PLAYER_WEAPON_HOLDER.SHOTGUN.ammo--;
                break;

            case RAILGUN:
                PLAYER_WEAPON_HOLDER.RAILGUN.ammo--;
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
    if(nextFire > 0)
    {
        nextFire -= GetFrameTime();
    }
    else
    {
        if(WeaponHasAmmo())
        {
            Ray rayCast = GetMouseRay(GetScreenCenter(), *camera);
            int id      = TestEntityHit(rayCast);
            printf("Id hit: %i \n", id);
            if(id != 0 && id != PLAYER_ID)
            {
                TakeDamage(&GetEnemies()[id], PLAYER_WEAPON_HOLDER.currentWeaponDamage);
                printf(
                    "Enemy id %d takes %d damage\n", id, PLAYER_WEAPON_HOLDER.currentWeaponDamage);
            }
        }
        nextFire = PLAYER_WEAPON_HOLDER.currentWeaponFirerate;
    }
    return nextFire;
}

PlayerWeaponHolder GetWeaponData()
{
    return PLAYER_WEAPON_HOLDER;
}