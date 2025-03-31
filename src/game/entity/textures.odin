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
TextureItemAmmoPistol := new(rl.Texture2D)
TextureItemAmmoRifle := new(rl.Texture2D)
TextureItemAmmoShotgun := new(rl.Texture2D)
TextureItemAmmoRailgun := new(rl.Texture2D)
TextureItemHealthSmall := new(rl.Texture2D)
TextureItemHealthMedium := new(rl.Texture2D)
TextureItemHealthLarge := new(rl.Texture2D)
TextureItemPickupKey := new(rl.Texture2D)
TextureItemPickupPistol := new(rl.Texture2D)
TextureItemPickupRifle := new(rl.Texture2D)
TextureItemPickupShotgun := new(rl.Texture2D)
TextureItemPickupRailgun := new(rl.Texture2D)

LoadTextures :: proc() {
	CreateTexture("./assets/textures/wall1.png", TextureWallCargo, true, false)
	CreateTexture("./assets/textures/wall2.png", TextureWallCargoScuffed, true, false)
	CreateTexture("./assets/textures/ammo_pistol.png", TextureItemAmmoPistol, false, true)
	CreateTexture("./assets/textures/ammo_rifle.png", TextureItemAmmoRifle, false, true)
	CreateTexture("./assets/textures/ammo_shotgun.png", TextureItemAmmoShotgun, false, true)
	CreateTexture("./assets/textures/ammo_railgun.png", TextureItemAmmoRailgun, false, true)
	CreateTexture("./assets/textures/health_small.png", TextureItemHealthSmall, false, true)
	CreateTexture("./assets/textures/health_medium.png", TextureItemHealthMedium, false, true)
	CreateTexture("./assets/textures/health_large.png", TextureItemHealthLarge, false, true)
	CreateTexture("./assets/textures/pickup_teleportkey.png", TextureItemPickupKey, false, true)
	CreateTexture("./assets/textures/pistol.png", TextureItemPickupPistol, false, true)
	CreateTexture("./assets/textures/rifle.png", TextureItemPickupRifle, false, true)
	CreateTexture("./assets/textures/shotgun.png", TextureItemPickupShotgun, false, true)
	CreateTexture("./assets/textures/railgun.png", TextureItemPickupRailgun, false, true)
}

CreateTexture :: proc(texturePath: string, texturePointer: ^rl.Texture2D, flipHorizontal: bool, flipVertical: bool) {
	// Skip creating if the texture is valid
	if (rl.IsTextureValid(texturePointer^)) {
		return
	}
	image := rl.LoadImage(strings.clone_to_cstring(texturePath))
	defer rl.UnloadImage(image)
	if (flipHorizontal) {
		rl.ImageFlipHorizontal(&image)
	}
	if (flipVertical) {
		rl.ImageFlipVertical(&image)
	}
	texturePointer^ = rl.LoadTextureFromImage(image)

}

