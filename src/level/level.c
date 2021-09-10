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
int levelBlockAmount;

//Enemies
Enemy *enemies = NULL;

//Items (contains interactable and non-interactable items)
Item *items = NULL;


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
    
    int enemyAmount = 1;
    int i = 1;

    LevelData* levelData = malloc(i*sizeof(LevelData));
    Enemy* enemies = malloc(enemyAmount*sizeof(Enemy));
    
    for (int y = 0; y < levelCubicMap.height; y++)
    {
        for (int x = 0; x < levelCubicMap.width; x++)
        {

            float mx = levelMapPosition.x - 0.5f + x * 1.0f;
            float my = levelMapPosition.z - 0.5f + y * 1.0f;

            Rectangle rect = (Rectangle){mx, my, 1.0f, 1.0f};

            //Find walls, which is white (255,255,255)
            if (CheckPixelForColor(x, levelCubicMap.width, y, 255, 255, 255))
            {

                Texture2D texture = LoadTexture(wallTextures[GetRandomValue(0, 1)]);
                // Set map diffuse texture
                Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
                Model cubeModel = LoadModelFromMesh(cube);
                cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                
                ulong d = sizeof(LevelData);

                LevelData* tmpLevel = realloc(levelData, (i+1) * sizeof(LevelData));
                if (tmpLevel)
                {
                    levelData = tmpLevel;
                    levelData->levelBlockModel = cubeModel;
                    levelData->levelBlockPosition = (Vector3){mx, levelMapPosition.y, my};
                    levelData->modelId = i;
                }
                i++;
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
                Enemy* tmpEnemies = realloc(enemies, (enemyAmount+1) * sizeof(Enemy));
                if (tmpEnemies)
                {
                    enemies = tmpEnemies;
                    *enemies = AddEnemy(mx,my);
                }
                enemyAmount++;
            }
            //TODO:
            //For entities and their RGB values: check README.md
            
        }
    }
    
    levelBlockAmount = i;
    printf("Level has total %d blocks \n", levelBlockAmount);
}



void DrawLevel()
{

    DrawModel(planeFloor, (Vector3){levelMapPosition.x, 0.0f, levelMapPosition.z}, 1.0f, WHITE);
    DrawModelEx(planeCeiling, (Vector3){levelMapPosition.x, 1.0f, -levelMapPosition.z}, ceilingRotation, 180.0f, (Vector3){1.0f, 1.0f, 1.0f}, WHITE);
    LevelData *loopLevelData = levelData;
    Enemy *loopEnemies = enemies;
    Item *loopItems = items;
    for (int i = 0; i < MAX_LEVEL_SIZE; i++)
    {
        if (loopLevelData != NULL)
        {
            DrawModel(loopLevelData->levelBlockModel, levelData->levelBlockPosition, 1.0f, WHITE);
            loopLevelData++;
        }
        if (loopEnemies != NULL)
        {
            UpdateEnemy(*loopEnemies);
            loopEnemies++;
        }
        if (loopItems != NULL)
        {
            UpdateItem(*loopItems); //commented out due to making the game slow
            loopItems++;
        }
    }
}

bool CheckLevelCollision(Vector3 entityPos, Vector3 entitySize)
{
    LevelData *levelData = GetLevelData();

    BoundingBox entityBox = (BoundingBox){(Vector3){entityPos.x - entitySize.x / 2,
                                                    entityPos.y - entitySize.y / 2,
                                                    entityPos.z - entitySize.z / 2},
                                          (Vector3){entityPos.x + entitySize.x / 2,
                                                    entityPos.y + entitySize.y / 2,
                                                    entityPos.z + entitySize.z / 2}};

    for (int i = 0; i < levelBlockAmount; i++)
    {
        Vector3 wallPos = levelData[i].levelBlockPosition;
        Vector3 wallSize = {1.0f, 1.0f, 1.0f};
        BoundingBox levelBox = (BoundingBox){(Vector3){wallPos.x - wallSize.x / 2,
                                                       wallPos.y - wallSize.y / 2,
                                                       wallPos.z - wallSize.z / 2},
                                             (Vector3){wallPos.x + wallSize.x / 2,
                                                       wallPos.y + wallSize.y / 2,
                                                       wallPos.z + wallSize.z / 2}};

        //DrawBoundingBox(levelBox,GREEN);

        if (CheckCollisionBoxes(entityBox, levelBox))
        {
            return true;
        }
    }
    return false;
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
    return levelBlockAmount;
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