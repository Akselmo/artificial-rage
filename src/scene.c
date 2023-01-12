#include "scene.h"
#include "raylib.h"

// Level has level data, Level_enemies, Level_items and Level_Projectiles
// Level is basically the "scene"

// Public variables
Scene_Data Scene_data = { 0 };

// Private variables
Level_BlockType Level_BlockTypes;

// Private functions
void Scene_PlaceBlocks(Texture2D sceneCubicMap, Color* sceneMapPixels);
void Scene_AllocateMeshData(Mesh* mesh, int triangleCount);
void Scene_SetBlockTypes(void);
void Scene_UpdateProjectiles(void);

Camera Scene_Initialize(void)
{
    Scene_Build();
    return Player_InitializeCamera(Scene_data.startPosition.x, Scene_data.startPosition.z);
}

// TODO: Add integer so you can select which level to load
//       Load textures from file, instead of being built into EXE
//
void Scene_Build(void)
{
    // Initialize block types
    Scene_SetBlockTypes();

    // Load level cubicmap image (RAM)
    const Image sceneImageMap     = LoadImage("./assets/level1/level.png");
    const Texture2D sceneCubicMap = LoadTextureFromImage(sceneImageMap);

    // Get map image data to be used for collision detection
    Scene_PlaceBlocks(sceneCubicMap, LoadImageColors(sceneImageMap));

    // Unload image from RAM
    UnloadImage(sceneImageMap);
}

// TODO: Entities can be moved here! Map file can be one png.
//       Move everything in this method, and change it's name
//  should first calculate the needed size of Level_items, then redo it and add the Level_items =
//  more memory efficient
void Scene_PlaceBlocks(Texture2D sceneCubicMap, Color* sceneMapPixels)
{
    // Place all Level_items based on their colors

    const float mapPosZ            = (float)sceneCubicMap.height;
    const float mapPosX            = (float)sceneCubicMap.width;
    const Texture2D ceilingTexture = LoadTexture("./assets/level1/ceiling.png");
    const Texture2D floorTexture   = LoadTexture("./assets/level1/floor.png");
    Scene_data.ceilingPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
    Scene_data.floorPlane   = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

    Scene_data.ceilingPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ceilingTexture;
    Scene_data.floorPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture   = floorTexture;

    // NOTE: By default each cube is mapped to one part of texture atlas
    // Load map texture, hardcoded for now.
    const char* wallTextures[2] = { "./assets/level1/wall1.png", "./assets/level1/wall2.png" };

    Scene_data.position = (Vector3) { -mapPosX / 2, 0.5f, -mapPosZ / 2 };
    Scene_data.size     = sceneCubicMap.height * sceneCubicMap.width;

    Scene_data.blocks      = calloc(Scene_data.size, sizeof(Scene_BlockData));
    Scene_data.actors      = calloc(Scene_data.size, sizeof(Actor_Data));
    Scene_data.projectiles = calloc(MAX_PROJECTILE_AMOUNT, sizeof(Projectile));

    for(int y = 0; y < sceneCubicMap.height; y++)
    {
        for(int x = 0; x < sceneCubicMap.width; x++)
        {

            const float mx = Scene_data.position.x - 0.5f + x * 1.0f;
            const float my = Scene_data.position.z - 0.5f + y * 1.0f;
            const int i    = y * sceneCubicMap.width + x;

            const Color pixelColor =
                Utilities_GetLevelPixelColor(sceneMapPixels, x, sceneCubicMap.width, y);

            // Find walls, which is white (255,255,255)
            if(Utilities_CompareColors(pixelColor, Level_BlockTypes.wallColor))
            {

                const Texture2D texture = LoadTexture(wallTextures[GetRandomValue(0, 1)]);
                // Set map diffuse texture
                const Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
                Model cubeModel = LoadModelFromMesh(cube);
                cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

                Scene_data.blocks[i].model       = cubeModel;
                Scene_data.blocks[i].position    = (Vector3) { mx, Scene_data.position.y, my };
                Scene_data.blocks[i].id          = WALL_MODEL_ID;
                Scene_data.blocks[i].size        = (Vector3) { 1.0f, 1.0f, 1.0f };
                Scene_data.blocks[i].boundingBox = Utilities_MakeBoundingBox(
                    (Vector3) { mx, Scene_data.position.y, my }, (Vector3) { 1.0f, 1.0f, 1.0f });
            }

            // Find start, which is green (0,255,0)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.startColor))
            {
                Scene_data.startPosition = (Vector3) { mx, 0.0f, my };
            }

            // Find end, which is blue (0,0,255)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.endColor))
            {
                Scene_data.endPosition = (Vector3) { mx, 0.0f, my };
            }

            // Find actor, which is red (255,0,0)
            else if(Utilities_CompareColors(pixelColor, Level_BlockTypes.actorColor))
            {
                Scene_data.actors[i] = Actor_Add(mx, my, i, "./assets/models/enemy.m3d");
            }

            // TODO: More entities. For entities and their RGB values: check README.md
        }
    }

    printf("Level has total %d blocks \n", Scene_data.size);
}

void Scene_Update(void)
{
    if(!Game_isStarted)
    {
        return;
    }

    DrawModel(Scene_data.floorPlane,
              (Vector3) { Scene_data.position.x, 0.0f, Scene_data.position.z },
              1.0f,
              WHITE);
    DrawModelEx(Scene_data.ceilingPlane,
                (Vector3) { Scene_data.position.x, 1.0f, -Scene_data.position.z },
                (Vector3) { -1.0f, 0.0f, 0.0f },
                180.0f,
                (Vector3) { 1.0f, 1.0f, 1.0f },
                WHITE);

    for(int i = 0; i < Scene_data.size; i++)
    {
        Scene_BlockData* data = &Scene_data.blocks[i];
        Actor_Data* actor     = &Scene_data.actors[i];
        if(data != NULL && data->id != 0)
        {
            DrawModel(data->model, data->position, 1.0f, WHITE);
        }

        if(actor != NULL && actor->id != 0)
        {
            // if Level_enemies[i] has nothing dont do anything
            Actor_Update(actor);
        }
    }
    Scene_UpdateProjectiles();
}

void Scene_UpdateProjectiles(void)
{
    for(int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        Projectile* projectile = &Scene_data.projectiles[i];
        if(projectile != NULL)
        {
            Projectile_Update(projectile);
        }
    }
}

bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId)
{
    const BoundingBox entityBox = Utilities_MakeBoundingBox(entityPos, entitySize);

    for(int i = 0; i < Scene_data.size; i++)
    {
        // Level blocks

        // Player and walls/enemies
        if(CheckCollisionBoxes(entityBox, Scene_data.blocks[i].boundingBox))
        {
            return true;
        }
        // Actor and wall/other enemies
        // Actors ignore themselves so they dont collide to themselve. Actors also ignore their
        // own projectiles
        else if(CheckCollisionBoxes(entityBox, Scene_data.actors[i].boundingBox) &&
                Scene_data.actors[i].id != entityId)
        {
            return true;
        }
    }
    return false;
}

Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize)
{
    // X width, Z height
    Mesh mesh = { 0 };
    Scene_AllocateMeshData(&mesh, 2);
    // clang-format off
    float vertices[] = {
        0,     0, 0,
        width, 0, height,
        width, 0, 0,
        0,     0, 0,
        0,     0, height,
        width, 0, height
    };

    float normals[] = {
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0
    };

    float texcoords[] = {
        0, 0,
        width / textureSize, height / textureSize,
        width / textureSize, 0,
        0, 0,
        0, height / textureSize,
        width / textureSize, height / textureSize
    };
    // clang-format on

    mesh.vertices  = vertices;
    mesh.normals   = normals;
    mesh.texcoords = texcoords;

    UploadMesh(&mesh, false);

    return mesh;
}

void Scene_AllocateMeshData(Mesh* mesh, int triangleCount)
{
    mesh->vertexCount   = triangleCount * 3;
    mesh->triangleCount = triangleCount;

    mesh->vertices  = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->texcoords = (float*)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
    mesh->normals   = (float*)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
}

void Scene_SetBlockTypes(void)
{
    Level_BlockTypes.startColor = (Color) { 0, 255, 0 };
    Level_BlockTypes.endColor   = (Color) { 0, 0, 255 };
    Level_BlockTypes.wallColor  = (Color) { 255, 255, 255 };
    Level_BlockTypes.actorColor = (Color) { 255, 0, 0 };
    Level_BlockTypes.NONE       = (Color) { 0, 0, 0 };
}