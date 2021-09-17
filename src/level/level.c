#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/raylib.h"
#include "../../include/raymath.h"
#include "level.h"
#include "../main.h"
#include "../player/player.h"
#include "../enemy/enemy.h"
#include "../item/item.h"

//Level has level data, enemies and items

//Level
int mapSize = 0;

LevelData *levelData = NULL;
Color *levelMapPixels = NULL;
Vector3 levelMapPosition;
Texture2D levelCubicMap;
Image levelImageMap;
Vector3 ceilingRotation = {-1.0f, 0.0f, 0.0f};
Model planeFloor;
Material *floorMaterial = NULL;
Model planeCeiling;
Material *ceilingMaterial= NULL;
char *wallTextures[2];
Vector3 levelStartPosition;
Vector3 levelEndPosition;

//Enemies
Enemy *enemies = NULL;


//Items (contains interactable and non-interactable items)
Item *items = NULL;

//PROTOTYPES
BoundingBox MakeBoundingBox(Vector3 position, Vector3 size);

bool CheckPixelForColor(int x, int width, int y, int r, int g, int b)
{
    if (levelMapPixels[y * width + x].r == r &&
        levelMapPixels[y * width + x].g == g &&
        levelMapPixels[y * width + x].b == b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//TODO: Entities can be moved here! Map file can be one png.
//      Move everything in this method, and change it's name
// should first calculate the needed size of items, then redo it and add the items = more memory efficient
void PlaceLevelBlocks()
{
    // Place all items based on their colors

    float mapPosZ = (float)levelCubicMap.height;
    float mapPosX = (float)levelCubicMap.width;

    Texture2D ceilingTexture = LoadTexture("../assets/ceiling.png");
    Texture2D floorTexture = LoadTexture("../assets/floor.png");
    planeCeiling = LoadModelFromMesh(MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
    planeFloor = LoadModelFromMesh(MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

    planeCeiling.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ceilingTexture;
    planeFloor.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = floorTexture;

    // NOTE: By default each cube is mapped to one part of texture atlas
    // Load map texture, hardcoded for now.
    wallTextures[0] = "../assets/wall1.png";
    wallTextures[1] = "../assets/wall2.png";

    levelMapPosition = (Vector3){-mapPosX / 2, 0.5f, -mapPosZ / 2};
    mapSize = levelCubicMap.height * levelCubicMap.width;

    levelData = calloc(mapSize, sizeof(LevelData));
    enemies = calloc(mapSize, sizeof(Enemy));
    
    for (int y = 0; y < levelCubicMap.height; y++)
    {
        for (int x = 0; x < levelCubicMap.width; x++)
        {

            float mx = levelMapPosition.x - 0.5f + x * 1.0f;
            float my = levelMapPosition.z - 0.5f + y * 1.0f;
            int i = y * levelCubicMap.width + x;

            Rectangle rect = (Rectangle){mx, my, 1.0f, 1.0f};

            //Find walls, which is white (255,255,255)
            if (CheckPixelForColor(x, levelCubicMap.width, y, 255, 255, 255))
            {

                Texture2D texture = LoadTexture(wallTextures[GetRandomValue(0, 1)]);
                // Set map diffuse texture
                Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
                Model cubeModel = LoadModelFromMesh(cube);
                cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                
                levelData[i].levelBlockModel = cubeModel;
                levelData[i].levelBlockPosition = (Vector3){mx, levelMapPosition.y, my};
                levelData[i].modelId = i;
            }

            //Find start, which is green (0,255,0)
            if (CheckPixelForColor(x, levelCubicMap.width, y, 0, 255, 0))
            {
                levelStartPosition = (Vector3){mx, 0.0f, my};
            }

            //Find end, which is blue (0,0,255)
            if (CheckPixelForColor(x, levelCubicMap.width, y, 0, 0, 255))
            {
                levelEndPosition = (Vector3){mx, 0.0f, my};
            }

            //Find enemy, which is red (255,0,0)
            if (CheckPixelForColor(x, levelCubicMap.width, y, 255, 0, 0))
            {
                enemies[i] = AddEnemy(mx,my);
            }
            //TODO:
            //For entities and their RGB values: check README.md
            
        }
    }
    
    printf("Level has total %d blocks \n", mapSize);
}



void DrawLevel()
{

    DrawModel(planeFloor, (Vector3){levelMapPosition.x, 0.0f, levelMapPosition.z}, 1.0f, WHITE);
    DrawModelEx(planeCeiling, (Vector3){levelMapPosition.x, 1.0f, -levelMapPosition.z}, ceilingRotation, 180.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);


    for (int i = 0; i < mapSize; i++)
    {
        if (&levelData[i] != NULL)
        {
            DrawModel(levelData[i].levelBlockModel, levelData[i].levelBlockPosition, 1.0f, WHITE);
        }

        if (&enemies[i] != NULL)
        {
            //if enemies[i] has nothing dont do anything
            UpdateEnemy(enemies[i]);
        }
    }
    
}

bool CheckLevelCollision(Vector3 entityPos, Vector3 entitySize)
{

    BoundingBox entityBox = MakeBoundingBox(entityPos, entitySize);
    
    for (int i = 0; i < mapSize; i++)
    {
        //Level blocks
        Vector3 wallPos = levelData[i].levelBlockPosition;
        Vector3 wallSize = {1.0f, 1.0f, 1.0f};
        BoundingBox levelBox = MakeBoundingBox(wallPos, wallSize);

        //Enemies
        Vector3 enemyPos = enemies[i].position;
        Vector3 enemySize = enemies[i].size;
        BoundingBox enemyBox = MakeBoundingBox(enemyPos, enemySize);


        if (CheckCollisionBoxes(entityBox, levelBox))
        {
            return true;
        }
        else if (CheckCollisionBoxes(entityBox, enemyBox))
        {
            return true;
        }
    }
    return false;
}

BoundingBox MakeBoundingBox(Vector3 position, Vector3 size)
{
    BoundingBox bb = (BoundingBox){(Vector3){position.x - size.x / 2,
                                                   position.y - size.y / 2,
                                                   position.z - size.z / 2},
                                          (Vector3){position.x + size.x / 2,
                                                   position.y + size.y / 2,
                                                   position.z + size.z / 2}};
    return bb;
}

void AllocateMeshData(Mesh *mesh, int triangleCount)
{
    mesh->vertexCount = triangleCount * 3;
    mesh->triangleCount = triangleCount;

    mesh->vertices = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->texcoords = (float *)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
    mesh->normals = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
}

Mesh MakeCustomPlaneMesh(float height, float width, float textureSize)
{
    //X width, Z height
    Mesh mesh = {0};
    AllocateMeshData(&mesh, 2);

    //First triangle
    mesh.vertices[0] = 0;
    mesh.vertices[1] = 0;
    mesh.vertices[2] = 0;
    mesh.normals[0] = 0;
    mesh.normals[1] = 1;
    mesh.normals[2] = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    mesh.vertices[3] = width;
    mesh.vertices[4] = 0;
    mesh.vertices[5] = height;
    mesh.normals[3] = 0;
    mesh.normals[4] = 1;
    mesh.normals[5] = 0;
    mesh.texcoords[2] = width / textureSize;
    mesh.texcoords[3] = height / textureSize;

    //Second triangle
    mesh.vertices[6] = width;
    mesh.vertices[7] = 0;
    mesh.vertices[8] = 0;
    mesh.normals[6] = 0;
    mesh.normals[7] = 1;
    mesh.normals[8] = 0;
    mesh.texcoords[4] = width / textureSize;
    mesh.texcoords[5] = 0;

    mesh.vertices[9] = 0;
    mesh.vertices[10] = 0;
    mesh.vertices[11] = 0;
    mesh.normals[9] = 0;
    mesh.normals[10] = 1;
    mesh.normals[11] = 0;
    mesh.texcoords[6] = 0;
    mesh.texcoords[7] = 0;

    mesh.vertices[12] = 0;
    mesh.vertices[13] = 0;
    mesh.vertices[14] = height;
    mesh.normals[12] = 0;
    mesh.normals[13] = 1;
    mesh.normals[14] = 0;
    mesh.texcoords[8] = 0;
    mesh.texcoords[9] = height / textureSize;

    mesh.vertices[15] = width;
    mesh.vertices[16] = 0;
    mesh.vertices[17] = height;
    mesh.normals[15] = 0;
    mesh.normals[16] = 1;
    mesh.normals[17] = 0;
    mesh.texcoords[10] = width / textureSize;
    mesh.texcoords[11] = height / textureSize;

    UploadMesh(&mesh, false);

    return mesh;
}

LevelData *GetLevelData()
{
    return levelData;
}

Vector3 GetMapPosition()
{
    return levelMapPosition;
}

Vector3 GetLevelStartPosition()
{
    return levelStartPosition;
}

int GetLevelBlockAmount()
{
    return mapSize;
}

//TODO: Add integer so you can select which level to load
//      Load textures from file, instead of being built into EXE
//
void BuildLevel()
{
    // Load level cubicmap image (RAM)
    levelImageMap = LoadImage("../assets/level.png");
    levelCubicMap = LoadTextureFromImage(levelImageMap);

    // Get map image data to be used for collision detection
    levelMapPixels = LoadImageColors(levelImageMap);

    PlaceLevelBlocks();

    // Unload image from RAM
    UnloadImage(levelImageMap);
}
