#include "actor.h"
#include "player.h"
#include "projectile.h"
#include "raylib.h"

// Private functions
bool Actor_UpdatePosition(Actor_Data* actor);
bool Actor_TestPlayerHit(Actor_Data* actor);
float Actor_FireAtPlayer(Actor_Data* actor, float nextFire);
Ray Actor_CreateRay(Actor_Data* actor);
void Actor_PlayAnimation(Actor_Data* actor, float animationSpeed);
// TODO: move all the functions in the following four functions
//  If something is reused, make an utility function for it?
void Actor_Attack(Actor_Data* actor);
void Actor_Move(Actor_Data* actor);
void Actor_Idle(Actor_Data* actor);
void Actor_Die(Actor_Data* actor);

// TODO: Rotation
Actor_Data Actor_Add(float pos_x, float pos_z, int id)
{
    Vector3 actorPosition = (Vector3) { pos_x, ACTOR_POSITION_Y, pos_z };
    Vector3 actorRotation = Vector3Zero();
    Vector3 actorSize     = (Vector3) { 0.25f, 0.8f, 0.25f };
    float randomTickRate  = ((float)rand() / (float)(RAND_MAX)) * 2;

    const char modelFileName[128] = "./assets/models/actor.m3d";
    unsigned int animationsCount  = 0;

    Actor_Data actor = {
        .position         = actorPosition,
        .rotation         = actorRotation,
        .size             = actorSize,
        .model            = LoadModel(modelFileName),
        .animations       = LoadModelAnimations(modelFileName, &animationsCount),
        .animationsCount  = animationsCount,
        .currentAnimation = IDLE,
        .dead             = false,
        .moving           = false,
        .attacking        = false,
        .damage           = 5,
        .health           = 15,  // Check actor health balance later
        .boundingBox      = Utilities_MakeBoundingBox(actorPosition, actorSize),
        .id               = id,
        .tickRate         = randomTickRate,
        .nextTick         = -1.0f,
        .movementSpeed    = ACTOR_DEFAULT_MOVEMENT_SPEED,
        .rotationSpeed    = ACTOR_DEFAULT_ROTATION_SPEED,
        .fireRate         = 5.75f,
        .nextFire         = 5.75f,
    };
    return actor;
}

// TODO: The animations need to be tied completely to the
// firing and moving.
// First we start playing animation, then we start doing the action related to animation

void Actor_Update(Actor_Data* actor)
{

    if(!actor->dead)
    {
        Actor_Draw(actor);
        if(actor->nextTick > 0)
        {
            actor->nextTick -= GetFrameTime();
        }
        else
        {
            actor->nextTick = actor->tickRate;
        }

        actor->moving = Actor_UpdatePosition(actor);

        if(actor->attacking)
        {
            actor->currentAnimation = ATTACK;
        }
        else
        {
            if(actor->moving)
            {
                actor->currentAnimation = MOVE;
            }
            else
            {
                actor->currentAnimation = IDLE;
            }
        }
        actor->attacking = !actor->moving;

        actor->nextFire -= GetFrameTime();
        actor->nextFire = Actor_FireAtPlayer(actor, actor->nextFire);
    }

    Actor_PlayAnimation(actor, 1.0f);
}

void Actor_Draw(Actor_Data* actor)
{
    DrawModel(actor->model, actor->position, 0.5f, WHITE);
}

Ray Actor_CreateRay(Actor_Data* actor)
{
    Ray rayCast;
    BoundingBox playerBb   = Player->boundingBox;
    Vector3 playerPosition = Player->position;
    Vector3 v              = Vector3Normalize(Vector3Subtract(actor->position, playerPosition));
    rayCast.direction      = (Vector3) { -1.0f * v.x, -1.0f * v.y, -1.0f * v.z };
    rayCast.position       = actor->position;
    return rayCast;
}

bool Actor_TestPlayerHit(Actor_Data* actor)
{

    Ray rayCast = Actor_CreateRay(actor);

    bool hitPlayer             = false;
    float distance             = 0.0f;
    float levelDistance        = INFINITY;
    float playerDistance       = INFINITY;
    int entitiesAmount         = Scene_data.size;
    Scene_BlockData* levelData = Scene_data.blocks;
    Scene_BlockData levelDataHit;

    for(int i = 0; i < entitiesAmount; i++)
    {
        if(levelData[i].id != 0)
        {
            Vector3 pos           = levelData[i].position;
            RayCollision hitLevel = GetRayCollisionMesh(
                rayCast, levelData[i].model.meshes[0], MatrixTranslate(pos.x, pos.y, pos.z));
            if(hitLevel.hit)
            {
                if(hitLevel.distance < levelDistance)
                {
                    levelDistance = hitLevel.distance;
                    levelDataHit  = levelData[i];
                }
            }
        }
    }

    playerDistance = Vector3Length(Vector3Subtract(Player->position, rayCast.position));

    if(playerDistance < levelDistance)
    {
        // Player is closer
        hitPlayer = true;
    }
    else
    {
        // Wall/other entity is closer so we didnt hit player
        hitPlayer = false;
    }
    return hitPlayer;
}

// Make this boolean: moving or not
bool Actor_UpdatePosition(Actor_Data* actor)
{
    bool moving = true;
    // Move actor towards player
    Vector3 DistanceFromPlayer = Vector3Subtract(actor->position, Player->position);
    //- Check if player can be seen (first raycast hit returns player)
    if(Actor_TestPlayerHit(actor))
    {
        //- If in certain range from player, stop
        if(fabsf(DistanceFromPlayer.x) >= ACTOR_MAX_DISTANCE_FROM_PLAYER ||
           fabsf(DistanceFromPlayer.z) >= ACTOR_MAX_DISTANCE_FROM_PLAYER)
        {
            Vector3 actorOldPosition = actor->position;
            Vector3 actorNewPosition =
                (Vector3) { Player->position.x, ACTOR_POSITION_Y, Player->position.z };
            actor->position = Vector3Lerp(
                actor->position, actorNewPosition, actor->movementSpeed * GetFrameTime());
            if(Scene_CheckCollision(actor->position, actor->size, actor->id))
            {
                actor->position = actorOldPosition;
            }
        }
        else
        {
            moving = false;
        }
    }
    actor->boundingBox = Utilities_MakeBoundingBox(actor->position, actor->size);
    return moving;
}

void Actor_TakeDamage(Actor_Data* actor, int damageAmount)
{
    if(!actor->dead)
    {
        actor->health -= damageAmount;
        printf("actor id %d took %d damage\n", actor->id, damageAmount);
        if(actor->health <= 0)
        {
            // Dirty hack to move bounding box outside of map so it cant be collided to.
            // We want to keep actor in the memory so we can use its position to display the
            // corpse/death anim
            Vector3 deadBoxPos      = (Vector3) { ACTOR_GRAVEYARD_POSITION,
                                                  ACTOR_GRAVEYARD_POSITION,
                                                  ACTOR_GRAVEYARD_POSITION };
            actor->boundingBox      = Utilities_MakeBoundingBox(deadBoxPos, Vector3Zero());
            actor->dead             = true;
            actor->currentAnimation = DEATH;
        }
    }
}

float Actor_FireAtPlayer(Actor_Data* actor, float nextFire)
{
    if(Actor_TestPlayerHit(actor))
    {
        Actor_RotateTowards(actor, Player->position);
        if(nextFire > 0)
        {
            nextFire -= GetFrameTime();
        }
        else
        {
            // Fire animation should play before we shoot projectile
            // TODO: Need to create "oneshot" animation thing that blocks all other animations until
            // its done playing
            actor->attacking        = true;
            actor->currentAnimation = ATTACK;

            Projectile_Create(
                Actor_CreateRay(actor), (Vector3) { 0.2f, 0.2f, 0.2f }, actor->damage, actor->id);
            nextFire = actor->fireRate;
        }
    }
    return nextFire;
}

void Actor_RotateTowards(Actor_Data* actor, Vector3 targetPosition)
{
    // Rotates the actor around Y axis
    Vector3 diff        = Vector3Subtract(actor->position, targetPosition);
    float y_angle       = -(atan2(diff.z, diff.x) + PI / 2.0);
    Vector3 newRotation = (Vector3) { 0, y_angle, 0 };

    Quaternion start = QuaternionFromEuler(actor->rotation.z, actor->rotation.y, actor->rotation.x);
    Quaternion end   = QuaternionFromEuler(newRotation.z, newRotation.y, newRotation.x);
    Quaternion slerp = QuaternionSlerp(start, end, actor->rotationSpeed * GetFrameTime());

    actor->model.transform = QuaternionToMatrix(slerp);
    actor->rotation        = newRotation;
}

// TODO: Need somekind of set animation thing
// Set animation and is it interruptable/loopable? If not, play animation once
// blocking all the other set animations until this animation has played
// If it's interruptable, just do it like below

void Actor_PlayAnimation(Actor_Data* actor, float animationSpeed)
{

    actor->animationFrame++;
    if(actor->animationFrame >= actor->animations[actor->currentAnimation].frameCount)
    {
        actor->animationFrame = 0;
    }

    UpdateModelAnimation(
        actor->model, actor->animations[actor->currentAnimation], actor->animationFrame);

    // printf("%d / %d\n",actor->model.animationFrame, frameCount);
}
