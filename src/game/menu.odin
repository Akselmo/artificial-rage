package game

import "core:fmt"
import "src:game/settings"
import rl "vendor:raylib"

// Returns true when clicked, false when idle
MenuButton :: proc(txt: cstring, color: rl.Color, rectangle: rl.Rectangle) -> bool {
	// Draw background
	rl.DrawRectangleRec(rectangle, color)

	if rl.CheckCollisionPointRec(rl.GetMousePosition(), rectangle) {
		// Draw hover effect
		rl.DrawRectangleRec(rectangle, rl.BLUE)
		// Handle clicking
		if rl.IsMouseButtonReleased(rl.MouseButton.LEFT) {
			fmt.printfln("%[0]v clicked", txt)
			return true
		}
	}

	// Draw text after everything
	rl.DrawText(
		txt,
		cast(i32)(rectangle.x + rectangle.width / 2.0 - cast(f32)rl.MeasureText(txt, 10) / 2.0),
		cast(i32)rectangle.y + 11.0,
		10,
		rl.BLACK,
	)
	return false
}

HeaderText :: proc(txt: cstring, color: rl.Color, position: rl.Vector2, fontSize: i32) {
	rl.DrawText(txt, cast(i32)position.x, cast(i32)position.y, fontSize, color)
}

