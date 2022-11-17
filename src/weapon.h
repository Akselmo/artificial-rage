#pragma once
#ifndef _WEAPON_H_
#define _WEAPON_H_

#define WEAPON_AMOUNT 5

#include "enemy.h"
#include "main.h"
#include "player.h"
#include "projectile.h"
#include "raylib.h"
#include "raymath.h"
#include "scene.h"
#include "settings.h"
#include "utilities.h"
#include <stdio.h>
#include <time.h>

// Fist has always ammo! :)
#define WEAPON_FIST_AMMO_MAX    10
#define WEAPON_PISTOL_AMMO_MAX  100
#define WEAPON_RIFLE_AMMO_MAX   200
#define WEAPON_SHOTGUN_AMMO_MAX 50
#define WEAPON_RAILGUN_AMMO_MAX 25

typedef struct Weapon_Data
{
        char* name;
        int weaponId;
        int inputKey;
        int damage;
        int ammo;
        int maxAmmo;
        float fireRate;
        float range;
        bool pickedUp;
        bool hitscan;

} Weapon_Data;

typedef struct Weapon_DataHolder
{
        // Current Weapon Data
        int currentWeapon;

        // Usable weapons
        // We could also use arrays but this way it is easier
        // to be able to remap weapon keys
        Weapon_Data* Weapons[WEAPON_AMOUNT];
} Weapon_DataHolder;

typedef enum Weapon_ID
{
    FIST    = 0,
    PISTOL  = 1,
    RIFLE   = 2,
    SHOTGUN = 3,
    RAILGUN = 4
} Weapon_ID;

extern struct Weapon_DataHolder WeaponDataHolder;

void Weapon_InitializeKeys();
void Weapon_GetSwitchInput();
void Weapon_SelectDefault();
float Weapon_Fire(Camera* camera, float nextFire);

#endif