#pragma once
#include "raylib.h"
#ifndef _GAME_H_
#define _GAME_H_

#include <stdbool.h>

void Game_Initialize(void);
void Game_Update(void);

extern Camera Game_camera;
extern bool Game_isStarted;

#endif
