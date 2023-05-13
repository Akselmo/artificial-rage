#include "entity.h"

// Entities used in the game
// TODO: More entities. For entities and their RGB values: check README.md
Entity_Data Entity_none = {
    .mapColor = (Color){0, 0, 0, 255},
    .type = SCENE_NONE,
    .fileName = ""};

Entity_Data Entity_start = {
    .mapColor = (Color){0, 255, 0, 255},
    .type = SCENE_START,
    .fileName = ""};

Entity_Data Entity_end = {
    .mapColor = (Color){0, 0, 255, 255},
    .type = SCENE_END,
    .fileName = ""};

Entity_Data Entity_wall1 = {
    .mapColor = (Color){255, 255, 255, 255},
    .type = SCENE_WALL,
    .fileName = "./assets/textures/wall1.png"};

Entity_Data Entity_wall2 = {
    .mapColor = (Color){255, 255, 254, 255},
    .type = SCENE_WALL,
    .fileName = "./assets/textures/wall2.png"};

Entity_Data Entity_enemy = {
    .mapColor = (Color){255, 0, 0, 255},
    .type = SCENE_ACTOR,
    .fileName = "./assets/models/enemy.m3d"};