#pragma once
#ifndef _WEAPON_H_
#define _WEAPON_H_

#define WEAPON_AMOUNT            5

#include "enemy.h"
#include "level.h"
#include "main.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "utilities.h"
#include "projectile.h"
#include <stdio.h>
#include <time.h>

// Fist has always ammo! :)
#define WEAPON_FIST_AMMO_MAX    9999
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

typedef struct Weapon_Holder
{
    // Current Weapon Data
    int currentWeapon;
    float currentWeaponFirerate;
    int currentWeaponDamage;
    int currentWeaponAmmo;
    int currentWeaponMaxAmmo;
    bool currentWeaponHitscan;
    // Usable weapons
    // We could also use arrays but this way it is easier
    // to be able to remap weapon keys
    Weapon_Data FIST;
    Weapon_Data PISTOL;
    Weapon_Data RIFLE;
    Weapon_Data SHOTGUN;
    Weapon_Data RAILGUN;

} Weapon_Holder;

typedef enum Weapon_ID
{
    FIST    = 1,
    PISTOL  = 2,
    RIFLE   = 3,
    SHOTGUN = 4,
    RAILGUN = 5
} Weapon_ID;

extern struct Weapon_Holder WeaponHolder;
extern struct Weapon_Data Weapon_PlayerWeapons[WEAPON_AMOUNT];

void Weapon_InitializeKeys();
void Weapon_GetSwitchInput();
void Weapon_SelectDefault();
float Weapon_Fire(Camera* camera, float nextFire);
float Weapon_TestLevelHit(Ray rayCast);

#endif
