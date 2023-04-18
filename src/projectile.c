#include "projectile.h"
#include "actor.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include "scene.h"
#include "utilities.h"

// Prototypes
void Projectile_DestroyOverTime(Projectile* projectile);
void Projectile_RotateTowards(Projectile* projectile);

void Projectile_Create(Ray rayCast, Vector3 size, int damage, int ownerId, Color color)
{
    // Projectile does not need to know if it hits or not, that is calculated from
    // entities themselves.
    // clang-format off
    for(int i = 1; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        if(Scene.projectiles[i].id != i || Scene.projectiles[i].destroyed == true)
        {
            Projectile projectile = {
                .startPosition = rayCast.position,
                .endPosition   = Vector3Add(
                    rayCast.position, Vector3Scale(rayCast.direction, PROJECTILE_TRAVEL_DISTANCE)),
                .position    = projectile.startPosition,
                .id          = i,
                .ownerId     = ownerId,
                .size        = size,
                .boundingBox = Utilities_MakeBoundingBox(rayCast.position, projectile.size),
                .damage      = damage,
                .color       = color,
                .speed       = 0.12f * GetFrameTime(),
                .destroyed   = false,
            };
            projectile.model = LoadModelFromMesh(GenMeshCube(projectile.size.x, projectile.size.y, projectile.size.z));
            printf("Projectile id created %d\n", projectile.id);
            Scene.projectiles[i] = projectile;
            Projectile_RotateTowards(&Scene.projectiles[i]);
            break;
        }
    }
    // clang-format on
}

void Projectile_Update(Projectile* projectile)
{
    if(!projectile->destroyed && projectile->id != 0)
    {

        DrawModel(projectile->model, projectile->position, 1.0, projectile->color);

        // Lerp projectile here (both model and bounding box)
        projectile->position = Vector3Lerp(projectile->position, projectile->endPosition, projectile->speed);
        Projectile_DestroyOverTime(projectile);
        Projectile_CheckCollision(projectile);
    }
}

void Projectile_CheckCollision(Projectile* projectile)
{
    // Check against the owner of the projectile and the entity id. if theres a match, ignore it,
    // unless its a wall
    //  Otherwise tell the entity they've been hit and give them damage
    const BoundingBox projectileBox = Utilities_MakeBoundingBox(projectile->position, projectile->size);
    for(int i = 0; i < Scene.size; i++)
    {

        // Test hitting against wall
        if(CheckCollisionBoxes(projectileBox, Scene.blocks[i].boundingBox))
        {
            Projectile_Destroy(projectile);
            return;
        }
        // Against enemy except if owned by enemy
        else if(CheckCollisionBoxes(projectileBox, Scene.actors[i].boundingBox) && Scene.actors[i].id != projectile->ownerId)
        {
            Actor_TakeDamage(&Scene.actors[i], projectile->damage);
            Projectile_Destroy(projectile);
            return;
        }
        // Against player except if owned by player
        else if(CheckCollisionBoxes(projectileBox, Player->boundingBox) && PLAYER_ID != projectile->ownerId)
        {
            Player_SetHealth(-1 * projectile->damage);
            Projectile_Destroy(projectile);
            return;
        }
    }
}

// Rename to projectile_DestroyOverTime
void Projectile_DestroyOverTime(Projectile* projectile)
{
    // TODO: Destroy after 20 seconds
}

// Rename to projectile_destroy
void Projectile_Destroy(Projectile* projectile)
{
    printf("Projectile id %d destroyed\n", projectile->id);
    projectile->destroyed = true;
    projectile            = NULL;
    free(projectile);
}

void Projectile_RotateTowards(Projectile* projectile)
{

    float dx = projectile->endPosition.x - projectile->startPosition.x;
    float dy = projectile->endPosition.y - projectile->startPosition.y;
    float dz = projectile->endPosition.z - projectile->startPosition.z;

    const float y_angle = -(atan2f(dz, dx) + PI / 2.0f);
    const float z_angle = atan2f(dy, sqrtf(dx * dx + dz * dz));

    projectile->model.transform = QuaternionToMatrix(QuaternionFromEuler(z_angle, y_angle, 0));
}