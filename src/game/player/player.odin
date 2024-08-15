package player

import "core:math"
import "src:game/settings"
import "src:game/entity"
import rl "vendor:raylib"


PLAYER_ID := -1
PLAYER_MAX_HEALTH: i32 = 20
PLAYER_CAMERA_MIN_CLAMP := 89.0
PLAYER_CAMERA_MAX_CLAMP := -89.0
PLAYER_CAMERA_PANNING_DIVIDER := 5.1
PLAYER_START_POSITION_Y: f32 = 0.4

CustomCameraData :: struct {
	targetDistance:     f32,
	playerEyesPosition: f32,
	angle:              rl.Vector2,
	mouseSensitivity:   f32,
	playerSpeed:        f32,
}

Data :: struct {
	health:   i32,
	dead:     bool,
	transform : entity.Transform
	nextFire: f32,
}

CameraMoveKeys :: enum {
	MOVE_FRONT = 0,
	MOVE_BACK  = 1,
	MOVE_RIGHT = 2,
	MOVE_LEFT  = 3,
}

CustomCamera: CustomCameraData = {
	targetDistance     = 0,
	playerEyesPosition = 1.85,
	angle              = {0, 0},
	mouseSensitivity   = 0.003,
	playerSpeed        = 1.75,
}

Player: Data

InitializeCamera :: proc(pos_x: f32, pos_z: f32) -> rl.Camera {
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

Update :: proc(camera: ^rl.Camera) {
	oldPlayerPos := camera.position
}

SetHealth :: proc(healthToAdd: i32) {

}

FireWeapon :: proc(camera: ^rl.Camera) {

}

