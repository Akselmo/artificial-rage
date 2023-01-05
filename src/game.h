#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include "actor.h"
#include "hud.h"
#include "item.h"
#include "player.h"
#include "raylib.h"
#include "scene.h"
#include "settings.h"
#include "utilities.h"
#include "weapon.h"
#include <stdio.h>
#include <stdlib.h>

void Game_Initialize(void);
void Game_Update(void);

extern bool Game_isStarted;

#endif
