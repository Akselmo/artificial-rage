#ifndef PLAYER_WEAPON
#define PLAYER_WEAPON

#include "include/raylib.h"

//TODO: Add weapon ranges!
typedef struct
{
    int weaponEquipped;
    float weaponFireRate;
    int weaponDamage;

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
    .weaponEquipped = 0,
    .weaponFireRate = 0.0f,
    .weaponDamage = 0,

    .fistKey = KEY_ONE,
    .fistFirerate = 1.25f,
    .fistDamage = 5,
    .pistolKey = KEY_TWO,
    .pistolAmmo = 0,
    .pistolFirerate = 1.0f,
    .pistolDamage = 3,
    .rifleKey = KEY_THREE,
    .rifleAmmo = 0,
    .rifleFirerate = 0.9f,
    .rifleDamage = 3,
    .shotgunKey = KEY_FOUR,
    .shotgunAmmo = 0,
    .shotgunFirerate = 1.5f,
    .shotgunDamage = 7,
    .railgunKey = KEY_FIVE,
    .railgunAmmo = 0,
    .railgunFirerate = 2.6f,
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


void InitializeWeaponKeys();
void ChangeWeapon();
void SelectDefaultWeapon();
float FireWeapon(Camera* camera, float nextFire);
float TestLevelHit(Ray rayCast);



#endif
