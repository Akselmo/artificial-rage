#include "level.h"
#include "raylib.h"

// Level has level data, Level_enemies, Level_items and Level_Projectiles
// Level is basically the "scene"

// Public variables
Level_Data* Level_data        = NULL;
Enemy_Data* Level_enemies     = NULL;
Item_Data* Level_items        = NULL;  // Contains interactable and non-interactable Level_items
Projectile* Level_projectiles = NULL;

Vector3 Level_mapPosition;
Vector3 Level_startPosition;
Vector3 Level_endPosition;
int Level_mapSize = 0;

// Private variables
Level_BlockType Level_BlockTypes;
Color* levelMapPixels = NULL;
Texture2D levelCubicMap;
Image levelImageMap;
Vector3 ceilingRotation = { -1.0f, 0.0f, 0.0f };
Model planeFloor;
Material* floorMaterial = NULL;
Model planeCeiling;
Material* ceilingMaterial = NULL;
char* wallTextures[2];

// Private functions
void Level_PlaceBlocks();
void Level_AllocateMeshData(Mesh* mesh, int triangleCount);
void Level_SetBlockTypes();
void Level_UpdateProjectiles();

// TODO: Add integer so you can select which level to load
//       Load textures from file, instead of being built into EXE
//
void Level_Build()
{
    // Initialize block types
    Level_SetBlockTypes();

    // Load level cubicmap image (RAM)
    levelImageMap = LoadImage("./assets/level1/level.png");
    levelCubicMap = LoadTextureFromImage(levelImageMap);

    // Get map image data to be used for collision detection
    levelMapPixels = LoadImageColors(levelImageMap);

    Level_PlaceBlocks();

    // Unload image from RAM
    UnloadImage(levelImageMap);
}

// TODO: Entities can be moved here! Map file can be one png.
//       Move everything in this method, and change it's name
//  should first calculate the needed size of Level_items, then redo it and add the Level_items =
//  more memory efficient
void Level_PlaceBlocks()
{
    // Place all Level_items based on their colors

    float mapPosZ            = (float)levelCubicMap.height;
    float mapPosX            = (float)levelCubicMap.width;
    Texture2D ceilingTexture = LoadTexture("./assets/level1/ceiling.png");
    Texture2D floorTexture   = LoadTexture("./assets/level1/floor.png");
    planeCeiling             = LoadModelFromMesh(Level_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
    planeFloor               = LoadModelFromMesh(Level_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

    planeCeiling.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ceilingTexture;
    planeFloor.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture   = floorTexture;

    // NOTE: By default each cube is mapped to one part of texture atlas
    // Load map texture, hardcoded for now.
    wallTextures[0] = "./assets/level1/wall1.png";
    wallTextures[1] = "./assets/level1/wall2.png";

    Level_mapPosition = (Vector3) { -mapPosX / 2, 0.5f, -mapPosZ / 2 };
    Level_mapSize     = levelCubicMap.height * levelCubicMap.width;

    Level_data        = calloc(Level_mapSize, sizeof(Level_Data));
    Level_enemies     = calloc(Level_mapSize, sizeof(Enemy_Data));
    Level_projectiles = calloc(MAX_PROJECTILE_AMOUNT, sizeof(Projectile));

    for(int y = 0; y < levelCubicMap.height; y++)
    {
        for(int x = 0; x < levelCubicMap.width; x++)
        {

            float mx = Level_mapPosition.x - 0.5f + x * 1.0f;
            float my = Level_mapPosition.z - 0.5f + y * 1.0f;
            int i    = y * levelCubicMap.width + x;

            const Color pixelColor =
                Utilities_GetLevelPixelColor(levelMapPixels, x, levelCubicMap.width, y);

            // Find walls, which is white (255,255,255)
            if(Utilities_CompareColors(pixelColor, Level_BlockTypes.wallColor))
            {

                Texture2D texture = LoadTexture(wallTextures[GetRandomValue(0, 1)]);
                // Set map diffuse texture
                Mesh cube       = GenMeshCube(1.0f, 1.0f, 1.0f);
                Model cubeModel = LoadModelFromMesh(cube);
                cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

                Level_data[i].blockModel       = cubeModel;
                Level_data[i].blockPosition    = (Vector3) { mx, Level_mapPosition.y, my };
                Level_data[i].modelId          = WALL_MODEL_ID;
                Level_data[i].blockSize        = (Vector3) { 1.0f, 1.0f, 1.0f };
                Level_data[i].blockBoundingBox = Utilities_MakeBoundingBox(
                    (Vector3) { mx, Level_mapPosition.y, my }, (Vector3) { 1.0f, 1.0f, 1.0f });
            }

            // Find start, which is green (0,255,0)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.startColor))
            {
                Level_startPosition = (Vector3) { mx, 0.0f, my };
            }

            // Find end, which is blue (0,0,255)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.endColor))
            {
                Level_endPosition = (Vector3) { mx, 0.0f, my };
            }

            // Find enemy, which is red (255,0,0)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.enemyColor))
            {
                Level_enemies[i] = Enemy_Add(mx, my, i);
            }

            // TODO: More entities. For entities and their RGB values: check README.md
        }
    }

    printf("Level has total %d blocks \n", Level_mapSize);
}

void Level_Update()
{
    if (!Game_isStarted)
    {
        return;
    }

    DrawModel(
        planeFloor, (Vector3) { Level_mapPosition.x, 0.0f, Level_mapPosition.z }, 1.0f, WHITE);
    DrawModelEx(planeCeiling,
                (Vector3) { Level_mapPosition.x, 1.0f, -Level_mapPosition.z },
                ceilingRotation,
                180.0f,
                (Vector3) { 1.0f, 1.0f, 1.0f },
                WHITE);

    for(int i = 0; i < Level_mapSize; i++)
    {
        Level_Data* data  = &Level_data[i];
        Enemy_Data* enemy = &Level_enemies[i];
        if(data != NULL && data->modelId != 0)
        {
            DrawModel(data->blockModel, data->blockPosition, 1.0f, WHITE);
        }

        if(enemy != NULL && enemy->id != 0)
        {
            // if Level_enemies[i] has nothing dont do anything
            if(!enemy->dead)
            {
                Enemy_Update(enemy);
            }
        }
    }
    Level_UpdateProjectiles();
}

void Level_UpdateProjectiles()
{
    for(int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        Projectile* projectile = &Level_projectiles[i];
        if(projectile != NULL)
        {
            Projectile_Update(projectile);
        }
    }
}

bool Level_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId)
{
    BoundingBox entityBox = Utilities_MakeBoundingBox(entityPos, entitySize);

    for(int i = 0; i < Level_mapSize; i++)
    {
        // Level blocks

        // Player and walls/enemies
        if(CheckCollisionBoxes(entityBox, Level_data[i].blockBoundingBox))
        {
            return true;
        }
        // Enemy and wall/other enemies
        // Enemies ignore themselves so they dont collide to themselve. Enemies also ignore their
        // own projectiles
        else if(CheckCollisionBoxes(entityBox, Level_enemies[i].boundingBox) &&
                Level_enemies[i].id != entityId)
        {
            return true;
        }
    }
    return false;
}

Mesh Level_MakeCustomPlaneMesh(float height, float width, float textureSize)
{
    // X width, Z height
    Mesh mesh = { 0 };
    Level_AllocateMeshData(&mesh, 2);

    // First triangle
    mesh.vertices[0]  = 0;
    mesh.vertices[1]  = 0;
    mesh.vertices[2]  = 0;
    mesh.normals[0]   = 0;
    mesh.normals[1]   = 1;
    mesh.normals[2]   = 0;
    mesh.texcoords[0] = 0;
    mesh.texcoords[1] = 0;

    mesh.vertices[3]  = width;
    mesh.vertices[4]  = 0;
    mesh.vertices[5]  = height;
    mesh.normals[3]   = 0;
    mesh.normals[4]   = 1;
    mesh.normals[5]   = 0;
    mesh.texcoords[2] = width / textureSize;
    mesh.texcoords[3] = height / textureSize;

    // Second triangle
    mesh.vertices[6]  = width;
    mesh.vertices[7]  = 0;
    mesh.vertices[8]  = 0;
    mesh.normals[6]   = 0;
    mesh.normals[7]   = 1;
    mesh.normals[8]   = 0;
    mesh.texcoords[4] = width / textureSize;
    mesh.texcoords[5] = 0;

    mesh.vertices[9]  = 0;
    mesh.vertices[10] = 0;
    mesh.vertices[11] = 0;
    mesh.normals[9]   = 0;
    mesh.normals[10]  = 1;
    mesh.normals[11]  = 0;
    mesh.texcoords[6] = 0;
    mesh.texcoords[7] = 0;

    mesh.vertices[12] = 0;
    mesh.vertices[13] = 0;
    mesh.vertices[14] = height;
    mesh.normals[12]  = 0;
    mesh.normals[13]  = 1;
    mesh.normals[14]  = 0;
    mesh.texcoords[8] = 0;
    mesh.texcoords[9] = height / textureSize;

    mesh.vertices[15]  = width;
    mesh.vertices[16]  = 0;
    mesh.vertices[17]  = height;
    mesh.normals[15]   = 0;
    mesh.normals[16]   = 1;
    mesh.normals[17]   = 0;
    mesh.texcoords[10] = width / textureSize;
    mesh.texcoords[11] = height / textureSize;

    UploadMesh(&mesh, false);

    return mesh;
}

void Level_AllocateMeshData(Mesh* mesh, int triangleCount)
{
    mesh->vertexCount   = triangleCount * 3;
    mesh->triangleCount = triangleCount;

    mesh->vertices  = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->texcoords = (float*)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
    mesh->normals   = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
}

void Level_SetBlockTypes()
{
    Level_BlockTypes.startColor = (Color) { 0, 255, 0 };
    Level_BlockTypes.endColor   = (Color) { 0, 0, 255 };
    Level_BlockTypes.wallColor  = (Color) { 255, 255, 255 };
    Level_BlockTypes.enemyColor = (Color) { 255, 0, 0 };
    Level_BlockTypes.NONE       = (Color) { 0, 0, 0 };
}