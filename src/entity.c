#include "entity.h"

// Entities have shared functions
bool Entity_UpdatePosition(Entity *entity);
bool Entity_TestPlayerHit(Entity *entity);
bool Entity_FireAtPlayer(Entity *entity, float nextFire);
Ray Entity_CreateRay(Entity *entity);

// Private creation functions
void Entity_CreateWall(Entity *entity, char *textureFileName);
void Entity_CreateEnemy(Entity *entity, char *modelFileName);

void Entity_Update(Entity *entity)
{
	Entity_Draw(entity);
	if (entity->type != ENTITY_ENEMY_DEFAULT)
	{
		return;
	}

	if (!entity->actor.dead)
	{
		if (Entity_TestPlayerHit(entity))
		{
			entity->actor.playerSpotted = true;
			if (Entity_FireAtPlayer(entity, entity->actor.nextFire))
			{
				// TODO: instead of directly changing the animation, use an animator that handles
				//       the animation loops
				Animator_SetAnimation(&entity->actor.animator, ATTACK);
			}
			else
			{
				if (Entity_UpdatePosition(entity))
				{
					Animator_SetAnimation(&entity->actor.animator, MOVE);
				}
				else
				{
					Animator_SetAnimation(&entity->actor.animator, IDLE);
				}
			}
			entity->actor.nextFire -= GetFrameTime();
		}
	}
	else
	{
		Animator_SetAnimation(&entity->actor.animator, DEATH);
	}
	entity->actor.animator.nextFrame -= GetFrameTime();
	entity->actor.animator.nextFrame = Animator_PlayAnimation(
		&entity->actor.animator, &entity->model, ACTOR_DEFAULT_ANIMATION_SPEED, entity->actor.animator.nextFrame
	);
}

void Entity_Draw(Entity *entity) { DrawModel(entity->model, entity->position, entity->scale, WHITE); }

Ray Entity_CreateRay(Entity *entity)
{

	const Vector3 playerPosition = Player->position;
	const Vector3 v              = Vector3Normalize(Vector3Subtract(entity->position, playerPosition));

	Ray rayCast = {
		.direction = (Vector3){-1.0f * v.x, -1.0f * v.y, -1.0f * v.z},
          .position = entity->position
	};

	return rayCast;
}

bool Entity_TestPlayerHit(Entity *entity)
{
	// TODO: this function can be quite heavy, could give it a tickrate?
	//  every 1-2 seconds instead of every frame
	if (entity->type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	if (Vector3Distance(Player->position, entity->position) > 5.0f && !entity->actor.playerSpotted)
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
			Vector3 pos = entities[i].position;
			RayCollision hitLevel =
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

	playerDistance = Vector3Length(Vector3Subtract(Player->position, rayCast.position));

	// Player is closer
	hitPlayer = (playerDistance < levelDistance);

	return hitPlayer;
}

// Make this boolean: moving or not
bool Entity_UpdatePosition(Entity *entity)
{
	if (entity->type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	bool moving = true;
	// Move entity towards player
	const Vector3 DistanceFromPlayer = Vector3Subtract(entity->position, Player->position);
	//- Check if player can be seen (first raycast hit returns player)

	//- If in certain range from player, stop
	if (fabsf(DistanceFromPlayer.x) >= ACTOR_MAX_DISTANCE_FROM_PLAYER ||
	    fabsf(DistanceFromPlayer.z) >= ACTOR_MAX_DISTANCE_FROM_PLAYER)
	{
		const Vector3 entityOldPosition = entity->position;
		const Vector3 entityNewPosition = (Vector3){ Player->position.x, ACTOR_POSITION_Y, Player->position.z };
		entity->position =
			Vector3Lerp(entity->position, entityNewPosition, entity->actor.movementSpeed * GetFrameTime());
		if (Scene_CheckCollision(entity->position, entity->size, entity->id))
		{
			entity->position = entityOldPosition;
		}
	}
	else
	{
		moving = false;
	}

	entity->boundingBox = Utilities_MakeBoundingBox(entity->position, entity->size);
	return moving;
}

void Entity_TakeDamage(Entity *entity, const int damageAmount)
{
	if (entity->type != ENTITY_ENEMY_DEFAULT)
	{
		return;
	}

	if (!entity->actor.dead)
	{
		entity->actor.health -= damageAmount;
		printf("entity id %d took %d damage\n", entity->id, damageAmount);
		if (entity->actor.health <= 0)
		{
			// Dirty hack to move bounding box outside of map so it cant be collided to.
			// We want to keep entity in the memory so we can use its position to display the
			// corpse/death anim
			const Vector3 deadBoxPos =
				(Vector3){ ACTOR_GRAVEYARD_POSITION, ACTOR_GRAVEYARD_POSITION, ACTOR_GRAVEYARD_POSITION };
			entity->boundingBox = Utilities_MakeBoundingBox(deadBoxPos, Vector3Zero());
			entity->actor.dead  = true;
		}
	}
}

bool Entity_FireAtPlayer(Entity *entity, float nextFire)
{

	if (entity->type != ENTITY_ENEMY_DEFAULT)
	{
		return false;
	}

	Entity_RotateTowards(entity, Player->position);
	if (nextFire > 0)
	{
		entity->actor.nextFire -= GetFrameTime();
		return false;
	}
	else
	{
		// Fire animation should play before we shoot projectile
		entity->actor.attacking = true;

		Projectile_Create(
			Entity_CreateRay(entity), (Vector3){ 0.2f, 0.2f, 0.2f }, entity->actor.damage, entity->id, PURPLE
		);
		entity->actor.nextFire = entity->actor.fireRate;
		return true;
	}
}

void Entity_RotateTowards(Entity *entity, const Vector3 targetPosition)
{
	if (entity->type == ENTITY_ENEMY_DEFAULT)
	{
		// Rotates the entity around Y axis
		const Vector3 diff        = Vector3Subtract(entity->position, targetPosition);
		const float y_angle       = -(atan2(diff.z, diff.x) + PI / 2.0);
		const Vector3 newRotation = (Vector3){ 0, y_angle, 0 };

		const Quaternion start = QuaternionFromEuler(entity->rotation.z, entity->rotation.y, entity->rotation.x);
		const Quaternion end   = QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x);
		const Quaternion slerp = QuaternionSlerp(start, end, entity->actor.rotationSpeed * GetFrameTime());

		entity->model.transform = QuaternionToMatrix(slerp);
		entity->rotation        = newRotation;
	}
}

Entity Entity_Create(const enum Entity_Type type, const Vector3 position, const int id)
{
	Entity entity = { .type = type, .id = id, .position = position, .scale = 0.5f };

	if (type == ENTITY_WALL_CARGO)
	{
		Entity_CreateWall(&entity, "./assets/textures/wall1.png");
	}

	else if (type == ENTITY_WALL_CARGO_SCUFFED)
	{
		Entity_CreateWall(&entity, "./assets/textures/wall2.png");
	}

	else if (type == ENTITY_ENEMY_DEFAULT)
	{
		Entity_CreateEnemy(&entity, "./assets/models/enemy.m3d");
	}

	return entity;
}

// Creation functions

void Entity_CreateWall(Entity *entity, char *textureFileName)
{
	entity->textureFileName = textureFileName;
	Image textureImage      = LoadImage(entity->textureFileName);
	// The image has to be flipped since its loaded upside down
	ImageFlipVertical(&textureImage);
	const Texture2D texture = LoadTextureFromImage(textureImage);
	// Set map diffuse texture
	const Mesh cube                                           = GenMeshCube(1.0f, 1.0f, 1.0f);
	entity->model                                             = LoadModelFromMesh(cube);
	entity->size                                              = Vector3One();
	entity->scale                                             = 1.0f;

	// Set texture
	entity->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

	entity->boundingBox = Utilities_MakeBoundingBox(entity->position, entity->size);
}

void Entity_CreateEnemy(Entity *entity, char *modelFileName)
{
	const Vector3 entityPosition = (Vector3){ entity->position.x, ACTOR_POSITION_Y, entity->position.z };
	const Vector3 entityRotation = Vector3Zero();
	const Vector3 entitySize     = (Vector3){ 0.25f, 1.1f, 0.25f };
	entity->boundingBox          = Utilities_MakeBoundingBox(entityPosition, entitySize);
	entity->position             = entityPosition;
	entity->rotation             = entityRotation;
	entity->size                 = entitySize;
	entity->scale                = 0.5f;
	entity->model                = LoadModel(modelFileName);
	entity->actor                = Actor_Add(modelFileName, 15, 2); // these could be randomized
}

/*

Entity Entity_CreateItem(const Vector3 position, enum Item_Type type)
{
    Vector3 size = (Vector3){0.2f, 0.2f, 0.2f};

    Item item = Item_Add(type);

    Entity health = { .type        = SCENE_ITEM,
                    .model       = LoadModel(EntityTemplate_itemHealthSmall.modelFileName),
                    .position    = position,
                    .id          = WALL_MODEL_ID,
                    .size        = size,
                    .scale       = 0.2f,
                    .boundingBox = Utilities_MakeBoundingBox(position, size),
                    .item = item
    };

    return health;
}
*/
