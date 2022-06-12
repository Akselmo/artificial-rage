#ifndef _HUD_H_
#define _HUD_H_

#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include <stdio.h>
#include <string.h>

void Hud_Draw(int health, int weaponAmmo, int weaponMaxAmmo, int weapon);

#endif