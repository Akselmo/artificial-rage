#include "player.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include "scene.h"
#include "settings.h"
#include "utilities.h"
#include "weapon.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Took some parts of raylib camera.h and made my own camera based on that for full control

// Initialize Custom Camera
static Player_CustomCameraData Player_CustomCamera = {
	.targetDistance     = 0,
	.playerEyesPosition = 1.85f,
	.angle              = { 0 },
	.mouseSensitivity   = 0.003f,
	.playerSpeed        = 1.75f,
};

Player_Data *Player = nullptr;

// Private functions

Camera Player_InitializeCamera(float pos_x, float pos_z)
{
	Camera camera = { 0 };

	// Place camera and apply settings
	camera.position   = (Vector3){ pos_x, PLAYER_START_POSITION_Y, pos_z };
	camera.target     = (Vector3){ 0.0f, 0.5f, 0.0f };
	camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy       = Settings.cameraFOV; // get fov from settings file
	camera.projection = CAMERA_PERSPECTIVE;

	// Distances
	const Vector3 v1 = camera.position;
	const Vector3 v2 = camera.target;
	const float dx   = v2.x - v1.x;
	const float dy   = v2.y - v1.y;
	const float dz   = v2.z - v1.z;

	// Distance to target
	Player_CustomCamera.targetDistance = sqrtf(dx * dx + dy * dy + dz * dz);

	// Camera angle calculation
	// Camera angle in plane XZ (0 aligned with Z, move positive CCW)
	Player_CustomCamera.angle.x = atan2f(dx, dz);
	// Camera angle in plane XY (0 aligned with X, move positive CW)
	Player_CustomCamera.angle.y = atan2f(dy, sqrtf(dx * dx + dz * dz));

	// Init player eyes position to camera Y position
	Player_CustomCamera.playerEyesPosition = camera.position.y;

	Player_CustomCamera.mouseSensitivity = Settings.mouseSensitivity;

	// Initialize player data
	Player              = calloc(1, sizeof(Player_Data));
	Player->health      = PLAYER_MAX_HEALTH;
	Player->dead        = false;
	Player->transform.size        = (Vector3){ 0.1f, 0.1f, 0.1f };
	Player->transform.position    = (Vector3){ 0.0f, 0.0f, 0.0f };
	Player->transform.boundingBox = Utilities_MakeBoundingBox(Player->transform.position, Player->transform.size);
	Player->nextFire    = 0.0f;

	Weapon_Initialize();

	Weapon_SelectDefault();

	return camera;
}

void Player_Update(Camera *camera)
{

	const Vector3 oldPlayerPos = camera->position;

	Player->transform.boundingBox = Utilities_MakeBoundingBox(camera->position, Player->transform.size);

	const Vector2 mousePositionDelta = GetMouseDelta();

	const bool direction[4] = { IsKeyDown(Settings.keyMoveForward),
		                        IsKeyDown(Settings.keyMoveBackward),
		                        IsKeyDown(Settings.keyMoveRight),
		                        IsKeyDown(Settings.keyMoveLeft) };

	// Move camera around X pos
	camera->position.x += ((sinf(Player_CustomCamera.angle.x) * direction[MOVE_BACK] -
	                        sinf(Player_CustomCamera.angle.x) * direction[MOVE_FRONT] -
	                        cosf(Player_CustomCamera.angle.x) * direction[MOVE_LEFT] +
	                        cosf(Player_CustomCamera.angle.x) * direction[MOVE_RIGHT]) *
	                       Player_CustomCamera.playerSpeed) *
	                      GetFrameTime();

	// Move camera around Y pos
	camera->position.y += ((sinf(Player_CustomCamera.angle.y) * direction[MOVE_FRONT] -
	                        sinf(Player_CustomCamera.angle.y) * direction[MOVE_BACK]) *
	                       Player_CustomCamera.playerSpeed) *
	                      GetFrameTime();

	// Move camera around Z pos
	camera->position.z += ((cosf(Player_CustomCamera.angle.x) * direction[MOVE_BACK] -
	                        cosf(Player_CustomCamera.angle.x) * direction[MOVE_FRONT] +
	                        sinf(Player_CustomCamera.angle.x) * direction[MOVE_LEFT] -
	                        sinf(Player_CustomCamera.angle.x) * direction[MOVE_RIGHT]) *
	                       Player_CustomCamera.playerSpeed) *
	                      GetFrameTime();

	// Camera orientation calculation
	Player_CustomCamera.angle.x -= mousePositionDelta.x * Player_CustomCamera.mouseSensitivity * GetFrameTime();
	Player_CustomCamera.angle.y -= mousePositionDelta.y * Player_CustomCamera.mouseSensitivity * GetFrameTime();

	// Angle clamp
	if (Player_CustomCamera.angle.y > PLAYER_CAMERA_MIN_CLAMP * DEG2RAD)
	{
		Player_CustomCamera.angle.y = PLAYER_CAMERA_MIN_CLAMP * DEG2RAD;
	}
	else if (Player_CustomCamera.angle.y < PLAYER_CAMERA_MAX_CLAMP * DEG2RAD)
	{
		Player_CustomCamera.angle.y = PLAYER_CAMERA_MAX_CLAMP * DEG2RAD;
	}

	// Recalculate camera target considering translation and rotation
	const Matrix translation =
		MatrixTranslate(0, 0, (Player_CustomCamera.targetDistance / PLAYER_CAMERA_PANNING_DIVIDER));
	const Matrix rotation  = MatrixInvert(MatrixRotateXYZ((Vector3
    ){ PI * 2 - Player_CustomCamera.angle.y, PI * 2 - Player_CustomCamera.angle.x, 0 }));
	const Matrix transform = MatrixMultiply(translation, rotation);

	// Move camera according to matrix position (where camera looks at)
	camera->target.x = camera->position.x - transform.m12;
	camera->target.y = camera->position.y - transform.m13;
	camera->target.z = camera->position.z - transform.m14;

	// Camera position update
	camera->position.y = Player_CustomCamera.playerEyesPosition;

	if (Scene_CheckCollision(camera->position, Player->transform.size, PLAYER_ID))
	{
		camera->position = oldPlayerPos;
	}

	Player->transform.position    = camera->position;
	Player->transform.boundingBox = Utilities_MakeBoundingBox(Player->transform.position, Player->transform.size);
	// Check if we need to switch weapon
	Weapon_GetSwitchInput();
	Player_FireWeapon(camera, &Player_CustomCamera);
}

// Use minus for removing health
void Player_SetHealth(int healthToAdd)
{
	Player->health += healthToAdd;
	if (Player->health > PLAYER_MAX_HEALTH)
	{
		Player->health = PLAYER_MAX_HEALTH;
	}
	else if (Player->health <= 0)
	{
		Player->dead = true;
	}
}

void Player_FireWeapon(Camera *camera, Player_CustomCameraData *cameraData)
{
	// Calculate fire rate countdown here
	Player->nextFire -= GetFrameTime();
	if (IsMouseButtonDown(Settings.keyFire))
	{
		Player->nextFire = Weapon_Fire(camera, Player->nextFire);
	}
}
