#include "weapon.h"
#include "level.h"
#include "player.h"
#include "projectile.h"
#include "raymath.h"

//Prototypes

void Weapon_Change(Weapon_Data weapon);

// TODO: Make weapon struct then make those structs into this
// Keeps code cleaner and easier to add/remove weapons
Weapon_Holder WeaponHolder = {
    // Current weapon data
    .currentWeapon         = 0,
    .currentWeaponFirerate = 0.0f,
    .currentWeaponDamage   = 0,
    .currentWeaponAmmo     = 0,
    .currentWeaponMaxAmmo  = 1,
    .currentWeaponHitscan  = false,

    // Weapon_ID are declared here
    // Fist
    .FIST.name     = "Fists",
    .FIST.inputKey = KEY_ONE,
    .FIST.weaponId = FIST,
    .FIST.damage   = 5,
    .FIST.ammo     = WEAPON_FIST_AMMO_MAX,  // Unlimited ammo for your fists!
    .FIST.fireRate = 1.25f,
    .FIST.range    = 2.0f,
    .FIST.pickedUp = true,  // You always have your fists with you
    .FIST.maxAmmo  = WEAPON_FIST_AMMO_MAX,
    .FIST.hitscan  = true,

    // Pistol
    .PISTOL.name     = "Pistol",
    .PISTOL.inputKey = KEY_TWO,
    .PISTOL.weaponId = PISTOL,
    .PISTOL.damage   = 3,
    .PISTOL.ammo     = 30,
    .PISTOL.fireRate = 1.0f,
    .PISTOL.range    = 8.0f,
    .PISTOL.pickedUp = true,  // You also always have your trusty pistol with you
    .PISTOL.maxAmmo  = WEAPON_PISTOL_AMMO_MAX,
    .PISTOL.hitscan  = false,

    // Rifle
    .RIFLE.name     = "Rifle",
    .RIFLE.inputKey = KEY_THREE,
    .RIFLE.weaponId = RIFLE,
    .RIFLE.damage   = 3,
    .RIFLE.ammo     = 0,
    .RIFLE.fireRate = 0.9f,
    .RIFLE.range    = 20.0f,
    .RIFLE.pickedUp = false,
    .RIFLE.maxAmmo  = WEAPON_RIFLE_AMMO_MAX,
    .RIFLE.hitscan  = false,

    // Shotgun
    .SHOTGUN.name     = "Shotgun",
    .SHOTGUN.inputKey = KEY_FOUR,
    .SHOTGUN.weaponId = SHOTGUN,
    .SHOTGUN.damage   = 7,
    .SHOTGUN.ammo     = 0,
    .SHOTGUN.fireRate = 1.5f,
    .SHOTGUN.range    = 6.0f,
    .SHOTGUN.pickedUp = false,
    .SHOTGUN.maxAmmo  = WEAPON_SHOTGUN_AMMO_MAX,
    .SHOTGUN.hitscan  = false,

    // Railgun
    .RAILGUN.name     = "Railgun",
    .RAILGUN.inputKey = KEY_FIVE,
    .RAILGUN.weaponId = RAILGUN,
    .RAILGUN.damage   = 30,
    .RAILGUN.ammo     = 0,
    .RAILGUN.fireRate = 2.6f,
    .RAILGUN.range    = 69.0f,
    .RAILGUN.pickedUp = false,
    .RAILGUN.maxAmmo  = WEAPON_RAILGUN_AMMO_MAX,
    .RAILGUN.hitscan  = false,
};

Weapon_Data Weapon_PlayerWeapons[WEAPON_AMOUNT];

void Weapon_InitializeKeys()
{
    WeaponHolder.FIST.inputKey    = Settings_GetCustomInput(KEY_ONE);
    WeaponHolder.PISTOL.inputKey  = Settings_GetCustomInput(KEY_TWO);
    WeaponHolder.RIFLE.inputKey   = Settings_GetCustomInput(KEY_THREE);
    WeaponHolder.SHOTGUN.inputKey = Settings_GetCustomInput(KEY_FOUR);
    WeaponHolder.RAILGUN.inputKey = Settings_GetCustomInput(KEY_FIVE);
}

void Weapon_SelectDefault()
{
    WeaponHolder.currentWeapon         = FIST;
    WeaponHolder.currentWeaponFirerate = WeaponHolder.FIST.fireRate;
    WeaponHolder.currentWeaponAmmo     = WeaponHolder.FIST.ammo;
    WeaponHolder.currentWeaponDamage   = WeaponHolder.FIST.damage;
    WeaponHolder.currentWeaponMaxAmmo  = WeaponHolder.FIST.maxAmmo;
    WeaponHolder.currentWeaponHitscan  = WeaponHolder.FIST.hitscan;

}

void Weapon_GetSwitchInput()
{
    int key = 0;
    key     = GetKeyPressed();

    for (int i = 0; i < WEAPON_AMOUNT; i++)
    {
        if (key == Weapon_PlayerWeapons[i].inputKey)
        {
            Weapon_Change(Weapon_PlayerWeapons[i]);
        }
    }
}

// Check if weapon is equipped
void Weapon_Change(Weapon_Data weapon)
{
    WeaponHolder.currentWeapon         = weapon.weaponId;
    WeaponHolder.currentWeaponFirerate = weapon.fireRate;
    WeaponHolder.currentWeaponDamage   = weapon.damage;
    WeaponHolder.currentWeaponAmmo     = weapon.ammo;
    WeaponHolder.currentWeaponMaxAmmo  = weapon.maxAmmo;
    WeaponHolder.currentWeaponHitscan  = weapon.hitscan;
 }

int TestEntityHit(Ray rayCast)
{
    int id                = 0;
    float distance        = 0.0f;
    float levelDistance   = INFINITY;
    float enemyDistance   = INFINITY;
    int entitiesAmount    = Level_mapSize;
    Level_Data* levelData = Level_data;
    Enemy_Data* enemies   = Level_enemies;
    Enemy_Data enemyDataHit;
    Level_Data levelDataHit;

    for(int i = 0; i < entitiesAmount; i++)
    {
        if(levelData[i].modelId != 0)
        {
            Vector3 pos           = levelData[i].blockPosition;
            RayCollision hitLevel = GetRayCollisionMesh(
                rayCast, levelData[i].blockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
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

    if(WeaponHolder.currentWeapon == FIST)
    {
        return true;
    }

    switch(WeaponHolder.currentWeapon)
    {
        case PISTOL:
            WeaponHolder.currentWeaponAmmo = WeaponHolder.PISTOL.ammo;
            break;

        case RIFLE:
            WeaponHolder.currentWeaponAmmo = WeaponHolder.RIFLE.ammo;
            break;

        case SHOTGUN:
            WeaponHolder.currentWeaponAmmo = WeaponHolder.SHOTGUN.ammo;
            break;

        case RAILGUN:
            WeaponHolder.currentWeaponAmmo = WeaponHolder.RAILGUN.ammo;
            break;
    }

    printf("Ammo: %d \n", WeaponHolder.currentWeaponAmmo);
    // TODO: Is there a better way to do this without so much repetition?
    if(WeaponHolder.currentWeaponAmmo > 0)
    {
        WeaponHolder.currentWeaponAmmo--;
        switch(WeaponHolder.currentWeapon)
        {
            case PISTOL:
                WeaponHolder.PISTOL.ammo--;
                break;

            case RIFLE:
                WeaponHolder.RIFLE.ammo--;
                break;

            case SHOTGUN:
                WeaponHolder.SHOTGUN.ammo--;
                break;

            case RAILGUN:
                WeaponHolder.RAILGUN.ammo--;
                break;
        }
        return true;
    }
    else
    {
        return false;
    }
}

float Weapon_Fire(Camera* camera, float nextFire)
{
    if(nextFire > 0)
    {
        nextFire -= GetFrameTime();
    }
    else
    {
        if(WeaponHasAmmo())
        {
            //TODO: Raycast or not? If projectile, add projectile to Level_projectiles
            Ray rayCast = GetMouseRay(Utilities_GetScreenCenter(), *camera);
            int id      = TestEntityHit(rayCast);
            if (WeaponHolder.currentWeaponHitscan)
            {
                printf("Id hit: %i \n", id);
                if(id != 0 && id != PLAYER_ID)
                {
                    Enemy_TakeDamage(&Level_enemies[id], WeaponHolder.currentWeaponDamage);
                    printf("Enemy_Data id %d takes %d damage\n", id, WeaponHolder.currentWeaponDamage);
                }
            }
            else
            {
                Projectile_Launch(Vector3Zero(), Vector3Zero());
            }
        }
        nextFire = WeaponHolder.currentWeaponFirerate;
    }
    return nextFire;
}
