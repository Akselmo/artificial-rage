#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include "enemy.h"
#include "hud.h"
#include "item.h"
#include "level.h"
#include "player.h"
#include "raylib.h"
#include "settings.h"
#include "utilities.h"
#include "weapon.h"
#include <stdio.h>
#include <stdlib.h>

void Game_Initialize();
void Game_Update();

extern bool Game_isStarted;

#endif
