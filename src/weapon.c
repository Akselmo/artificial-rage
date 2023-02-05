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
    .spriteSpeed = 10,
    .spriteFireFrame = 1,
    .projectileSize = (Vector3){0,0,0},
    .projectileColor = WHITE
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
    .spriteSpeed = 10,
    .spriteFireFrame = 2,
    .projectileSize = (Vector3){0.05f,0.05f,0.2f},
    .projectileColor = GREEN
};

Weapon_Data Weapon_Rifle = {
    .name     = "Rifle",
    .inputKey = KEY_THREE,
    .weaponId = RIFLE,
    .damage   = 3,
    .ammo     = 60,     //TODO: Set to 0 for release!
    .fireRate = 0.3f,
    .range    = 20.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_RIFLE_AMMO_MAX,
    .hitscan  = false,
    .spriteSpeed = 10,
    .spriteFireFrame = 2,
    .projectileSize = (Vector3){0.05f,0.05f,0.1f},
    .projectileColor = BLUE
};

Weapon_Data Weapon_Shotgun = {
    .name     = "Shotgun",
    .inputKey = KEY_FOUR,
    .weaponId = SHOTGUN,
    .damage   = 7,
    .ammo     = 40, //TODO: Set to 0 for release!
    .fireRate = 1.5f,
    .range    = 6.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_SHOTGUN_AMMO_MAX,
    .hitscan  = false,
    .spriteSpeed = 10,
    .spriteFireFrame = 2,
    .projectileSize = (Vector3){0.3f,0.05f,0.05f},
    .projectileColor = RED
};

Weapon_Data Weapon_Railgun = {
    .name     = "Railgun",
    .inputKey = KEY_FIVE,
    .weaponId = RAILGUN,
    .damage   = 30,
    .ammo     = 10, //TODO: Set to 0 for release!
    .fireRate = 2.6f,
    .range    = 69.0f,
    .pickedUp = false,
    .maxAmmo  = WEAPON_RAILGUN_AMMO_MAX,
    .hitscan  = false,
    .spriteSpeed = 10,
    .spriteFireFrame = 1,
    .projectileSize = (Vector3){0.05f,0.05f,0.5f},
    .projectileColor = YELLOW
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
    .currentFrame     = 0,
    .frameCounter     = 0,
    .active           = false
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
    WeaponDataHolder.Weapons[MELEE]->spriteTexture   = LoadTexture("./assets/weapons/melee.png");
    WeaponDataHolder.Weapons[PISTOL]->spriteTexture  = LoadTexture("./assets/weapons/pistol.png");
    WeaponDataHolder.Weapons[RIFLE]->spriteTexture   = LoadTexture("./assets/weapons/rifle.png");
    WeaponDataHolder.Weapons[SHOTGUN]->spriteTexture = LoadTexture("./assets/weapons/shotgun.png");
    WeaponDataHolder.Weapons[RAILGUN]->spriteTexture = LoadTexture("./assets/weapons/railgun.png");
    // Add total sprites
    WeaponDataHolder.Weapons[MELEE]->spritesTotal   = 4;
    WeaponDataHolder.Weapons[PISTOL]->spritesTotal  = 5;
    WeaponDataHolder.Weapons[RIFLE]->spritesTotal   = 5;
    WeaponDataHolder.Weapons[SHOTGUN]->spritesTotal = 5;
    WeaponDataHolder.Weapons[RAILGUN]->spritesTotal = 5;
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
    if(!WeaponDataHolder.active)
    {
        WeaponDataHolder.currentWeapon = weaponId;
    }
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
        const Weapon_Data* weapon = WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon];
        if(WeaponHasAmmo(WeaponDataHolder.currentWeapon) && !WeaponDataHolder.active)
        {
            WeaponDataHolder.active       = true;
            WeaponDataHolder.currentFrame = weapon->spriteFireFrame;

            Ray rayCast       = GetMouseRay(Utilities_GetScreenCenter(), *camera);
            const int id      = TestEntityHit(rayCast);

            if(weapon->hitscan)
            {
                printf("Id hit: %i \n", id);
                if(id != 0 && id != PLAYER_ID)
                {
                    Actor_TakeDamage(&Scene_data.actors[id], weapon->damage);
                    printf("Enemy_Data id %d takes %d damage\n", id, weapon->damage);
                }
            }
            else
            {
                // Move raycast start position a bit further from player if firing a projectile
                rayCast.position = Vector3Add(rayCast.position, Vector3Scale(rayCast.direction, 0.1f));
                Projectile_Create(rayCast, weapon->projectileSize, weapon->damage, PLAYER_ID, weapon->projectileColor);
            }
        }
        nextFire = weapon->fireRate;
    }
    return nextFire;
}

void Weapon_DrawSprite(void)
{
    const Weapon_Data* weapon = WeaponDataHolder.Weapons[WeaponDataHolder.currentWeapon];

    const float frameWidth  = (float)weapon->spriteTexture.width / (float)weapon->spritesTotal;
    const float frameHeight = (float)weapon->spriteTexture.height;
    const Vector2 origin    = { (float)frameWidth, (float)frameHeight };
    const float posX        = Utilities_GetScreenCenter().x + (float)frameWidth / 1.3f;
    const float posY        = (float)GetScreenHeight() - (float)frameHeight / 1.3f;
    const float scale       = 2.0f;

    Rectangle sourceRec = { 0.0f, 0.0f, frameWidth, frameHeight };
    Rectangle destRec   = { posX, posY, frameWidth * scale, frameHeight * scale };

    if(WeaponDataHolder.active)
    {
        WeaponDataHolder.frameCounter++;

        if(WeaponDataHolder.frameCounter >= GetFPS() / (weapon->spriteSpeed / weapon->fireRate))
        {
            WeaponDataHolder.currentFrame++;

            if(WeaponDataHolder.currentFrame >= weapon->spritesTotal)
            {
                WeaponDataHolder.currentFrame = 0;
                WeaponDataHolder.active       = false;
            }
            WeaponDataHolder.frameCounter = 0;
        }
    }

    sourceRec.x = frameWidth * (float)WeaponDataHolder.currentFrame;

    DrawTexturePro(weapon->spriteTexture, sourceRec, destRec, origin, 0, WHITE);
}
