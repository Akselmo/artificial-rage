package game

import "core:fmt"
import "core:strings"
import "src:game/entity"
import rl "vendor:raylib"

HudUpdate :: proc() {
	rl.DrawFPS(10, 10)

	// Draw weapon
	entity.WeaponDrawSprite()
	if (entity.Player.health <= 0) {
		HealthText("DEAD", rl.RED)
	} else {
		txt := strings.clone_to_cstring(fmt.aprintf("%[0]v", entity.Player.health))
		defer delete(txt)
		HealthText(txt, rl.GREEN)
	}

	// Draw weapon ammo text
	wpn := entity.Weapons[entity.WeaponCurrent]
	if (wpn.weaponId == entity.WeaponID.MELEE) {
		WeaponText("-", rl.GREEN)
	} else {
		wpnText := strings.clone_to_cstring(fmt.aprintf("%[0]v / %[1]v", wpn.ammo, wpn.maxAmmo))
		WeaponText(wpnText, rl.GREEN)
	}
}

@(private = "file")
HealthText :: proc(txt: cstring, color: rl.Color) {
	rl.DrawText(txt, 50, rl.GetScreenHeight() - 20, 20, color)
}

@(private = "file")
WeaponText :: proc(txt: cstring, color: rl.Color) {
	rl.DrawText(txt, rl.GetScreenWidth() - 100, rl.GetScreenHeight() - 20, 20, color)
}

