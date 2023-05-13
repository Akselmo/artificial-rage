#include "scene.h"
#include "raylib.h"
// Level has level data, Level_enemies, Level_items and Level_Projectiles
// Level is basically the "scene"

// Public variables
Scene_Data Scene = {0};

// Private variables
Entity_Data *Scene_entities[ENTITIES_TOTAL]; // Remember to update this if you add more entities to below

// Private functions
void Scene_PlaceEntities(Texture2D sceneCubicMap, Color *sceneMapPixels);
void Scene_AllocateMeshData(Mesh *mesh, int triangleCount);
void Scene_SetEntityTypes(void);
void Scene_UpdateProjectiles(void);
void Scene_AddEntityToScene(Entity_Data *entity, float mx, float my, int id);
void Scene_LoadPlaneTextures(void);
bool Scene_ParseConfig(char *key, char *value);

Camera Scene_Initialize(void)
{
    Scene_Build();
    return Player_InitializeCamera(Scene.startPosition.x, Scene.startPosition.z);
}

// TODO: Add integer so you can select which level to load
//       Load textures from file, instead of being built into EXE
//
void Scene_Build(void)
{
    // Initialize entity types
    Scene_SetEntityTypes();

    // Load level cubicmap image (RAM)
    const Image sceneImageMap = LoadImage("./assets/level1/level.png");
    const Texture2D sceneCubicMap = LoadTextureFromImage(sceneImageMap);

    // Get map image data to be used for collision detection
    Scene_PlaceEntities(sceneCubicMap, LoadImageColors(sceneImageMap));

    // Unload image from RAM
    UnloadImage(sceneImageMap);
}

void Scene_PlaceEntities(Texture2D sceneCubicMap, Color *sceneMapPixels)
{
    const float mapPosZ = (float)sceneCubicMap.height;
    const float mapPosX = (float)sceneCubicMap.width;

    // Prepare plane textures for level
    Scene_LoadPlaneTextures();

    Scene.ceilingPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
    Scene.floorPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

    Scene.ceilingPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = Scene.ceilingPlaneTexture;
    Scene.floorPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = Scene.floorPlaneTexture;

    Scene.position = (Vector3){-mapPosX / 2, 0.5f, -mapPosZ / 2};
    Scene.size = sceneCubicMap.height * sceneCubicMap.width;

    Scene.entities = calloc(Scene.size, sizeof(Entity_Data));
    Scene.actors = calloc(Scene.size, sizeof(Actor_Data));
    Scene.projectiles = calloc(MAX_PROJECTILE_AMOUNT, sizeof(Projectile));

    for (int y = 0; y < sceneCubicMap.height; y++)
    {
        for (int x = 0; x < sceneCubicMap.width; x++)
        {

            const float mx = Scene.position.x - 0.5f + x * 1.0f;
            const float my = Scene.position.z - 0.5f + y * 1.0f;
            const int id = y * sceneCubicMap.width + x;

            const Color pixelColor = Utilities_GetLevelPixelColor(sceneMapPixels, x, sceneCubicMap.width, y);

            for (int b = 0; b < ENTITIES_TOTAL; b++)
            {
                if (Utilities_CompareColors(pixelColor, Scene_entities[b]->mapColor))
                {
                    Scene_AddEntityToScene(Scene_entities[b], mx, my, id);
                }
            }
        }
    }

    printf("Level has total %d entities \n", Scene.size);
}

void Scene_Update(void)
{
    if (!Game_isStarted)
    {
        return;
    }

    DrawModel(Scene.floorPlane, (Vector3){Scene.position.x, 0.0f, Scene.position.z}, 1.0f, WHITE);
    DrawModelEx(Scene.ceilingPlane,
                (Vector3){Scene.position.x, 1.0f, -Scene.position.z},
                (Vector3){-1.0f, 0.0f, 0.0f},
                180.0f,
                (Vector3){1.0f, 1.0f, 1.0f},
                WHITE);

    for (int i = 0; i < Scene.size; i++)
    {
        Entity_Data *data = &Scene.entities[i];
        Actor_Data *actor = &Scene.actors[i];
        if (data != NULL && data->id != 0)
        {
            DrawModel(data->model, data->position, 1.0f, WHITE);
        }

        if (actor != NULL && actor->id != 0)
        {
            // if Level_enemies[i] has nothing dont do anything
            Actor_Update(actor);
        }
    }
    Scene_UpdateProjectiles();
}

void Scene_UpdateProjectiles(void)
{
    for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
    {
        Projectile *projectile = &Scene.projectiles[i];
        if (projectile != NULL)
        {
            Projectile_Update(projectile);
        }
    }
}

bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId)
{
    const BoundingBox entityBox = Utilities_MakeBoundingBox(entityPos, entitySize);

    for (int i = 0; i < Scene.size; i++)
    {
        // Level entities

        // Player and walls/enemies
        if (CheckCollisionBoxes(entityBox, Scene.entities[i].boundingBox))
        {
            return true;
        }
        // Actor and wall/other enemies
        // Actors ignore themselves so they dont collide to themselve. Actors also ignore their
        // own projectiles
        else if (CheckCollisionBoxes(entityBox, Scene.actors[i].boundingBox) && Scene.actors[i].id != entityId)
        {
            return true;
        }
    }
    return false;
}

Mesh Scene_MakeCustomPlaneMesh(float height, float width, float textureSize)
{
    // X width, Z height
    Mesh mesh = {0};
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

    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.texcoords = texcoords;

    UploadMesh(&mesh, false);

    return mesh;
}

void Scene_AllocateMeshData(Mesh *mesh, int triangleCount)
{
    mesh->vertexCount = triangleCount * 3;
    mesh->triangleCount = triangleCount;

    mesh->vertices = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
    mesh->texcoords = (float *)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
    mesh->normals = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
}

void Scene_SetEntityTypes(void)
{
    Scene_entities[0] = &Entity_none;
    Scene_entities[1] = &Entity_start;
    Scene_entities[2] = &Entity_end;
    Scene_entities[3] = &Entity_wall1;
    Scene_entities[4] = &Entity_wall2;
    Scene_entities[5] = &Entity_enemy;
}

void Scene_AddEntityToScene(Entity_Data *entity, float mx, float my, int id)
{
    if (entity->type == SCENE_WALL)
    {
        Image textureImage = LoadImage(entity->fileName);
        // The image has to be flipped since its loaded upside down
        ImageFlipVertical(&textureImage);
        const Texture2D texture = LoadTextureFromImage(textureImage);
        // Set map diffuse texture
        const Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        Model cubeModel = LoadModelFromMesh(cube);
        cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

        Scene.entities[id].model = cubeModel;
        Scene.entities[id].position = (Vector3){mx, Scene.position.y, my};
        Scene.entities[id].id = WALL_MODEL_ID;
        Scene.entities[id].size = (Vector3){1.0f, 1.0f, 1.0f};
        Scene.entities[id].boundingBox = Utilities_MakeBoundingBox((Vector3){mx, Scene.position.y, my}, (Vector3){1.0f, 1.0f, 1.0f});
    }

    else if (entity->type == SCENE_START)
    {
        Scene.startPosition = (Vector3){mx, 0.0f, my};
    }

    else if (entity->type == SCENE_END)
    {
        Scene.endPosition = (Vector3){mx, 0.0f, my};
    }

    else if (entity->type == SCENE_ACTOR)
    {
        Scene.actors[id] = Actor_Add(mx, my, id, entity->fileName);
    }
}

// Parse scene plane textures from level.cfg
void Scene_LoadPlaneTextures(void)
{
    const char *fileName = "./assets/level1/level.cfg";

    FILE *filePointer = fopen(fileName, "r");
    if (NULL == filePointer)
    {
        printf("Failed to open level config file %s \n", fileName);
        printf("Please check your levelfolderhere/level.cfg file \n");
        return;
    }
    int bufferLength = 255;
    char buffer[bufferLength];

    while (fgets(buffer, bufferLength, filePointer))
    {
        char key[255];
        char value[255];
        Utilities_ParseKeyValuePair(buffer, key, value);

        if (!Scene_ParseConfig(key, value))
        {
            printf("Failed to parse level.cfg key-value: %s - %s \n", key, value);
        }
        else
        {
            printf("Parsed level.cfg key-value: %s - %s \n", key, value);
        }
    }
    fclose(filePointer);
}

bool Scene_ParseConfig(char *key, char *value)
{

    char *texturesPath = "./assets/textures/";
    char *fullPath = malloc(strlen(texturesPath) + strlen(value) + 1);
    strcpy(fullPath, texturesPath);
    strcat(fullPath, value);
    bool returnVal = false;
    if (strcmp(key, "ceiling") == 0)
    {
        Scene.ceilingPlaneTexture = LoadTexture(fullPath);
        returnVal = true;
    }
    else if (strcmp(key, "floor") == 0)
    {
        Scene.floorPlaneTexture = LoadTexture(fullPath);
        returnVal = true;
    }
    else
    {
        printf("Failed to parse scene config file!\n");
        printf("Please check your levelfolderhere/level.cfg file \n");
    }
    free(fullPath);
    return returnVal;
}
