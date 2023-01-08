#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include "hud.h"
#include "raylib.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>

void Game_Initialize(void);
void Game_Update(void);

extern bool Game_isStarted;

#endif
