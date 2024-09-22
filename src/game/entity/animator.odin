package entity

import "core:fmt"
import rl "vendor:raylib"

Animation :: struct {
	animation:     rl.ModelAnimation,
	id:            i32,
	firstFrame:    i32, // For trimming purposes. Some animations can have extra t-pose time,            // etc...
	lastFrame:     i32, // This means that 0/maxFrameCount is not always the spot where the            // animation starts/stops
	loopable:      bool,
	interruptable: bool,
}

Animator :: struct {
	animations:       [dynamic]Animation,
	animationsCount:  i32,
	currentAnimation: Animation,
	animationFrame:   i32,
	nextFrame:        f32,
}


SetAnimation :: proc(animator: ^Animator, id: i32) {

	currentAnimation := animator.currentAnimation
	newAnimation := animator.animations[id]

	if (currentAnimation.id == newAnimation.id) {
		return
	}

	if (!currentAnimation.interruptable) {
		if (animator.animationFrame < currentAnimation.lastFrame) {
			return
		}
	}

	animator.animationFrame = currentAnimation.firstFrame
	animator.currentAnimation = newAnimation
}

// TODO: This would need somekind of frameskip feature if FPS is lower than animationspeed.
PlayAnimation :: proc(animator: ^Animator, model: ^rl.Model, speed: f32, oldFrame: f32) -> f32 {
	nextFrame := oldFrame

	if (nextFrame > 0) {
		nextFrame -= rl.GetFrameTime()
	} else {
		currentAnimation := animator.currentAnimation
		animator.animationFrame += 1
		if (animator.animationFrame > currentAnimation.lastFrame) {
			if (currentAnimation.loopable) {
				animator.animationFrame = currentAnimation.firstFrame
			} else {
				animator.animationFrame = currentAnimation.lastFrame
			}
		}
		rl.UpdateModelAnimation(model^, currentAnimation.animation, animator.animationFrame)
		nextFrame = 1.0 / speed
	}
	return nextFrame
}

