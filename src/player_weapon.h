#ifndef PLAYER_WEAPON
#define PLAYER_WEAPON

#include "include/raylib.h"

//TODO: Add weapon ranges!

typedef struct
{
    char* name;
    int inputKey;
    int damage;
    int ammo;
    float fireRate;
    float range;
    bool pickedUp;

} WeaponData;

typedef struct
{
    //Current Weapon Data
    int currentWeapon;
    float currentWeaponFirerate;
    int currentWeaponDamage;
    int currentWeaponAmmo;

    //Usable weapons
    //We could also use arrays but this way it is easier
    //to be able to remap weapon keys
    WeaponData FIST;
    WeaponData PISTOL;
    WeaponData RIFLE;
    WeaponData SHOTGUN;
    WeaponData RAILGUN;

} PlayerWeaponHolder;

//TODO: Make weapon struct then make those structs into this
//Keeps code cleaner and easier to add/remove weapons
static PlayerWeaponHolder PLAYER_WEAPON_HOLDER =
    {
        //Current weapon data
        .currentWeapon = 0,
        .currentWeaponFirerate = 0.0f,
        .currentWeaponDamage = 0,
        .currentWeaponAmmo = 0,

        //Weapons are declared here
        //Fist
        .FIST.name = "Fists",
        .FIST.inputKey = KEY_ONE,
        .FIST.damage = 5,
        .FIST.ammo = 9999, //Unlimited ammo for your fists!
        .FIST.fireRate = 1.25f,
        .FIST.range = 2.0f,
        .FIST.pickedUp = true, //You always have your fists with you

        //Pistol
        .PISTOL.name = "Pistol",
        .PISTOL.inputKey = KEY_TWO,
        .PISTOL.damage = 3,
        .PISTOL.ammo = 30,
        .PISTOL.fireRate = 1.0f,
        .PISTOL.range = 8.0f,
        .PISTOL.pickedUp = true, //You also always have your trusty pistol with you

        //Rifle
        .RIFLE.name = "Rifle",
        .RIFLE.inputKey = KEY_THREE,
        .RIFLE.damage = 3,
        .RIFLE.ammo = 0,
        .RIFLE.fireRate = 0.9f,
        .RIFLE.range = 20.0f,
        .RIFLE.pickedUp = false,

        //Shotgun
        .SHOTGUN.name = "Shotgun",
        .SHOTGUN.inputKey = KEY_FOUR,
        .SHOTGUN.damage = 7,
        .SHOTGUN.ammo = 0,
        .SHOTGUN.fireRate = 1.5f,
        .SHOTGUN.range = 6.0f,
        .SHOTGUN.pickedUp = false,

        //Railgun
        .RAILGUN.name = "Railgun",
        .RAILGUN.inputKey = KEY_FIVE,
        .RAILGUN.damage = 30,
        .RAILGUN.ammo = 0,
        .RAILGUN.fireRate = 2.6f,
        .RAILGUN.range = 69.0f,
        .RAILGUN.pickedUp = false
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
PlayerWeaponHolder GetWeaponData();

#endif
