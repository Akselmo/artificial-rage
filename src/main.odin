package main

import "src:game"
import rl "vendor:raylib"

// Entry point
main :: proc() {
	game.Initialize()

	for !rl.WindowShouldClose() {
		game.Update()
	}

	rl.CloseWindow()
}

