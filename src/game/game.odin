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
rectangleWidth: f32 = 150.0
rectangleHeight: f32 = 30.0

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

	ChangeState(GameState.MainMenu)
	screenCenter = utilities.GetScreenCenter()

}

Update :: proc() {
	rl.BeginDrawing()

	rl.ClearBackground(rl.BLACK)

	// Check if close button was pressed
	if (rl.WindowShouldClose()) {
		ChangeState(GameState.Exit)
	}

	// TODO: could the pause menu stuff be handled here
	switch state {
	case GameState.MainMenu:
		MenuUpdate()
	case GameState.Loading:
		fmt.printfln("Loading scene")
		playerCamera = scene.Initialize()
		ChangeState(GameState.GameRunning)
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
		ChangeState(GameState.MainMenu)
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

	HeaderText(
		"Paused",
		rl.GREEN,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight,
			rectangleWidth,
			rectangleHeight,
		},
		20,
	)
	continueButton := MenuButton(
		"CONTINUE",
		rl.WHITE,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight * 3,
			rectangleWidth,
			rectangleHeight,
		},
	)
	quitButton := MenuButton(
		"EXIT TO MENU",
		rl.WHITE,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight * 4.5,
			rectangleWidth,
			rectangleHeight,
		},
	)

	if continueButton || rl.IsKeyPressed(rl.KeyboardKey.ESCAPE) {
		fmt.printfln("state set to gamin")
		rl.DisableCursor()
		ChangeState(GameState.GameRunning)
	}
	if quitButton {
		fmt.printfln("state set to menu")
		rl.EnableCursor()
		ChangeState(GameState.CleanUp)
	}

}

MenuUpdate :: proc() {

	HeaderText(
		"Artificial Rage - Menu",
		rl.GREEN,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight,
			rectangleWidth,
			rectangleHeight,
		},
		20,
	)
	//https://www.raylib.com/examples/textures/loader.html?name=textures_image_processing
	startButton := MenuButton(
		"START",
		rl.WHITE,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight * 3,
			rectangleWidth,
			rectangleHeight,
		},
	)
	quitButton := MenuButton(
		"QUIT",
		rl.WHITE,
		rl.Rectangle {
			screenCenter.x - rectangleWidth / 2,
			rectangleHeight * 4.5,
			rectangleWidth,
			rectangleHeight,
		},
	)

	// menu presses etc come here
	// Enable and disable cursor based on if menu is on or off
	if startButton {
		ChangeState(GameState.Loading)
	}
	if quitButton {
		ChangeState(GameState.Exit)
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
		ChangeState(GameState.GamePauseMenu)
	}
}

ChangeState :: proc(newState: GameState) {
	if (state == newState) {
		return
	}
	state = newState

	if (state == GameState.GamePauseMenu || state == GameState.MainMenu) {
		rl.EnableCursor()
	} else {
		rl.DisableCursor()
	}
}

