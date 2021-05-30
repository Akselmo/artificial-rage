#include"../../include/raylib.h"
#include<stdio.h>

//Globals

//Level
Color *levelMapPixels;
Vector3 levelMapPosition;
Texture2D levelCubicMap;
Image levelImageMap;

//Entities
Color* entityMapPixels;
Texture2D entityCubicMap;
Image entityImageMap;

Vector3 startPosition;

//TODO: Add integer so you can select which level to load
//      Create map from different layers: Map layer, entity layer
Model BuildLevel()
{
    // Load level cubicmap image (RAM)
    levelImageMap = LoadImage("../assets/level.png");
    levelCubicMap = LoadTextureFromImage(levelImageMap);

    // Load entity cubicmap 
    entityImageMap = LoadImage("../assets/entities.png");
    entityCubicMap = LoadTextureFromImage(entityImageMap);

    // Convert image to texture to display (VRAM)
    Mesh mesh = GenMeshCubicmap(levelImageMap, (Vector3){ 1.0f, 1.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);

    // NOTE: By default each cube is mapped to one part of texture atlas
    // Load map texture
    Texture2D texture = LoadTexture("../assets/level_texture.png");    
    // Set map diffuse texture
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;             
    // Get map image data to be used for collision detection
    levelMapPixels = LoadImageColors(levelImageMap);
    entityMapPixels = LoadImageColors(entityImageMap);
    // Unload image from RAM
    UnloadImage(levelImageMap);
    UnloadImage(entityImageMap);
    PlaceAllEntities();


    return model;
}

void PlaceAllEntities()
{
    // Place all items based on their colors
    float mapPosZ = (float) entityCubicMap.height;
    float mapPosX = (float) entityCubicMap.width;
    levelMapPosition = (Vector3){ -mapPosX/2, 0.0f, -mapPosZ/2 };

    for (int y = 0; y < entityCubicMap.height; y++)
    {
        for (int x = 0; x < entityCubicMap.width; x++)
        {

            float mx = levelMapPosition.x - 0.5f + x*1.0f;
            float my = levelMapPosition.z - 0.5f + y*1.0f;

            Rectangle rect = (Rectangle){ mx, my, 1.0f, 1.0f };

            //Find start, which is red (255,0,0)
            if (entityMapPixels[y*entityCubicMap.width + x].r == 255 &&
                entityMapPixels[y*entityCubicMap.width + x].g == 0 &&
                entityMapPixels[y*entityCubicMap.width + x].b == 0)
            {
                startPosition = (Vector3) {mx,0.0f,my};
            }
            //TODO: 
            //Green, place goal
            //Yellow, place enemy
            //Blue, place keycard
            //Ammo and stuff??

        }
    }
}

void DrawLevel(Model model)
{
    DrawModel(model, levelMapPosition, 1.0f, WHITE);
}

bool CheckLevelCollision(Vector2 entityPos, float entityRadius)
{
    // Check collision of entity (Player, enemy, etc..)
    int entityCellX = (int)(entityPos.x - levelMapPosition.x + 0.5f);
    int entityCellY = (int)(entityPos.y - levelMapPosition.z + 0.5f);

    // Out-of-limits security check
    if (entityCellX < 0)
    {
        entityCellX = 0;
        return true;
    } 
    else if (entityCellX >= levelCubicMap.width)
    {
        entityCellX = levelCubicMap.width - 1;
        return true;
    }

    if (entityCellY < 0)
    {
        entityCellY = 0;
        return true;
    } 
    else if (entityCellY >= levelCubicMap.height)
    {
        entityCellY = levelCubicMap.height - 1;
        return true;
    }

    

    // Check map collisions using image data and entity position
    for (int y = 0; y < levelCubicMap.height; y++)
    {
        for (int x = 0; x < levelCubicMap.width; x++)
        {

            float mx = levelMapPosition.x - 0.5f + x*1.0f;
            float my = levelMapPosition.z - 0.5f + y*1.0f;

            Rectangle rect = (Rectangle){ mx, my, 1.0f, 1.0f };
            // Collision: white pixel, only check R channel
            if ((levelMapPixels[y*levelCubicMap.width + x].r == 255) && (CheckCollisionCircleRec(entityPos, entityRadius,rect)))
            {
                return true;
            }

        }
    }
    return false;
}

Vector3 GetMapPosition()
{
    return levelMapPosition;
}

Vector3 GetStartPosition()
{
    return startPosition;
}