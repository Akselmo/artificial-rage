package game

import "src:game/entity"
import "src:game/scene"
import "src:game/settings"
import rl "vendor:raylib"

camera: rl.Camera
isStarted: bool = false

Initialize :: proc() {
	isStarted = false

	settings.Initialize()

	rl.InitWindow(settings.Values.screenWidth, settings.Values.screenHeight, settings.GameTitle)
	rl.SetTargetFPS(settings.Values.maxFPS)
	isStarted = true

	camera = scene.Initialize()
	isStarted = true

}

Update :: proc() {
	rl.BeginDrawing()

	rl.ClearBackground(rl.BLACK)

	rl.BeginMode3D(camera)

	if (isStarted) {
		entity.PlayerUpdate(&camera)
		scene.Update()
	}
	rl.EndMode3D()

	HudUpdate()
	MenuUpdate()
	rl.EndDrawing()
}

HudUpdate :: proc() {
	// hud draw
	rl.DrawFPS(10, 10)
}

MenuUpdate :: proc() {
	rl.DisableCursor()

	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
}

