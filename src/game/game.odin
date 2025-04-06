package game

import "core:fmt"
import "src:game/scene"
import "src:game/settings"
import "src:game/utilities"
import rl "vendor:raylib"

playerCamera: rl.Camera
menuCamera: rl.Camera2D
state: GameState
screenCenter: rl.Vector2

GameState :: enum {
	MainMenu,
	Loading,
	GameRunning,
	GameOver,
	GamePauseMenu,
	CleanUp,
	Exit,
}

Initialize :: proc() {
	state = GameState.Loading

	settings.Initialize()

	rl.InitWindow(settings.screenWidth, settings.screenHeight, settings.GameTitle)
	rl.SetTargetFPS(settings.maxFPS)
	rl.SetExitKey(rl.KeyboardKey.KEY_NULL)

	state = GameState.MainMenu
	screenCenter = utilities.GetScreenCenter()

}

Update :: proc() {
	rl.BeginDrawing()

	rl.ClearBackground(rl.BLACK)

	// Check if close button was pressed
	if (rl.WindowShouldClose()) {
		state = GameState.Exit
	}

	// TODO: could the pause menu stuff be handled here
	switch state {
	case GameState.MainMenu:
		MenuUpdate()
	case GameState.Loading:
		fmt.printfln("Loading scene")
		playerCamera = scene.Initialize()
		state = GameState.GameRunning
	case GameState.GameRunning:
		GameUpdate()
	case GameState.GamePauseMenu:
		PauseUpdate()
	case GameState.GameOver:
		// TODO this has to clean up and then remake the same level if player restarts
		fmt.printfln("no gameover yet!")
	case GameState.CleanUp:
		fmt.printfln("Cleaning up the scene")
		scene.Clean()
		rl.EnableCursor()
		state = GameState.MainMenu
	case GameState.Exit:
		// Don't do anything
		return
	}

	rl.EndDrawing()
}

ShouldExit :: proc() -> bool {
	return state == GameState.Exit
}

PauseUpdate :: proc() {
	HeaderText("PAUSED", rl.GREEN, rl.Vector2{screenCenter.x, 10}, 20)
	continueButton := MenuButton(
		"CONTINUE",
		rl.WHITE,
		rl.Rectangle{screenCenter.x, 50.0, 150.0, 30.0},
	)
	quitButton := MenuButton(
		"EXIT TO MENU",
		rl.WHITE,
		rl.Rectangle{screenCenter.x, 80.0, 150.0, 30.0},
	)

	if continueButton || rl.IsKeyPressed(rl.KeyboardKey.ESCAPE) {
		fmt.printfln("state set to gamin")
		rl.DisableCursor()
		state = GameState.GameRunning
	}
	if quitButton {
		fmt.printfln("state set to menu")
		rl.EnableCursor()
		state = GameState.CleanUp
	}

}

MenuUpdate :: proc() {

	// TODO We need to do level clean up here
	HeaderText("Artificial Rage - Menu", rl.GREEN, rl.Vector2{screenCenter.x, 10}, 20)
	//https://www.raylib.com/examples/textures/loader.html?name=textures_image_processing
	startButton := MenuButton("START", rl.WHITE, rl.Rectangle{screenCenter.x, 50.0, 150.0, 30.0})
	quitButton := MenuButton("QUIT", rl.WHITE, rl.Rectangle{screenCenter.x, 80.0, 150.0, 30.0})

	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
	if startButton {
		rl.DisableCursor()
		// TODO somekind of state updater proc that runs disable/enable cursor as well
		state = GameState.Loading
	}
	if quitButton {
		state = GameState.Exit
	}

}

GameUpdate :: proc() {
	rl.BeginMode3D(playerCamera)
	scene.Update(&playerCamera)
	rl.EndMode3D()
	HudUpdate()
	if rl.IsKeyPressed(rl.KeyboardKey.ESCAPE) {
		fmt.printfln("state set to pause")
		rl.EnableCursor()
		state = GameState.GamePauseMenu
	}
}

