package main

import "src:game"
import rl "vendor:raylib"

// Entry point
main :: proc()
{
    game.initialize()

    for !rl.WindowShouldClose() {
        game.update()
    }

    rl.CloseWindow()
}

