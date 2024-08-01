package settings

import rl "vendor:raylib"

Input :: union
{
	rl.KeyboardKey,
	rl.MouseButton,
	rl.GamepadButton,
}

SettingData :: struct
{
	screenWidth:      i32,
	screenHeight:     i32,
	mouseSensitivity: f32,
	cameraFOV:        i32,
	maxFPS:           i32,
	keyMoveForward:   Input,
	keyMoveBackward:  Input,
	keyMoveRight:     Input,
	keyMoveLeft:      Input,
	keyFire:          Input,
	keyUse:           Input,
	keyJump:          Input,
	keyWeaponOne:     Input,
	keyWeaponTwo:     Input,
	keyWeaponThree:   Input,
	keyWeaponFour:    Input,
	keyWeaponFive:    Input,
}

Values: SettingData

Initialize :: proc()
{
	Values = CreateDefault()
}

CreateDefault :: proc() -> SettingData
{
	defaults := SettingData{}
	defaults.screenWidth = 800
	defaults.screenHeight = 600
	defaults.mouseSensitivity = 0.25
	defaults.cameraFOV = 90
	defaults.maxFPS = 200
	defaults.keyMoveForward = rl.KeyboardKey.W
	defaults.keyMoveBackward = rl.KeyboardKey.S
	defaults.keyMoveLeft = rl.KeyboardKey.A
	defaults.keyMoveRight = rl.KeyboardKey.D
	defaults.keyFire = rl.MouseButton.LEFT
	defaults.keyUse = rl.KeyboardKey.E
	defaults.keyJump = rl.KeyboardKey.SPACE
	defaults.keyWeaponOne = rl.KeyboardKey.ONE
	defaults.keyWeaponTwo = rl.KeyboardKey.TWO
	defaults.keyWeaponThree = rl.KeyboardKey.THREE
	defaults.keyWeaponFour = rl.KeyboardKey.FOUR
	defaults.keyWeaponFive = rl.KeyboardKey.FIVE

	return defaults
}
