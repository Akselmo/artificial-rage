package game

import "core:fmt"
import "core:strings"
import "src:game/entity"
import rl "vendor:raylib"

HudUpdate :: proc() {
	rl.DrawFPS(10, 10)

	// Draw weapon
	// weapon.DrawSprite()
	if (entity.Player.health <= 0) {
		HealthText("DEAD", rl.RED)
	} else {
		txt := strings.clone_to_cstring(fmt.aprintf("%[0]v", entity.Player.health))
		defer delete(txt)
		HealthText(txt, rl.GREEN)
	}

	// TODO Draw weapon ammo text
	// if weaponholder etc etc....
}

@(private = "file")
HealthText :: proc(txt: cstring, color: rl.Color) {
	rl.DrawText(txt, 50, rl.GetScreenHeight() - 20, 20, color)
}

@(private = "file")
WeaponText :: proc(txt: cstring, color: rl.Color) {
	rl.DrawText(txt, rl.GetScreenWidth() - 100, rl.GetScreenHeight() - 20, 20, color)
}

