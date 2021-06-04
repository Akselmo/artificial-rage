#include"../../include/raylib.h"
#include"../../include/raymath.h"
#include"../settings/settings.h"
#include"../level/level.h"
#include"player.h"
#include"../main.h"
#include<stdio.h>
#include <time.h>

//Fist has always ammo! :)
#define FIST_AMMO 1 
#define PISTOL_AMMO_MAX 100
#define RIFLE_AMMO_MAX 200
#define SHOTGUN_AMMO_MAX 50
#define ROCKET_AMMO_MAX 25

int weaponEquipped;
float fireRate = 1.0f;
float nextFire = -1.0f;

typedef struct {
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

    int rocketKey;
    int rocketAmmo;   
    float rocketFirerate;
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
    .rocketKey = KEY_FIVE,
    .rocketAmmo = 0,
    .rocketFirerate = 1.0f
};


typedef enum {
    FIST = 1,
    PISTOL = 2,
    RIFLE = 3,
    SHOTGUN = 4,
    ROCKET = 5
} Weapons;


void InitializeWeaponKeys()
{
    WEAPONDATA.fistKey = GetCustomInput(KEY_ONE);
    WEAPONDATA.pistolKey = GetCustomInput(KEY_TWO);
    WEAPONDATA.rifleKey = GetCustomInput(KEY_THREE);
    WEAPONDATA.shotgunKey = GetCustomInput(KEY_FOUR);
    WEAPONDATA.rocketKey = GetCustomInput(KEY_FIVE);
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
    else if (key == WEAPONDATA.rocketKey)
    {
        weaponEquipped = ROCKET;
        fireRate = WEAPONDATA.rocketFirerate;
    }
    //Weapon switching goes here
}

int id;

float TestLevelHit(Ray rayCast)
{
    float distance = INFINITY;
    LevelData* levelData = GetLevelData();
    //Make sure the level size is all the cubes
    for (int i=0; i < GetLevelBlockAmount(); i++)
    {

        Vector3 pos = levelData[i].levelBlockPosition;
        RayHitInfo hitLevel = GetCollisionRayMesh(rayCast,levelData[i].levelBlockModel.meshes[0],MatrixTranslate(pos.x,pos.y,pos.z));
        if (hitLevel.hit)
        {
            if (hitLevel.distance < distance)
            {
                distance = hitLevel.distance;
                id = levelData[i].modelId;
            }
        } 
        
    }
    return distance;
}

float TestEntityHit(Ray rayCast, int entityAmount)
{
    float distance = INFINITY;
    Model* entities = GetAllEntities();
    for (int i=0; i < entityAmount; i++)
    {

        //See if we need meshes here too
        RayHitInfo hitEntities = GetCollisionRayModel(rayCast,entities[i]);
        if (hitEntities.hit)
        {
            if (hitEntities.distance < distance)
            {
                distance = hitEntities.distance;
            }
        }
    }
    return distance;
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

        case ROCKET:
            ammo = WEAPONDATA.rocketAmmo;
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
            
            Vector3 v = Vector3Normalize(Vector3Subtract(playerPosition,target));
            rayCast.direction = v;
            rayCast.position = playerPosition;

            float levelDistance = TestLevelHit(rayCast);
            float entityDistance = TestEntityHit(rayCast, entities);    

            if (levelDistance != INFINITY ||entityDistance != INFINITY)
            {
                if (levelDistance < entityDistance)
                {
                    //We hit wall before we hit anything
                }
                else
                {
                    //we hit the entity instead
                }
                //printf("Levelhit %f Entityhit %f \n", levelDistance, entityDistance);
                printf("id: %d\n", id);
                nextFire = fireRate;
            }
        }
    }

}

