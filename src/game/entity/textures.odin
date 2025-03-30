package entity
import "core:strings"
import rl "vendor:raylib"

//TODO create new(rl.Texture2D) that makes a pointer for the texture,
// then reuse that texture for all these entities
// can use "rl.IsTextureValid" for checking
// this avoids the reallocation of same item to memory
// Could move all these textures in their own file, then add initializer there
// which will load all the textures at startup. They will be cleaned on game quit.

TextureWallCargo := new(rl.Texture2D)
TextureWallCargoScuffed := new(rl.Texture2D)

LoadTextures :: proc() {
	CreateTexture("./assets/textures/wall1.png", TextureWallCargo, true, false)
	CreateTexture("./assets/textures/wall2.png", TextureWallCargoScuffed, true, false)
}

CreateTexture :: proc(texturePath: string, texturePointer: ^rl.Texture2D, flipHorizontal: bool, flipVertical: bool) {
	// Skip creating if the texture is valid
	if (rl.IsTextureValid(texturePointer^)){
		return;
	}
 	image :=  rl.LoadImage(strings.clone_to_cstring(texturePath))
	defer rl.UnloadImage(image)
 	if (flipHorizontal){
 		rl.ImageFlipHorizontal(&image)
 	}
 	if (flipVertical){
 		rl.ImageFlipVertical(&image)
 	}
 	texturePointer^ = rl.LoadTextureFromImage(image)
	
}
