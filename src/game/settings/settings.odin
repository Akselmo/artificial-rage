package settings

import "core:fmt"
import "core:os"
import "core:strings"
import "core:strconv"
import rl "vendor:raylib"

GameTitle: cstring = "Artificial Rage"
FileName: string = "settings.cfg"

SettingData :: struct
{
	screenWidth:      i32,
	screenHeight:     i32,
	mouseSensitivity: f32,
	cameraFOV:        i32,
	maxFPS:           i32,
	keyMoveForward:   i32,
	keyMoveBackward:  i32,
	keyMoveRight:     i32,
	keyMoveLeft:      i32,
	keyFire:          i32,
	keyUse:           i32,
	keyJump:          i32,
	keyWeaponOne:     i32,
	keyWeaponTwo:     i32,
	keyWeaponThree:   i32,
	keyWeaponFour:    i32,
	keyWeaponFive:    i32,
}

Values: SettingData

Initialize :: proc()
{
	if (Read())
	{
		fmt.printfln("Settings file read and loaded succesfully!")
	}
	else
	{
		fmt.printfln("Reading settings failed, creating a new settings file...")
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
	// We have to cast these to integers since Odin Raylib handles them as enums
	defaults.keyMoveForward = cast(i32)rl.KeyboardKey.W
	defaults.keyMoveBackward = cast(i32)rl.KeyboardKey.S
	defaults.keyMoveLeft = cast(i32)rl.KeyboardKey.A
	defaults.keyMoveRight = cast(i32)rl.KeyboardKey.D
	defaults.keyFire = cast(i32)rl.MouseButton.LEFT
	defaults.keyUse = cast(i32)rl.KeyboardKey.E
	defaults.keyJump = cast(i32)rl.KeyboardKey.SPACE
	defaults.keyWeaponOne = cast(i32)rl.KeyboardKey.ONE
	defaults.keyWeaponTwo = cast(i32)rl.KeyboardKey.TWO
	defaults.keyWeaponThree = cast(i32)rl.KeyboardKey.THREE
	defaults.keyWeaponFour = cast(i32)rl.KeyboardKey.FOUR
	defaults.keyWeaponFive = cast(i32)rl.KeyboardKey.FIVE

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
	strings.write_string(&sb, fmt.aprintfln("screenWidth=%[0]v", Values.screenWidth))
	strings.write_string(&sb, fmt.aprintfln("screenHeight=%[0]v", Values.screenHeight))
	strings.write_string(&sb, fmt.aprintfln("mouseSensitivity=%[0]v", Values.mouseSensitivity))
	strings.write_string(&sb, fmt.aprintfln("cameraFOV=%[0]v", Values.cameraFOV))
	strings.write_string(&sb, fmt.aprintfln("maxFPS=%[0]v", Values.maxFPS))
	strings.write_string(&sb, fmt.aprintfln("keyMoveForward=%[0]v", Values.keyMoveForward))
	strings.write_string(&sb, fmt.aprintfln("keyMoveBackward=%[0]v", Values.keyMoveBackward))
	strings.write_string(&sb, fmt.aprintfln("keyMoveLeft=%[0]v", Values.keyMoveLeft))
	strings.write_string(&sb, fmt.aprintfln("keyMoveRight=%[0]v", Values.keyMoveRight))
	strings.write_string(&sb, fmt.aprintfln("keyFire=%[0]v", Values.keyFire))
	strings.write_string(&sb, fmt.aprintfln("keyUse=%[0]v", Values.keyUse))
	strings.write_string(&sb, fmt.aprintfln("keyJump=%[0]v", Values.keyJump))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponOne=%[0]v", Values.keyWeaponOne))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponTwo=%[0]v", Values.keyWeaponTwo))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponThree=%[0]v", Values.keyWeaponThree))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponFour=%[0]v", Values.keyWeaponFour))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponFive=%[0]v", Values.keyWeaponFive))
	stringToWrite := strings.to_string(sb)

	fmt.printfln("Write  %[0]v", stringToWrite)

	data := transmute([]u8)stringToWrite

	success := os.write_entire_file(FileName, data)
	if (!success)
	{
		fmt.println("Writing settings file failed!")
	}
}

Parse :: proc(key: string, value: string)
{
	switch key
	{
		case "screenWidth":
			Values.screenWidth = cast(i32)strconv.atoi(value)
		case "screenHeight":
			Values.screenHeight = cast(i32)strconv.atoi(value)
		case "mouseSensitivity":
			Values.mouseSensitivity = cast(f32)strconv.atof(value)
		case "cameraFOV":
			Values.cameraFOV = cast(i32)strconv.atoi(value)
		case "maxFPS":
			Values.maxFPS = cast(i32)strconv.atoi(value)
		case "keyMoveForward":
			Values.keyMoveForward = cast(i32)strconv.atoi(value)
		case "keyMoveBackward":
			Values.keyMoveBackward = cast(i32)strconv.atoi(value)
		case "keyMoveLeft":
			Values.keyMoveLeft = cast(i32)strconv.atoi(value)
		case "keyMoveRight":
			Values.keyMoveRight = cast(i32)strconv.atoi(value)
		case "keyFire":
			Values.keyFire = cast(i32)strconv.atoi(value)
		case "keyUse":
			Values.keyUse = cast(i32)strconv.atoi(value)
		case "keyJump":
			Values.keyJump = cast(i32)strconv.atoi(value)
		case "keyWeaponOne":
			Values.keyWeaponOne = cast(i32)strconv.atoi(value)
		case "keyWeaponTwo":
			Values.keyWeaponTwo = cast(i32)strconv.atoi(value)
		case "keyWeaponThree":
			Values.keyWeaponThree = cast(i32)strconv.atoi(value)
		case "keyWeaponFour":
			Values.keyWeaponFour = cast(i32)strconv.atoi(value)
		case "keyWeaponFive":
			Values.keyWeaponFive = cast(i32)strconv.atoi(value)
		case:
			fmt.printfln("Key or value does not exist: Key %[0]v - Value %[1]v", key, value)
	}
}
