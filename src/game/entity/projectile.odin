package entity

import rl "vendor:raylib"
// Projectiles are just entitites like the rest.
// They have their own dynamic array where they are allocated and deleted

Projectile :: struct {
	startPosition: rl.Vector3,
	endPosition:   rl.Vector3,
	position:      rl.Vector3,
	size:          rl.Vector3,
	color:         rl.Color,
	model:         rl.Model,
	bounding:      rl.BoundingBox,
	id:            i32,
	ownerId:       i32,
	damage:        i32,
	destroyed:     bool,
	speed:         f32,
}

ProjectileCreate :: proc(rayCast: rl.Ray, size: rl.Vector3, damage: i32, ownerId: i32, color: rl.Color) {

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

