package settings

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"
import rl "vendor:raylib"

GameTitle: cstring = "Artificial Rage"
FileName: string = "settings.cfg"

screenWidth: i32
screenHeight: i32
mouseSensitivity: f32
cameraFOV: f32
maxFPS: i32
keyMoveForward: i32
keyMoveBackward: i32
keyMoveRight: i32
keyMoveLeft: i32
keyFire: i32
keyUse: i32
keyJump: i32
keyWeaponOne: i32
keyWeaponTwo: i32
keyWeaponThree: i32
keyWeaponFour: i32
keyWeaponFive: i32

Initialize :: proc() {
	if (Read()) {
		fmt.printfln("Settings file read and loaded succesfully!")
	} else {
		fmt.printfln("Reading settings failed, creating a new settings file...")
		SetDefaults()
		Write()
	}

}

SetDefaults :: proc() {

	screenWidth = 800
	screenHeight = 600
	mouseSensitivity = 0.25
	cameraFOV = 90
	maxFPS = 200
	// We have to cast these to integers since Odin Raylib handles them as enums
	keyMoveForward = cast(i32)rl.KeyboardKey.W
	keyMoveBackward = cast(i32)rl.KeyboardKey.S
	keyMoveLeft = cast(i32)rl.KeyboardKey.A
	keyMoveRight = cast(i32)rl.KeyboardKey.D
	keyFire = cast(i32)rl.MouseButton.LEFT
	keyUse = cast(i32)rl.KeyboardKey.E
	keyJump = cast(i32)rl.KeyboardKey.SPACE
	keyWeaponOne = cast(i32)rl.KeyboardKey.ONE
	keyWeaponTwo = cast(i32)rl.KeyboardKey.TWO
	keyWeaponThree = cast(i32)rl.KeyboardKey.THREE
	keyWeaponFour = cast(i32)rl.KeyboardKey.FOUR
	keyWeaponFive = cast(i32)rl.KeyboardKey.FIVE
}

Read :: proc() -> bool {
	data, success := os.read_entire_file(FileName)
	defer delete(data)
	if (!success) {
		return false
	}
	settingsFileText := string(data)
	settingsArr := strings.split_lines(settingsFileText)
	for setting in settingsArr {
		keyValuePair := strings.split(setting, "=")
		if (len(keyValuePair) < 2) {
			continue
		}
		Parse(keyValuePair[0], keyValuePair[1])
	}
	return true

}

Write :: proc() {
	sb := strings.builder_make()
	strings.write_string(&sb, fmt.aprintfln("screenWidth=%[0]v", screenWidth))
	strings.write_string(&sb, fmt.aprintfln("screenHeight=%[0]v", screenHeight))
	strings.write_string(&sb, fmt.aprintfln("mouseSensitivity=%[0]v", mouseSensitivity))
	strings.write_string(&sb, fmt.aprintfln("cameraFOV=%[0]v", cameraFOV))
	strings.write_string(&sb, fmt.aprintfln("maxFPS=%[0]v", maxFPS))
	strings.write_string(&sb, fmt.aprintfln("keyMoveForward=%[0]v", keyMoveForward))
	strings.write_string(&sb, fmt.aprintfln("keyMoveBackward=%[0]v", keyMoveBackward))
	strings.write_string(&sb, fmt.aprintfln("keyMoveLeft=%[0]v", keyMoveLeft))
	strings.write_string(&sb, fmt.aprintfln("keyMoveRight=%[0]v", keyMoveRight))
	strings.write_string(&sb, fmt.aprintfln("keyFire=%[0]v", keyFire))
	strings.write_string(&sb, fmt.aprintfln("keyUse=%[0]v", keyUse))
	strings.write_string(&sb, fmt.aprintfln("keyJump=%[0]v", keyJump))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponOne=%[0]v", keyWeaponOne))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponTwo=%[0]v", keyWeaponTwo))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponThree=%[0]v", keyWeaponThree))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponFour=%[0]v", keyWeaponFour))
	strings.write_string(&sb, fmt.aprintfln("keyWeaponFive=%[0]v", keyWeaponFive))
	stringToWrite := strings.to_string(sb)

	fmt.printfln("Write  %[0]v", stringToWrite)

	data := transmute([]u8)stringToWrite

	success := os.write_entire_file(FileName, data)
	if (!success) {
		fmt.println("Writing settings file failed!")
	}
}

Parse :: proc(key: string, value: string) {

	switch key {
	case "screenWidth":
		screenWidth = cast(i32)strconv.atoi(value)
	case "screenHeight":
		screenHeight = cast(i32)strconv.atoi(value)
	case "mouseSensitivity":
		mouseSensitivity = cast(f32)strconv.atof(value)
	case "cameraFOV":
		cameraFOV = cast(f32)strconv.atoi(value)
	case "maxFPS":
		maxFPS = cast(i32)strconv.atoi(value)
	case "keyMoveForward":
		keyMoveForward = cast(i32)strconv.atoi(value)
	case "keyMoveBackward":
		keyMoveBackward = cast(i32)strconv.atoi(value)
	case "keyMoveLeft":
		keyMoveLeft = cast(i32)strconv.atoi(value)
	case "keyMoveRight":
		keyMoveRight = cast(i32)strconv.atoi(value)
	case "keyFire":
		keyFire = cast(i32)strconv.atoi(value)
	case "keyUse":
		keyUse = cast(i32)strconv.atoi(value)
	case "keyJump":
		keyJump = cast(i32)strconv.atoi(value)
	case "keyWeaponOne":
		keyWeaponOne = cast(i32)strconv.atoi(value)
	case "keyWeaponTwo":
		keyWeaponTwo = cast(i32)strconv.atoi(value)
	case "keyWeaponThree":
		keyWeaponThree = cast(i32)strconv.atoi(value)
	case "keyWeaponFour":
		keyWeaponFour = cast(i32)strconv.atoi(value)
	case "keyWeaponFive":
		keyWeaponFive = cast(i32)strconv.atoi(value)
	case:
		fmt.printfln("Key or value does not exist: Key %[0]v - Value %[1]v", key, value)
	}
}

