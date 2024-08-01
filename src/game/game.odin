package game

import "src:game/settings"
import rl "vendor:raylib"

camera: rl.Camera
isStarted: bool = false

Initialize :: proc()
{
	isStarted = false

	settings.Initialize()
	//camera = //scene camera

	rl.InitWindow(settings.Values.screenWidth, settings.Values.screenHeight, "Artificial Rage")
	rl.SetTargetFPS(settings.Values.maxFPS)
	isStarted = true

}

Update :: proc()
{
	rl.BeginDrawing()

	rl.ClearBackground(rl.BLACK)

	rl.BeginMode3D(camera)

	if (isStarted)
	{
		// player update
		// scene update
	}
	rl.EndMode3D()

	//game hudupdate
	// game menuupdate
	rl.EndDrawing()
}

HudUpdate :: proc()
{
	// hud draw
}

MenuUpdate :: proc()
{
	rl.DisableCursor()

	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
}
