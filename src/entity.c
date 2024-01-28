#include "entity.h"
#include "animator.h"
#include "player.h"
#include "raymath.h"
// Entities have shared functions
bool Entity_UpdatePosition(Entity *entity);
bool Entity_TestPlayerHit(Entity *entity);
bool Entity_FireAtPlayer(Entity *entity, float nextFire);
Ray Entity_CreateRay(Entity *entity);

// Private creation functions
void Entity_SetupTransform(Entity *entity, Vector3 pos, Vector3 rot, Vector3 size, float scale);
void Entity_CreateWall(Entity *entity);
void Entity_CreateEnemy(Entity *entity);
void Entity_CreateItem(Entity *entity, bool pickup, int value);

void Entity_Update(Entity *entity)
{
	Entity_Draw(entity);
	if (entity->data.type != ENTITY_ENEMY_DEFAULT)
	{
		return;
	}

	if (!entity->data.value.actor.dead)
	{
		if (Entity_TestPlayerHit(entity))
		{
			entity->data.value.actor.playerSpotted = true;
			if (Entity_FireAtPlayer(entity, entity->data.value.actor.nextFire))
			{
				Animator_SetAnimation(&entity->data.value.actor.animator, ENEMY_ATTACK);
			}
			else
			{
				if (Entity_UpdatePosition(entity))
				{
					Animator_SetAnimation(&entity->data.value.actor.animator, ENEMY_MOVE);
				}
				else
				{
					Animator_SetAnimation(&entity->data.value.actor.animator, ENEMY_IDLE);
				}
			}
			entity->data.value.actor.nextFire -= GetFrameTime();
		}
	}
	else
	{
		Animator_SetAnimation(&entity->data.value.actor.animator, ENEMY_DEATH);
	}
	entity->data.value.actor.animator.nextFrame -= GetFrameTime();
	entity->data.value.actor.animator.nextFrame = Animator_PlayAnimation(
		&entity->data.value.actor.animator,
		&entity->model.data,
		ACTOR_DEFAULT_ANIMATION_SPEED,
		entity->data.value.actor.animator.nextFrame
	);
}

void Entity_Draw(Entity *entity)
{
	DrawModel(entity->model.data, entity->transform.position, entity->transform.scale, WHITE);
}

Ray Entity_CreateRay(Entity *entity)
{

	const Vector3 playerPosition = Player->transform.position;
	const Vector3 v              = Vector3Normalize(Vector3Subtract(entity->transform.position, playerPosition));

	Ray rayCast = {
		.direction = (Vector3){-1.0f * v.x, -1.0f * v.y, -1.0f * v.z},
          .position = entity->transform.position
	};

	return rayCast;
}

bool Entity_TestPlayerHit(Entity *entity)
{
	// TODO: this function can be quite heavy, could give it a tickrate?
	//  every 1-2 seconds instead of every frame
	if (entity->data.type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	if (Vector3Distance(Player->transform.position, entity->transform.position) > 5.0f &&
	    !entity->data.value.actor.playerSpotted)
	{
		return false;
	}
	const Ray rayCast = Entity_CreateRay(entity);

	bool hitPlayer           = false;
	float levelDistance      = INFINITY;
	float playerDistance     = INFINITY;
	const int entitiesAmount = scene->size;
	const Entity *entities   = scene->entities;

	for (int i = 0; i < entitiesAmount; i++)
	{
		if (entities[i].id != 0 && entities[i].id != entity->id)
		{
			Vector3 pos = entities[i].transform.position;
			RayCollision hitLevel =
				GetRayCollisionMesh(rayCast, entities[i].model.data.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
			if (hitLevel.hit)
			{
				if (hitLevel.distance < levelDistance)
				{
					levelDistance = hitLevel.distance;
				}
			}
		}
	}

	playerDistance = Vector3Length(Vector3Subtract(Player->transform.position, rayCast.position));

	// Player is closer
	hitPlayer = (playerDistance < levelDistance);

	return hitPlayer;
}

// Make this boolean: moving or not
bool Entity_UpdatePosition(Entity *entity)
{
	if (entity->data.type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	bool moving = true;
	// Move entity towards player
	const Vector3 DistanceFromPlayer = Vector3Subtract(entity->transform.position, Player->transform.position);
	//- Check if player can be seen (first raycast hit returns player)

	//- If in certain range from player, stop
	if (fabsf(DistanceFromPlayer.x) >= ACTOR_MAX_DISTANCE_FROM_PLAYER ||
	    fabsf(DistanceFromPlayer.z) >= ACTOR_MAX_DISTANCE_FROM_PLAYER)
	{
		const Vector3 entityOldPosition = entity->transform.position;
		const Vector3 entityNewPosition =
			(Vector3){ Player->transform.position.x, ACTOR_POSITION_Y, Player->transform.position.z };
		entity->transform.position = Vector3Lerp(
			entity->transform.position, entityNewPosition, entity->data.value.actor.movementSpeed * GetFrameTime()
		);
		if (Scene_CheckCollision(entity->transform.position, entity->transform.size, entity->id))
		{
			entity->transform.position = entityOldPosition;
		}
	}
	else
	{
		moving = false;
	}

	entity->transform.boundingBox = Utilities_MakeBoundingBox(entity->transform.position, entity->transform.size);
	return moving;
}

void Entity_TakeDamage(Entity *entity, const int damageAmount)
{
	if (entity->data.type != ENTITY_ENEMY_DEFAULT)
	{
		return;
	}

	if (!entity->data.value.actor.dead)
	{
		// "Wake up" enemy if it gets hit by a projectile
		if (!entity->data.value.actor.playerSpotted)
		{
			entity->data.value.actor.playerSpotted = true;
		}
		entity->data.value.actor.health -= damageAmount;
		printf("entity id %d took %d damage\n", entity->id, damageAmount);
		if (entity->data.value.actor.health <= 0)
		{
			// HACK: Dirty hack to move bounding box outside of map so it cant be collided to.
			// We want to keep entity in the memory so we can use its position to display the
			// corpse/death anim
			const Vector3 deadBoxPos =
				(Vector3){ ACTOR_GRAVEYARD_POSITION, ACTOR_GRAVEYARD_POSITION, ACTOR_GRAVEYARD_POSITION };
			entity->transform.boundingBox = Utilities_MakeBoundingBox(deadBoxPos, Vector3Zero());
			entity->data.value.actor.dead = true;
		}
	}
}

bool Entity_FireAtPlayer(Entity *entity, float nextFire)
{

	if (entity->data.type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	Entity_RotateTowards(entity, Player->transform.position);
	if (nextFire > 0)
	{
		entity->data.value.actor.nextFire -= GetFrameTime();
		return false;
	}
	else
	{
		// Fire animation should play before we shoot projectile
		entity->data.value.actor.attacking = true;

		Projectile_Create(
			Entity_CreateRay(entity), (Vector3){ 0.2f, 0.2f, 0.2f }, entity->data.value.actor.damage, entity->id, PURPLE
		);
		entity->data.value.actor.nextFire = entity->data.value.actor.fireRate;
		return true;
	}
}

void Entity_RotateTowards(Entity *entity, const Vector3 targetPosition)
{
	if (entity->data.type == ENTITY_ENEMY_DEFAULT)
	{
		// Rotates the entity around Y axis
		const Vector3 diff        = Vector3Subtract(entity->transform.position, targetPosition);
		const float y_angle       = -(atan2(diff.z, diff.x) + PI / 2.0);
		const Vector3 newRotation = (Vector3){ 0, y_angle, 0 };

		const Quaternion start = QuaternionFromEuler(
			entity->transform.rotation.z, entity->transform.rotation.y, entity->transform.rotation.x
		);
		const Quaternion end   = QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x);
		const Quaternion slerp = QuaternionSlerp(start, end, entity->data.value.actor.rotationSpeed * GetFrameTime());

		entity->model.data.transform = QuaternionToMatrix(slerp);
		entity->transform.rotation   = newRotation;
	}
}

Entity Entity_Create(const enum Entity_Type type, const Vector3 position, const int id)
{
	Entity entity = { .data.type = type, .id = id, .transform.position = position };

	if (type == ENTITY_WALL_CARGO)
	{
		entity.model.textureFileName = "./assets/textures/wall1.png";
		Entity_CreateWall(&entity);
	}

	else if (type == ENTITY_WALL_CARGO_SCUFFED)
	{
		entity.model.textureFileName = "./assets/textures/wall2.png";
		Entity_CreateWall(&entity);
	}

	else if (type == ENTITY_ENEMY_DEFAULT)
	{
		entity.model.fileName = "./assets/models/enemy.m3d";
		Entity_CreateEnemy(&entity);
	}

	else if (type == ENTITY_ITEM_HEALTH_SMALL)
	{
		entity.model.isBillboard = true;
		entity.model.fileName    = "./assets/billboards/health.png";
		Entity_CreateItem(&entity, true, 10);
	}

	return entity;
}

// Creation functions

void Entity_SetupTransform(Entity *entity, Vector3 pos, Vector3 rot, Vector3 size, float scale)
{
	entity->transform.boundingBox = Utilities_MakeBoundingBox(pos, size);
	entity->transform.position    = pos;
	entity->transform.rotation    = rot;
	entity->transform.size        = size;
	entity->transform.scale       = scale;
}

void Entity_CreateWall(Entity *entity)
{
	Image textureImage = LoadImage(entity->model.textureFileName);
	// The image has to be flipped since its loaded upside down
	ImageFlipVertical(&textureImage);
	const Texture2D texture = LoadTextureFromImage(textureImage);
	// Set map diffuse texture
	const Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	entity->model.data      = LoadModelFromMesh(cube);
	Entity_SetupTransform(entity, entity->transform.position, Vector3Zero(), Vector3One(), 1.0f);

	// Set texture
	entity->model.data.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void Entity_CreateEnemy(Entity *entity)
{
	const Vector3 entityPosition =
		(Vector3){ entity->transform.position.x, ACTOR_POSITION_Y, entity->transform.position.z };
	const Vector3 entitySize     = (Vector3){ 0.25f, 1.1f, 0.25f };
	Entity_SetupTransform(entity, entityPosition, Vector3Zero(), entitySize, 0.5);
	entity->model.data            = LoadModel(entity->model.fileName);

	Actor actor = { .dead          = false,
		            .moving        = false,
		            .attacking     = false,
		            .playerSpotted = false,
		            .damage        = 2,
		            .health        = 15, // Check actor health balance later
		            .movementSpeed = ACTOR_DEFAULT_MOVEMENT_SPEED,
		            .rotationSpeed = ACTOR_DEFAULT_ROTATION_SPEED,
		            .fireRate      = 5.75f,
		            .nextFire      = 5.75f,
		            .animator      = Animator_EnemyAnimations(entity->model.fileName) };

	entity->data.value.actor = actor;
}

void Entity_CreateItem(Entity *entity, bool pickup, int value)
{
	Item item = { .destroyed = false, .value = value, .pickup = pickup };
	const Vector3 entityPosition =
		(Vector3){ entity->transform.position.x, ITEM_START_POSITION_Y, entity->transform.position.z };
	Entity_SetupTransform(entity, entityPosition, Vector3Zero(), Vector3One(), 1.0);
	entity->data.value.item = item;
}
