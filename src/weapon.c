#include "weapon.h"
#include "level.h"
#include "player.h"
#include "projectile.h"
#include "raymath.h"

// Prototypes

void Weapon_Change(int weaponId);
bool WeaponHasAmmo(int currentWeapon);

Weapon_Data Weapon_Fists = {
    .name     = "Fists",
    .inputKey = KEY_ONE,
    .weaponId = FIST,
    .damage   = 5,
    .ammo     = WEAPON_FIST_AMMO_MAX,  // Unlimited ammo for your fists!
    .fireRate = 1.25f,
    .range    = 2.0f,
    .pickedUp = true,  // You always have your fists with you
    .maxAmmo  = WEAPON_FIST_AMMO_MAX,
    .hitscan  = true,
};

Weapon_Data Weapon_Pistol = {
    .name     = "Pistol",
    .inputKey = KEY_TWO,
    .weaponId = PISTOL,
    .damage   = 3,
    .ammo     = 30,
    .fireRate = 1.0f,
    .range    = 8.0f,
    .pickedUp = true,  // You also always have your trusty pistol with you
    .maxAmmo  = WEAPON_PISTOL_AMMO_MAX,
    .hitscan  = false,
};

Weapon_Data Weapon_Rifle = {
    .name     = "Rifle",
    .inputKey = KEY_THREE,
    .weaponId = RIFLE,
    .damage   = 3,
    .ammo     = 0,
    .fireRate = 0.9f,
    .range    = 20.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_RIFLE_AMMO_MAX,
    .hitscan  = false,
};

Weapon_Data Weapon_Shotgun = {
    .name     = "Shotgun",
    .inputKey = KEY_FOUR,
    .weaponId = SHOTGUN,
    .damage   = 7,
    .ammo     = 0,
    .fireRate = 1.5f,
    .range    = 6.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_SHOTGUN_AMMO_MAX,
    .hitscan  = false,
};

Weapon_Data Weapon_Railgun = {
    .name     = "Railgun",
    .inputKey = KEY_FIVE,
    .weaponId = RAILGUN,
    .damage   = 30,
    .ammo     = 0,
    .fireRate = 2.6f,
    .range    = 69.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_RAILGUN_AMMO_MAX,
    .hitscan  = false,
};

// TODO: Make weapon struct then make those structs into this
// Keeps code cleaner and easier to add/remove weapons
struct Weapon_DataHolder WeaponDataHolder = {
    // Current weapon data
    .currentWeapon    = 0,
    .Weapons[FIST]    = &Weapon_Fists,
    .Weapons[PISTOL]  = &Weapon_Pistol,
    .Weapons[RIFLE]   = &Weapon_Rifle,
    .Weapons[SHOTGUN] = &Weapon_Shotgun,
    .Weapons[RAILGUN] = &Weapon_Railgun,
};

void Weapon_InitializeKeys()
{
    WeaponDataHolder.Weapons[FIST]->inputKey    = Settings_GetCustomInput(KEY_ONE);
    WeaponDataHolder.Weapons[PISTOL]->inputKey  = Settings_GetCustomInput(KEY_TWO);
    WeaponDataHolder.Weapons[RIFLE]->inputKey   = Settings_GetCustomInput(KEY_THREE);
    WeaponDataHolder.Weapons[SHOTGUN]->inputKey = Settings_GetCustomInput(KEY_FOUR);
    WeaponDataHolder.Weapons[RAILGUN]->inputKey = Settings_GetCustomInput(KEY_FIVE);
}

void Weapon_SelectDefault()
{
    WeaponDataHolder.currentWeapon = WeaponDataHolder.Weapons[FIST]->weaponId;
}

void Weapon_GetSwitchInput()
{
    int key = 0;
    key     = GetKeyPressed();

    for(int i = 0; i < WEAPON_AMOUNT; i++)
    {
        if(key == WeaponDataHolder.Weapons[i]->inputKey)
        {
            Weapon_Change(WeaponDataHolder.Weapons[i]->weaponId);
        }
    }
}

// Check if weapon is equipped
void Weapon_Change(int weaponId)
{
    WeaponDataHolder.currentWeapon = weaponId;
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
            RayCollision hitLevel = GetRayCollisionMesh(rayCast, levelData[i].blockModel.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
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
                if(Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position)) < enemyDistance)
                {
                    enemyDistance = Vector3Length(Vector3Subtract(enemies[i].position, rayCast.position));
                    enemyDataHit  = enemies[i];
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

bool WeaponHasAmmo(int currentWeapon)
{

    if(currentWeapon == FIST)
    {
        return true;
    }
    printf("Ammo: %d \n", WeaponDataHolder.Weapons[currentWeapon]->ammo);
    // TODO: Is there a better way to do this without so much repetition?
    if(WeaponDataHolder.Weapons[currentWeapon]->ammo > 0)
    {
        WeaponDataHolder.Weapons[currentWeapon]->ammo--;
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
        if(WeaponHasAmmo(WeaponDataHolder.currentWeapon))
        {
            // TODO: Raycast or not? If projectile, add projectile to Level_projectiles
            Ray rayCast = GetMouseRay(Utilities_GetScreenCenter(), *camera);
            int id      = TestEntityHit(rayCast);
            if(WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->hitscan)
            {
                printf("Id hit: %i \n", id);
                if(id != 0 && id != PLAYER_ID)
                {
                    Enemy_TakeDamage(&Level_enemies[id], WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage);
                    printf("Enemy_Data id %d takes %d damage\n", id, WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage);
                }
            }
            else
            {
                Projectile_Create(rayCast, Vector3One(), WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage);
            }
        }
        nextFire = WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->fireRate;
    }
    return nextFire;
}
