#include "entity.h"

// Entities used in the game
// TODO: More entities. For entities and their RGB values: check README.md
Entity_Data Entity_none = {
    .type = SCENE_NONE,
    .fileName = ""};

Entity_Data Entity_start = {
    .type = SCENE_START,
    .fileName = ""};

Entity_Data Entity_end = {
    .type = SCENE_END,
    .fileName = ""};

Entity_Data Entity_wall1 = {
    .type = SCENE_WALL,
    .fileName = "./assets/textures/wall1.png"};

Entity_Data Entity_wall2 = {
    .type = SCENE_WALL,
    .fileName = "./assets/textures/wall2.png"};

Entity_Data Entity_enemy = {
    .type = SCENE_ACTOR,
    .fileName = "./assets/models/enemy.m3d"};