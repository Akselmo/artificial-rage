package player

import "core:fmt"
import "core:math"
import "src:game/entity"
import "src:game/settings"
import "src:game/utilities"
import rl "vendor:raylib"


PLAYER_ID: i32 : -1
PLAYER_MAX_HEALTH: i32 : 20
PLAYER_CAMERA_MIN_CLAMP: f32 : 89.0
PLAYER_CAMERA_MAX_CLAMP: f32 : -89.0
PLAYER_CAMERA_PANNING_DIVIDER: f32 : 5.1
PLAYER_START_POSITION_Y: f32 : 0.4

CustomCameraData :: struct
{
	targetDistance:     f32,
	playerEyesPosition: f32,
	angle:              rl.Vector2,
	mouseSensitivity:   f32,
	playerSpeed:        f32,
}

Data :: struct
{
	health:    i32,
	dead:      bool,
	transform: entity.Transform,
	nextFire:  f32,
}

CameraMoveKeys :: map[string]bool \
{
	"MOVE_FRONT" = rl.IsKeyDown(settings.Values.keyMoveForward),
	"MOVE_BACK"  = rl.IsKeyDown(settings.Values.keyMoveBackward),
	"MOVE_RIGHT" = rl.IsKeyDown(settings.Values.keyMoveRight),
	"MOVE_LEFT"  = rl.IsKeyDown(settings.Values.keyMoveLeft),
}

CustomCamera: CustomCameraData = \
{
	targetDistance     = 0,
	playerEyesPosition = 1.85,
	angle              = {0, 0},
	mouseSensitivity   = 0.003,
	playerSpeed        = 1.75,
}

Player: Data

InitializeCamera :: proc(pos_x: f32, pos_z: f32) -> rl.Camera
{
	camera: rl.Camera

	// Place camera and apply settings
	camera.position = {pos_x, PLAYER_START_POSITION_Y, pos_z}
	camera.target = {0.0, 0.5, 0.0}
	camera.up = {0.0, 1.0, 0.0}
	camera.fovy = settings.Values.cameraFOV // get fov from settings file
	camera.projection = rl.CameraProjection.PERSPECTIVE

	// Distances
	v1 := camera.position
	v2 := camera.target
	dx: f32 = v2.x - v1.x
	dy: f32 = v2.y - v1.y
	dz: f32 = v2.z - v1.z

	// Distance to target
	CustomCamera.targetDistance = math.sqrt(dx * dx + dy * dy + dz * dz)

	// Camera angle calculation
	// Camera angle in plane XZ (0 aligned with Z, move positive CCW)
	CustomCamera.angle.x = math.atan2(dx, dz)
	// Camera angle in plane XY (0 aligned with X, move positive CW)
	CustomCamera.angle.y = math.atan2(dy, math.sqrt(dx * dx + dz * dz))

	// Init player eyes position to camera Y position
	CustomCamera.playerEyesPosition = camera.position.y

	CustomCamera.mouseSensitivity = settings.Values.mouseSensitivity

	Player.health = PLAYER_MAX_HEALTH
	Player.dead = false
	//Transforms come here
	Player.nextFire = 0.0

	// Weapon initialize
	// Weapon selectdefault

	return camera

}

Update :: proc(camera: ^rl.Camera)
{
	oldPlayerPos := camera.position

	Player.transform.boundingBox = utilities.MakeBoundingBox(
		camera.position,
		Player.transform.size,
	)

	mousePositionDelta := rl.GetMouseDelta()

	// Move camera around X pos
	camera.position.x +=
		((math.sin(CustomCamera.angle.x) * CameraMoveKeys["MOVE_BACK"] -
				math.sin(CustomCamera.angle.x) * CameraMoveKeys["MOVE_FRONT"] -
				math.cos(CustomCamera.angle.x) * CameraMoveKeys["MOVE_LEFT"] +
				math.cos(CustomCamera.angle.x) * CameraMoveKeys["MOVE_RIGHT"]) *
			CustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Move camera around Y pos
	camera.position.y +=
		((math.sin(CustomCamera.angle.y) * CameraMoveKeys["MOVE_FRONT"] -
				math.sinf(CustomCamera.angle.y) * CameraMoveKeys["MOVE_BACK"]) *
			CustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Move camera around Z pos
	camera.position.z +=
		((math.cos(CustomCamera.angle.x) * CameraMoveKeys["MOVE_BACK"] -
				math.cos(CustomCamera.angle.x) * CameraMoveKeys["MOVE_FRONT"] +
				math.sin(CustomCamera.angle.x) * CameraMoveKeys["MOVE_LEFT"] -
				math.sin(CustomCamera.angle.x) * CameraMoveKeys["MOVE_RIGHT"]) *
			CustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Camera orientation calculation
	CustomCamera.angle.x -=
		mousePositionDelta.x * CustomCamera.mouseSensitivity * rl.GetFrameTime()
	CustomCamera.angle.y -=
		mousePositionDelta.y * CustomCamera.mouseSensitivity * rl.GetFrameTime()

	// Angle clamp
	if (CustomCamera.angle.y > PLAYER_CAMERA_MIN_CLAMP * math.DEG_PER_RAD)
	{
		CustomCamera.angle.y = PLAYER_CAMERA_MIN_CLAMP * math.DEG_PER_RAD
	}
	 else if (CustomCamera.angle.y < PLAYER_CAMERA_MAX_CLAMP * math.DEG_PER_RAD)
	{
		CustomCamera.angle.y = PLAYER_CAMERA_MAX_CLAMP * math.DEG_PER_RAD
	}

	// Recalculate camera target considering translation and rotation
	translation :: rl.MatrixTranslate(
		0,
		0,
		(CustomCamera.targetDistance / PLAYER_CAMERA_PANNING_DIVIDER),
	)
	rotation :: rl.MatrixInvert(
		rl.MatrixRotateXYZ(
			(rl.Vector3 {
					math.PI * 2 - CustomCamera.angle.y,
					math.PI * 2 - CustomCamera.angle.x,
					0,
				}),
		),
	)
	transform :: rl.MatrixMultiply(translation, rotation)

	// Move camera according to matrix position (where camera looks at)
	camera.target.x = camera.position.x - transform.m12
	camera.target.y = camera.position.y - transform.m13
	camera.target.z = camera.position.z - transform.m14

	// Camera position update
	camera.position.y = CustomCamera.playerEyesPosition

	//TODO scene check collision

	Player.transform.position = camera.position
	Player.transform.boundingBox = utilities.MakeBoundingBox(
		Player.transform.position,
		Player.transform.size,
	)

	// Check if we need to switch weapon
	// TODO weapon.GetSwitchInput()
	FireWeapon(camera, CustomCamera)
}

SetHealth :: proc(healthToAdd: i32)
{
	Player.health += healthToAdd
	if (Player.health > PLAYER_MAX_HEALTH)
	{
		Player.health = PLAYER_MAX_HEALTH
	}
	 else if (Player.health <= 0)
	{
		Player.dead = true
	}

}

FireWeapon :: proc(camera: ^rl.Camera, cameraData: ^CustomCameraData)
{
	Player.nextFire -= GetFrameTime()
	if (rl.IsMouseButtonDown(settings.Values.keyFire) || rl.IsKeyDown(settings.Values.keyFire))
	{
		Player.nextFire = GetFrameTime() // weapon.Fire(camera, Player.nextFire)
		fmt.println("pew pew")
	}
}

