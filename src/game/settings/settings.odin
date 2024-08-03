package settings

import "core:fmt"
import "core:os"
import "core:strings"
import rl "vendor:raylib"

Input :: union
{
	rl.KeyboardKey,
	rl.MouseButton,
	rl.GamepadButton,
}

GameTitle: cstring = "Artificial Rage"
FileName: string = "settings.cfg"

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
	if (Read())
	{

	}
	 else
	{
		Values = CreateDefault()
		Write()
	}

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

Read :: proc() -> bool
{
	data, success := os.read_entire_file(FileName)
	defer delete(data)
	settingsFileText := string(data)
	if (!success)
	{
		return false
	}
	settingsArr := strings.split_lines(settingsFileText)
	for setting in settingsArr
	{
		keyValuePair := strings.split(setting, "=")
		if (len(keyValuePair) < 2)
		{
			continue
		}
		Parse(keyValuePair[0], keyValuePair[1])
	}
	return true

}

Write :: proc()
{
	sb := strings.builder_make()
	// TODO add all values
	strings.write_string(&sb, fmt.aprintfln("screenWidth=%[0]d", Values.screenWidth))
	strings.write_string(&sb, fmt.aprintfln("screenHeight=%[0]d", Values.screenHeight))

	stringToWrite := strings.to_string(sb)

	data := transmute([]u8)stringToWrite

	success := os.write_entire_file(FileName, data)
	if (!success)
	{
		fmt.println("error lol")
	}
}

Parse :: proc(key: string, value: string) -> bool
{
	fmt.printfln("%[0]v %[1]v", key, value)
	// Switch ladder here to parse the values to right places
	return true
}
