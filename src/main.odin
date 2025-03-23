package main

import "src:game"
import rl "vendor:raylib"

// Entry point
main :: proc() {
	game.Initialize()

	for !game.ShouldExit() {
		game.Update()
	}

	rl.CloseWindow()
}

