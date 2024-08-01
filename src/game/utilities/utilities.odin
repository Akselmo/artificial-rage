package utilities

import rl "vendor:raylib"

GetScreenCenter :: proc() -> rl.Vector2
{
	center := rl.Vector2{rl.GetScreenWidth() / 2.0, rl.GetScreenHeight() / 2.0}
	return center
}

MakeBoundingBox :: proc(position: rl.Vector3, size: rl.Vector3) -> rl.BoundingBox
{
	bb: rl.BoundingBox = {
		rl.Vector3{position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2},
		rl.Vector3{position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2},
	}
	return bb
}
