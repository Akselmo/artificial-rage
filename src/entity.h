#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "raylib.h"

// Remember to update this if you add more entities to below
#define ENTITIES_TOTAL 6

enum Entity_Type
{
    SCENE_NONE,
    SCENE_START,
    SCENE_END,
    SCENE_WALL,
    SCENE_ACTOR,
};

typedef struct Entity_Data
{
    int id;
    Vector3 position;
    Vector3 size;
    Color mapColor;
    enum Entity_Type type;
    char *fileName;
    BoundingBox boundingBox;
    Model model;
    bool loaded;
} Entity_Data;

extern Entity_Data Entity_none;
extern Entity_Data Entity_start;
extern Entity_Data Entity_end;
extern Entity_Data Entity_wall1;
extern Entity_Data Entity_wall2;
extern Entity_Data Entity_enemy;

#endif