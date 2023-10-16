#include "weapon.h"

// Prototypes

void Weapon_Change(int weaponId);
bool WeaponHasAmmo(int currentWeapon);

Weapon_Data Weapon_Melee = {
	.name                 = "Fists",
	.inputKey             = KEY_ONE,
	.weaponId             = MELEE,
	.damage               = 5,
	.ammo                 = WEAPON_FIST_AMMO_MAX, // Unlimited ammo for your fists!
	.fireRate             = 1.25f,
	.range                = 2.0f,
	.pickedUp             = true, // You always have your fists with you
	.maxAmmo              = WEAPON_FIST_AMMO_MAX,
	.hitscan              = true,
	.spriteSpeed          = 10,
	.spriteFireFrame      = 1,
	.projectileSize       = (Vector3){ 0, 0, 0 },
	.projectileColor      = WHITE,
	.spritesTotal         = 7,
	.spritePositionOffset = (Vector2){ 0.5f, 1.0f }
};

Weapon_Data Weapon_Pistol = {
	.name                 = "Pistol",
	.inputKey             = KEY_TWO,
	.weaponId             = PISTOL,
	.damage               = 3,
	.ammo                 = 30,
	.fireRate             = 1.0f,
	.range                = 8.0f,
	.pickedUp             = true, // You also always have your trusty pistol with you
	.maxAmmo              = WEAPON_PISTOL_AMMO_MAX,
	.hitscan              = false,
	.spriteSpeed          = 10,
	.spriteFireFrame      = 2,
	.projectileSize       = (Vector3){ 0.05f, 0.05f, 0.2f },
	.projectileColor      = GREEN,
	.spritesTotal         = 5,
	.spritePositionOffset = (Vector2){ -0.10f, 0.75f }
};

Weapon_Data Weapon_Rifle = {
	.name                 = "Rifle",
	.inputKey             = KEY_THREE,
	.weaponId             = RIFLE,
	.damage               = 3,
	.ammo                 = 60, // TODO: Set to 0 for release!
	.fireRate             = 0.3f,
	.range                = 20.0f,
	.pickedUp             = false,
	.maxAmmo              = WEAPON_RIFLE_AMMO_MAX,
	.hitscan              = false,
	.spriteSpeed          = 10,
	.spriteFireFrame      = 2,
	.projectileSize       = (Vector3){ 0.05f, 0.05f, 0.1f },
	.projectileColor      = BLUE,
	.spritesTotal         = 5,
	.spritePositionOffset = (Vector2){ -0.10f, 1.0f }
};

Weapon_Data Weapon_Shotgun = {
	.name                 = "Shotgun",
	.inputKey             = KEY_FOUR,
	.weaponId             = SHOTGUN,
	.damage               = 7,
	.ammo                 = 40, // TODO: Set to 0 for release!
	.fireRate             = 1.5f,
	.range                = 6.0f,
	.pickedUp             = false,
	.maxAmmo              = WEAPON_SHOTGUN_AMMO_MAX,
	.hitscan              = false,
	.spriteSpeed          = 10,
	.spriteFireFrame      = 2,
	.projectileSize       = (Vector3){ 0.3f, 0.05f, 0.05f },
	.projectileColor      = RED,
	.spritesTotal         = 5,
	.spritePositionOffset = (Vector2){ -0.6f, 1.0f }
};

Weapon_Data Weapon_Railgun = {
	.name                 = "Railgun",
	.inputKey             = KEY_FIVE,
	.weaponId             = RAILGUN,
	.damage               = 30,
	.ammo                 = 10, // TODO: Set to 0 for release!
	.fireRate             = 2.6f,
	.range                = 69.0f,
	.pickedUp             = false,
	.maxAmmo              = WEAPON_RAILGUN_AMMO_MAX,
	.hitscan              = false,
	.spriteSpeed          = 10,
	.spriteFireFrame      = 1,
	.projectileSize       = (Vector3){ 0.05f, 0.05f, 0.5f },
	.projectileColor      = YELLOW,
	.spritesTotal         = 5,
	.spritePositionOffset = (Vector2){ -0.10f, 1.0f }
};

// TODO: Make weapon struct then make those structs into this
// Keeps code cleaner and easier to add/remove weapons
struct Weapon_Holder WeaponHolder = {
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
	WeaponHolder.Weapons[MELEE]->inputKey   = Settings.keyWeaponOne;
	WeaponHolder.Weapons[PISTOL]->inputKey  = Settings.keyWeaponTwo;
	WeaponHolder.Weapons[RIFLE]->inputKey   = Settings.keyWeaponThree;
	WeaponHolder.Weapons[SHOTGUN]->inputKey = Settings.keyWeaponFour;
	WeaponHolder.Weapons[RAILGUN]->inputKey = Settings.keyWeaponFive;

	// Initialize sprites
	WeaponHolder.Weapons[MELEE]->spriteTexture   = LoadTexture("./assets/weapons/melee.png");
	WeaponHolder.Weapons[PISTOL]->spriteTexture  = LoadTexture("./assets/weapons/pistol.png");
	WeaponHolder.Weapons[RIFLE]->spriteTexture   = LoadTexture("./assets/weapons/rifle.png");
	WeaponHolder.Weapons[SHOTGUN]->spriteTexture = LoadTexture("./assets/weapons/shotgun.png");
	WeaponHolder.Weapons[RAILGUN]->spriteTexture = LoadTexture("./assets/weapons/railgun.png");
}

void Weapon_SelectDefault(void) { WeaponHolder.currentWeapon = WeaponHolder.Weapons[MELEE]->weaponId; }

void Weapon_GetSwitchInput(void)
{

	const int key = GetKeyPressed();

	for (int i = 0; i < WEAPON_AMOUNT; i++)
	{
		if (key == WeaponHolder.Weapons[i]->inputKey)
		{
			Weapon_Change(WeaponHolder.Weapons[i]->weaponId);
		}
	}
}

// Check if weapon is equipped
void Weapon_Change(const int weaponId)
{
	if (!WeaponHolder.active)
	{
		WeaponHolder.currentWeapon = weaponId;
	}
}

int TestEntityHit(const Ray rayCast)
{
	int id;
	float levelDistance    = INFINITY;
	float enemyDistance    = INFINITY;
	int entitiesAmount     = scene->size;
	Entity *entities      = scene->entities;
	Entity enemyDataHit;

	for (int i = 0; i < entitiesAmount; i++)
	{
		if (entities[i].id != 0)
		{
			if (entities[i].type == SCENE_ACTOR)
			{
				const RayCollision enemyHit = GetRayCollisionBox(rayCast, entities[i].boundingBox);
				if (enemyHit.hit)
				{
					if (!entities[i].actor.dead)
					{
						if (Vector3Length(Vector3Subtract(entities[i].position, rayCast.position)) < enemyDistance)
						{
							enemyDistance = Vector3Length(Vector3Subtract(entities[i].position, rayCast.position));
							enemyDataHit  = entities[i];
						}
					}
				}
			}
			else
			{
				const Vector3 pos = entities[i].position;
				const RayCollision hitLevel =
					GetRayCollisionMesh(rayCast, entities[i].model.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
				if (hitLevel.hit)
				{
					if (hitLevel.distance < levelDistance)
					{
						levelDistance = hitLevel.distance;
					}
				}
			}

		}

	}
	if (enemyDistance < levelDistance)
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

	if (currentWeapon == MELEE)
	{
		return true;
	}
	printf("Ammo: %d \n", WeaponHolder.Weapons[currentWeapon]->ammo);
	// TODO: Is there a better way to do this without so much repetition?
	if (WeaponHolder.Weapons[currentWeapon]->ammo > 0)
	{
		WeaponHolder.Weapons[currentWeapon]->ammo--;
		return true;
	}
	else
	{
		return false;
	}
}

float Weapon_Fire(Camera *camera, float nextFire)
{
	if (nextFire > 0)
	{
		nextFire -= GetFrameTime();
	}
	else
	{
		const Weapon_Data *weapon = WeaponHolder.Weapons[WeaponHolder.currentWeapon];
		if (WeaponHasAmmo(WeaponHolder.currentWeapon) && !WeaponHolder.active)
		{
			WeaponHolder.active       = true;
			WeaponHolder.currentFrame = weapon->spriteFireFrame;

			Ray rayCast  = GetMouseRay(Utilities_GetScreenCenter(), *camera);
			const int id = TestEntityHit(rayCast);

			if (weapon->hitscan)
			{
				printf("Id hit: %i \n", id);
				if (id != 0 && id != PLAYER_ID && scene->entities[id].type == SCENE_ACTOR)
				{
					Entity_TakeDamage(&scene->entities[id], weapon->damage);
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
	const Weapon_Data *weapon = WeaponHolder.Weapons[WeaponHolder.currentWeapon];

	const float frameWidth  = (float)weapon->spriteTexture.width / (float)weapon->spritesTotal;
	const float frameHeight = (float)weapon->spriteTexture.height;
	const Vector2 origin    = { (float)frameWidth / 2, (float)frameHeight };

	const float scale = Utilities_MinF(frameWidth * 2.0f / frameWidth, frameHeight * 2.0f / frameHeight);
	const float posX  = (float)Utilities_GetScreenCenter().x - ((float)frameWidth * weapon->spritePositionOffset.x);
	const float posY  = (float)GetScreenHeight() - ((float)frameHeight * weapon->spritePositionOffset.y);

	Rectangle sourceRec = { 0.0f, 0.0f, frameWidth, frameHeight };
	Rectangle destRec   = { posX, posY, frameWidth * scale, frameHeight * scale };

	if (WeaponHolder.active)
	{
		WeaponHolder.frameCounter++;

		if (WeaponHolder.frameCounter >= GetFPS() / (weapon->spriteSpeed / weapon->fireRate))
		{
			WeaponHolder.currentFrame++;

			if (WeaponHolder.currentFrame >= weapon->spritesTotal)
			{
				WeaponHolder.currentFrame = 0;
				WeaponHolder.active       = false;
			}
			WeaponHolder.frameCounter = 0;
		}
	}

	sourceRec.x = frameWidth * (float)WeaponHolder.currentFrame;

	DrawTexturePro(weapon->spriteTexture, sourceRec, destRec, origin, 0, WHITE);
}
