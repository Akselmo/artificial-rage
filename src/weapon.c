#include "weapon.h"
#include "player.h"
#include "projectile.h"
#include "raymath.h"
#include "scene.h"

// Prototypes

void Weapon_Change(int weaponId);
bool WeaponHasAmmo(int currentWeapon);

// clang-format off
Weapon_Data Weapon_Melee = {
    .name     = "Fists",
    .inputKey = KEY_ONE,
    .weaponId = MELEE,
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

// clang-format on

// TODO: Make weapon struct then make those structs into this
// Keeps code cleaner and easier to add/remove weapons
struct Weapon_DataHolder WeaponDataHolder = {
    // Current weapon data
    .currentWeapon    = 0,
    .Weapons[MELEE]   = &Weapon_Melee,
    .Weapons[PISTOL]  = &Weapon_Pistol,
    .Weapons[RIFLE]   = &Weapon_Rifle,
    .Weapons[SHOTGUN] = &Weapon_Shotgun,
    .Weapons[RAILGUN] = &Weapon_Railgun,
};

void Weapon_Initialize(void)
{
    // Initialize keys
    WeaponDataHolder.Weapons[MELEE]->inputKey   = Settings.keyWeaponOne;
    WeaponDataHolder.Weapons[PISTOL]->inputKey  = Settings.keyWeaponTwo;
    WeaponDataHolder.Weapons[RIFLE]->inputKey   = Settings.keyWeaponThree;
    WeaponDataHolder.Weapons[SHOTGUN]->inputKey = Settings.keyWeaponFour;
    WeaponDataHolder.Weapons[RAILGUN]->inputKey = Settings.keyWeaponFive;

    // Initialize sprites
    WeaponDataHolder.Weapons[MELEE]->weaponSprite   = LoadTexture("./assets/weapons/melee.png");
    WeaponDataHolder.Weapons[PISTOL]->weaponSprite  = LoadTexture("./assets/weapons/pistol.png");
    WeaponDataHolder.Weapons[RIFLE]->weaponSprite   = LoadTexture("./assets/weapons/rifle.png");
    WeaponDataHolder.Weapons[SHOTGUN]->weaponSprite = LoadTexture("./assets/weapons/shotgun.png");
    WeaponDataHolder.Weapons[RAILGUN]->weaponSprite = LoadTexture("./assets/weapons/railgun.png");
    // Add total sprites
    WeaponDataHolder.Weapons[MELEE]->weaponSpritesTotal   = 4;
    WeaponDataHolder.Weapons[PISTOL]->weaponSpritesTotal  = 5;
    WeaponDataHolder.Weapons[RIFLE]->weaponSpritesTotal   = 5;
    WeaponDataHolder.Weapons[SHOTGUN]->weaponSpritesTotal = 5;
    WeaponDataHolder.Weapons[RAILGUN]->weaponSpritesTotal = 5;
}

void Weapon_SelectDefault(void)
{
    WeaponDataHolder.currentWeapon = WeaponDataHolder.Weapons[MELEE]->weaponId;
}

void Weapon_GetSwitchInput(void)
{

    const int key = GetKeyPressed();

    for(int i = 0; i < WEAPON_AMOUNT; i++)
    {
        if(key == WeaponDataHolder.Weapons[i]->inputKey)
        {
            Weapon_Change(WeaponDataHolder.Weapons[i]->weaponId);
        }
    }
}

// Check if weapon is equipped
void Weapon_Change(const int weaponId)
{
    WeaponDataHolder.currentWeapon = weaponId;
}

int TestEntityHit(const Ray rayCast)
{
    int id;
    float levelDistance        = INFINITY;
    float enemyDistance        = INFINITY;
    int entitiesAmount         = Scene_data.size;
    Scene_BlockData* levelData = Scene_data.blocks;
    Actor_Data* enemies        = Scene_data.actors;
    Actor_Data enemyDataHit;

    for(int i = 0; i < entitiesAmount; i++)
    {
        if(levelData[i].id != 0)
        {
            const Vector3 pos           = levelData[i].position;
            const RayCollision hitLevel = GetRayCollisionMesh(rayCast, levelData[i].model.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
            if(hitLevel.hit)
            {
                if(hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                }
            }
        }
        const RayCollision enemyHit = GetRayCollisionBox(rayCast, enemies[i].boundingBox);
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

bool WeaponHasAmmo(const int currentWeapon)
{

    if(currentWeapon == MELEE)
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
            const Ray rayCast = GetMouseRay(Utilities_GetScreenCenter(), *camera);
            const int id      = TestEntityHit(rayCast);
            if(WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->hitscan)
            {
                printf("Id hit: %i \n", id);
                if(id != 0 && id != PLAYER_ID)
                {
                    Actor_TakeDamage(&Scene_data.actors[id], WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage);
                    printf("Enemy_Data id %d takes %d damage\n", id, WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage);
                }
            }
            else
            {
                Projectile_Create(rayCast, (Vector3) { 0.2f, 0.2f, 0.2f }, WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->damage, PLAYER_ID);
            }
        }
        nextFire = WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon]->fireRate;
    }
    return nextFire;
}

void Weapon_DrawSprite(void)
{
    const Weapon_Data* weapon = WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon];

    const float frameWidth  = (float)weapon->weaponSprite.width / (float)weapon->weaponSpritesTotal;
    const float frameHeight = (float)weapon->weaponSprite.height;
    const Vector2 origin    = { (float)frameWidth, (float)frameHeight };
    const float posX        = Utilities_GetScreenCenter().x + (float)frameWidth / 1.3f;
    const float posY        = (float)GetScreenHeight() - (float)frameHeight / 1.3f;
    const float scale       = 2.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, frameWidth, frameHeight };
    Rectangle destRec   = { posX, posY, frameWidth * scale, frameHeight * scale };

    DrawTexturePro(weapon->weaponSprite, sourceRec, destRec, origin, 0, WHITE);
}
