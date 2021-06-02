#include"../../include/raylib.h"
#include"../../include/raymath.h"
#include"../settings/settings.h"
#include"player.h"
#include"../main.h"
#include<stdio.h>

//Fist has always ammo! :)
#define FIST_AMMO 1 
#define PISTOL_AMMO_MAX 100
#define RIFLE_AMMO_MAX 200
#define SHOTGUN_AMMO_MAX 50
#define ROCKET_AMMO_MAX 25

int weaponEquipped;

typedef struct {
    int fistKey;

    int pistolKey;
    int pistolAmmo;

    int rifleKey;
    int rifleAmmo;

    int shotgunKey;
    int shotgunAmmo;

    int rocketKey;
    int rocketAmmo;   
} WeaponData;

static WeaponData WEAPONDATA = {
    .fistKey = KEY_ONE,
    .pistolKey = KEY_TWO,
    .pistolAmmo = 0,
    .rifleKey = KEY_THREE,
    .rifleAmmo = 0,
    .shotgunKey = KEY_FOUR,
    .shotgunAmmo = 0,
    .rocketKey = KEY_FIVE,
    .rocketAmmo = 0
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
    }
    else if (key == WEAPONDATA.pistolKey)
    {
        weaponEquipped = PISTOL;
    }
    else if (key == WEAPONDATA.rifleKey)
    {
        weaponEquipped = RIFLE;
    }
    else if (key == WEAPONDATA.shotgunKey)
    {
        weaponEquipped = SHOTGUN;
    }
    else if (key == WEAPONDATA.rocketKey)
    {
        weaponEquipped = ROCKET;
    }
    //Weapon switching goes here
    printf("Switched weapon to %d\n", weaponEquipped);
}



void FireWeapon(Vector3 playerPosition, Vector3 target)
{
    Ray rayCast;
    
    Vector3 v = Vector3Normalize(Vector3Subtract(playerPosition,target));
    rayCast.direction = v;
    rayCast.position = playerPosition;
    Model* local = GetAllModels();
    for (int i=0; i < 128; i++)
    {
        RayHitInfo hit = GetCollisionRayModel(rayCast,local[i]);
        
        if (hit.hit)
        {
            printf("Gun hit %f\n", hit.distance);
        }
    }
    
}