package game

import "src:game/scene"
import "src:game/settings"
import rl "vendor:raylib"

playerCamera: rl.Camera
isStarted: bool = false

Initialize :: proc() {
	isStarted = false

	settings.Initialize()

	rl.InitWindow(settings.screenWidth, settings.screenHeight, settings.GameTitle)
	rl.SetTargetFPS(settings.maxFPS)
	isStarted = true

	playerCamera = scene.Initialize()
	isStarted = true

}

Update :: proc() {
	rl.BeginDrawing()

	rl.ClearBackground(rl.BLACK)

	rl.BeginMode3D(playerCamera)

	if (isStarted) {
		scene.Update(&playerCamera)
	}
	rl.EndMode3D()

	HudUpdate()
	MenuUpdate()
	rl.EndDrawing()
}

MenuUpdate :: proc() {
	rl.DisableCursor()

	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
}

