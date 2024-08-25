package entity

import "core:fmt"
import "core:math"
import "src:game/settings"
import "src:game/utilities"
import rl "vendor:raylib"


PLAYER_ID: i32 : -1
PLAYER_MAX_HEALTH: i32 : 20
PLAYER_CAMERA_MIN_CLAMP: f32 : 89.0
PLAYER_CAMERA_MAX_CLAMP: f32 : -89.0
PLAYER_CAMERA_PANNING_DIVIDER: f32 : 5.1
PLAYER_START_POSITION_Y: f32 : 0.4

PlayerCameraData :: struct {
	targetDistance:     f32,
	playerEyesPosition: f32,
	angle:              rl.Vector2,
	mouseSensitivity:   f32,
	playerSpeed:        f32,
}

PlayerData :: struct {
	health:    i32,
	dead:      bool,
	transform: Transform,
	nextFire:  f32,
}

// TODO this doesnt work


PlayerCustomCamera: PlayerCameraData = {
	targetDistance     = 0,
	playerEyesPosition = 1.85,
	angle              = {0, 0},
	mouseSensitivity   = 0.003,
	playerSpeed        = 1.75,
}

Player: PlayerData = {}

PlayerInitializeCamera :: proc(pos_x: f32, pos_z: f32) -> rl.Camera {
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
	PlayerCustomCamera.targetDistance = math.sqrt(dx * dx + dy * dy + dz * dz)

	// Camera angle calculation
	// Camera angle in plane XZ (0 aligned with Z, move positive CCW)
	PlayerCustomCamera.angle.x = math.atan2(dx, dz)
	// Camera angle in plane XY (0 aligned with X, move positive CW)
	PlayerCustomCamera.angle.y = math.atan2(dy, math.sqrt(dx * dx + dz * dz))

	// Init player eyes position to camera Y position
	PlayerCustomCamera.playerEyesPosition = camera.position.y

	PlayerCustomCamera.mouseSensitivity = settings.Values.mouseSensitivity

	Player.health = PLAYER_MAX_HEALTH
	Player.dead = false
	Player.transform.size = rl.Vector3{0.1, 0.1, 0.1}
	Player.transform.position = rl.Vector3{0.0, 0.0, 0.0}
	Player.transform.boundingBox = utilities.MakeBoundingBox(Player.transform.position, Player.transform.size)
	Player.nextFire = 0.0

	// Weapon initialize
	// Weapon selectdefault

	return camera

}

PlayerUpdate :: proc(camera: ^rl.Camera) {
	//TODO camera moves up and down but not to sides, something wrong in the matrix i guess?
	oldPlayerPos := camera.position

	MOVE_FRONT :: 1
	MOVE_BACK :: 2
	MOVE_RIGHT :: 3
	MOVE_LEFT :: 4

	PlayerCameraMoveKeys := map[i32]f32 {
		MOVE_FRONT = cast(f32)cast(i32)rl.IsKeyDown(cast(rl.KeyboardKey)settings.Values.keyMoveForward),
		MOVE_BACK  = cast(f32)cast(i32)rl.IsKeyDown(cast(rl.KeyboardKey)settings.Values.keyMoveBackward),
		MOVE_RIGHT = cast(f32)cast(i32)rl.IsKeyDown(cast(rl.KeyboardKey)settings.Values.keyMoveRight),
		MOVE_LEFT  = cast(f32)cast(i32)rl.IsKeyDown(cast(rl.KeyboardKey)settings.Values.keyMoveLeft),
	}

	Player.transform.boundingBox = utilities.MakeBoundingBox(camera.position, Player.transform.size)

	mousePositionDelta := rl.GetMouseDelta()
	// Move camera around X pos
	camera.position.x +=
		((math.sin(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_BACK] -
				math.sin(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_FRONT] -
				math.cos(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_LEFT] +
				math.cos(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_RIGHT]) *
			PlayerCustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Move camera around Y pos
	camera.position.y +=
		((math.sin(PlayerCustomCamera.angle.y) * PlayerCameraMoveKeys[MOVE_FRONT] -
				math.sin(PlayerCustomCamera.angle.y) * PlayerCameraMoveKeys[MOVE_BACK]) *
			PlayerCustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Move camera around Z pos
	camera.position.z +=
		((math.cos(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_BACK] -
				math.cos(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_FRONT] +
				math.sin(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_LEFT] -
				math.sin(PlayerCustomCamera.angle.x) * PlayerCameraMoveKeys[MOVE_RIGHT]) *
			PlayerCustomCamera.playerSpeed) *
		rl.GetFrameTime()

	// Camera orientation calculation
	PlayerCustomCamera.angle.x -= mousePositionDelta.x * PlayerCustomCamera.mouseSensitivity * rl.GetFrameTime()
	PlayerCustomCamera.angle.y -= mousePositionDelta.y * PlayerCustomCamera.mouseSensitivity * rl.GetFrameTime()

	// Angle clamp
	if (PlayerCustomCamera.angle.y > PLAYER_CAMERA_MIN_CLAMP * math.DEG_PER_RAD) {
		PlayerCustomCamera.angle.y = PLAYER_CAMERA_MIN_CLAMP * math.DEG_PER_RAD
	} else if (PlayerCustomCamera.angle.y < PLAYER_CAMERA_MAX_CLAMP * math.DEG_PER_RAD) {
		PlayerCustomCamera.angle.y = PLAYER_CAMERA_MAX_CLAMP * math.DEG_PER_RAD
	}

	// Recalculate camera target considering translation and rotation
	translation := rl.MatrixTranslate(0, 0, (PlayerCustomCamera.targetDistance / PLAYER_CAMERA_PANNING_DIVIDER))
	rotation := rl.MatrixInvert(
		rl.MatrixRotateXYZ(
			(rl.Vector3{math.PI * 2 - PlayerCustomCamera.angle.y, math.PI * 2 - PlayerCustomCamera.angle.x, 0}),
		),
	)
	transform: rl.Matrix = rl.MatrixMultiply(rotation, translation)

	// Move camera according to matrix position (where camera looks at)
	camera.target.x = camera.position.x - transform[0, 3]
	camera.target.y = camera.position.y - transform[1, 3]
	camera.target.z = camera.position.z - transform[2, 3]

	// Camera position update
	camera.position.y = PlayerCustomCamera.playerEyesPosition

	//TODO scene check collision
	if (CheckCollision(camera.position, Player.transform.size, PLAYER_ID)) {
		camera.position = oldPlayerPos
	}

	Player.transform.position = camera.position
	Player.transform.boundingBox = utilities.MakeBoundingBox(Player.transform.position, Player.transform.size)

	// Check if we need to switch weapon
	// TODO weapon.GetSwitchInput()
	PlayerFireWeapon(camera, &PlayerCustomCamera)
}

PlayerSetHealth :: proc(healthToAdd: i32) {
	Player.health += healthToAdd
	if (Player.health > PLAYER_MAX_HEALTH) {
		Player.health = PLAYER_MAX_HEALTH
	} else if (Player.health <= 0) {
		Player.dead = true
	}

}

PlayerFireWeapon :: proc(camera: ^rl.Camera, cameraData: ^PlayerCameraData) {
	Player.nextFire -= rl.GetFrameTime()
	if (rl.IsMouseButtonDown(cast(rl.MouseButton)settings.Values.keyFire)) {
		Player.nextFire = rl.GetFrameTime() // weapon.Fire(camera, entity.nextFire)
		fmt.println("pew pew")
	}
}

