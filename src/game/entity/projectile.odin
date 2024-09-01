package entity

import "core:fmt"
import "src:game/utilities"
import rl "vendor:raylib"
// Projectiles are just entitites like the rest.
// They have their own dynamic array where they are allocated and deleted

PROJECTILE_TRAVEL_DISTANCE: f32 : 100
PROJECTILE_DESTROY_AFTER_SECONDS: i32 : 10

Projectile :: struct {
	startPosition: rl.Vector3,
	endPosition:   rl.Vector3,
	position:      rl.Vector3,
	size:          rl.Vector3,
	color:         rl.Color,
	model:         rl.Model,
	boundingBox:   rl.BoundingBox,
	id:            i32,
	ownerId:       i32,
	damage:        i32,
	destroyed:     bool,
	speed:         f32,
}

projectilesInScene: [255]Projectile

ProjectileCreate :: proc(rayCast: rl.Ray, size: rl.Vector3, damage: i32, ownerId: i32, color: rl.Color) {

	for i: i32 = 0; i < len(projectilesInScene); i += 1 {
		proj := projectilesInScene[i]
		if (proj.id != i || proj.destroyed == true) {
			newProjectile: Projectile = {
				startPosition = rayCast.position,
				endPosition   = rl.Vector3Add(
					rayCast.position,
					rl.Vector3Scale(rayCast.direction, PROJECTILE_TRAVEL_DISTANCE),
				),
				id            = i,
				ownerId       = ownerId,
				size          = size,
				boundingBox   = utilities.MakeBoundingBox(rayCast.position, size),
				damage        = damage,
				color         = color,
				speed         = 0.12 * rl.GetFrameTime(),
				destroyed     = false,
			}
			newProjectile.position = newProjectile.startPosition
			newProjectile.model = rl.LoadModelFromMesh(
				rl.GenMeshCube(newProjectile.size.x, newProjectile.size.y, newProjectile.size.z),
			)
			fmt.printfln("Projectile id %[0]v created", newProjectile.id)
			projectilesInScene[i] = newProjectile
			ProjectileRotateTowards(&projectilesInScene[i])
			break

		}

	}

}

ProjectileUpdate :: proc(projectile: ^Projectile) {

}

ProjectileCheckCollision :: proc(projectile: ^Projectile) {

}

ProjectileDestroyOverTime :: proc(projectile: ^Projectile) {

}

ProjectileDestroy :: proc(projectile: ^Projectile) {

}

ProjectileRotateTowards :: proc(projectile: ^Projectile) {

}

