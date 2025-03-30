package game

import "core:fmt"
import "src:game/settings"
import rl "vendor:raylib"

MenuButton :: proc(txt: cstring, color: rl.Color, rectangle: rl.Rectangle) -> bool {
	rl.DrawRectangleRec(rectangle, color)
	rl.DrawRectangleLines(
		cast(i32)rectangle.x,
		cast(i32)rectangle.y,
		cast(i32)rectangle.width,
		cast(i32)rectangle.height,
		color,
	)
	rl.DrawText(
		txt,
		cast(i32)(rectangle.x + rectangle.width / 2.0 - cast(f32)rl.MeasureText(txt, 10) / 2.0),
		cast(i32)rectangle.y + 11.0,
		10,
		rl.BLACK,
	)

	if (rl.CheckCollisionPointRec(rl.GetMousePosition(), rectangle) && rl.IsMouseButtonDown(rl.MouseButton.LEFT)) {
		fmt.printfln("%[0]v clicked", txt)
		return true
	}
	return false
}

HeaderText :: proc(txt: cstring, color: rl.Color, position: rl.Vector2, fontSize: i32) {
	rl.DrawText(txt, cast(i32)position.x, cast(i32)position.y, fontSize, color)
}
