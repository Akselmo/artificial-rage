#include "scene.h"
#include "raylib.h"
// Level has level data, Level_enemies, Level_items and Level_Projectiles
// Level is basically the "scene"

// Public variables
// TODO: Initialize scene properly
Scene_Data *Scene = NULL;

// Private variables
Entity_Data *Scene_entities[ENTITIES_TOTAL];

// Private functions
void Scene_PlaceEntities(void);
void Scene_AllocateMeshData(Mesh *mesh, int triangleCount);
void Scene_InitEntityTypes(void);
void Scene_UpdateProjectiles(void);
void Scene_AddEntityToScene(Entity_Data *entity, float mx, float my, int id);
void Scene_LoadSceneConfig(void);
bool Scene_ParseConfig(char *key, char *value);

Camera Scene_Initialize(void)
{
	Scene_Build();
	return Player_InitializeCamera(Scene->startPosition.x, Scene->startPosition.z);
}

// TODO: Add integer so you can select which level to load
//       Load textures from file, instead of being built into EXE
//
void Scene_Build(void)
{
	// Create scene
	Scene = calloc(1, sizeof(Scene_Data));
	// TODO: Free all scene data

	// Initialize entity types
	Scene_InitEntityTypes();

	// Load scene data from the config file
	Scene_LoadSceneConfig();

	printf("Scene data 3: %d\n", Scene->data[2]);
	// Get map image data to be used for collision detection
	// Scene_PlaceEntities();
}
/*
 * FIXME: Do this part next
void Scene_PlaceEntities(void)
{

	const float mapPosZ = (float)sceneCubicMap.height;
	const float mapPosX = (float)sceneCubicMap.width;

	// Prepare plane textures for level
	Scene_LoadSceneConfig();

	Scene->ceilingPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
	Scene->floorPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

	Scene->ceilingPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = Scene->ceilingPlaneTexture;
	Scene->floorPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = Scene->floorPlaneTexture;

	Scene->position = (Vector3){-mapPosX / 2, 0.5f, -mapPosZ / 2};
	Scene->size = sceneCubicMap.height * sceneCubicMap.width;

	Scene->entities = calloc(Scene->size, sizeof(Entity_Data));
	Scene->actors = calloc(Scene->size, sizeof(Actor_Data));
	Scene->projectiles = calloc(MAX_PROJECTILE_AMOUNT, sizeof(Projectile));

	for (int y = 0; y < sceneCubicMap.height; y++)
	{
		for (int x = 0; x < sceneCubicMap.width; x++)
		{

			const float mx = Scene->position.x - 0.5f + x * 1.0f;
			const float my = Scene->position.z - 0.5f + y * 1.0f;
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

	printf("Level has total %d entities \n", Scene->size);
}
*/
void Scene_Update(void)
{
	if (!Game_isStarted)
	{
		return;
	}

	DrawModel(Scene->floorPlane, (Vector3){Scene->position.x, 0.0f, Scene->position.z}, 1.0f, WHITE);
	DrawModelEx(Scene->ceilingPlane,
				(Vector3){Scene->position.x, 1.0f, -Scene->position.z},
				(Vector3){-1.0f, 0.0f, 0.0f},
				180.0f,
				(Vector3){1.0f, 1.0f, 1.0f},
				WHITE);

	for (int i = 0; i < Scene->size; i++)
	{
		Entity_Data *data = &Scene->entities[i];
		Actor_Data *actor = &Scene->actors[i];
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
		Projectile *projectile = &Scene->projectiles[i];
		if (projectile != NULL)
		{
			Projectile_Update(projectile);
		}
	}
}

bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId)
{
	const BoundingBox entityBox = Utilities_MakeBoundingBox(entityPos, entitySize);

	for (int i = 0; i < Scene->size; i++)
	{
		// Level entities

		// Player and walls/enemies
		if (CheckCollisionBoxes(entityBox, Scene->entities[i].boundingBox))
		{
			return true;
		}
		// Actor and wall/other enemies
		// Actors ignore themselves so they dont collide to themselve. Actors also ignore their
		// own projectiles
		else if (CheckCollisionBoxes(entityBox, Scene->actors[i].boundingBox) && Scene->actors[i].id != entityId)
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

	float vertices[] = {
		0, 0, 0,
		width, 0, height,
		width, 0, 0,
		0, 0, 0,
		0, 0, height,
		width, 0, height};

	float normals[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0};

	float texcoords[] = {
		0, 0,
		width / textureSize, height / textureSize,
		width / textureSize, 0,
		0, 0,
		0, height / textureSize,
		width / textureSize, height / textureSize};

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

void Scene_InitEntityTypes(void)
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

		Scene->entities[id].model = cubeModel;
		Scene->entities[id].position = (Vector3){mx, Scene->position.y, my};
		Scene->entities[id].id = WALL_MODEL_ID;
		Scene->entities[id].size = (Vector3){1.0f, 1.0f, 1.0f};
		Scene->entities[id].boundingBox = Utilities_MakeBoundingBox((Vector3){mx, Scene->position.y, my}, (Vector3){1.0f, 1.0f, 1.0f});
	}

	else if (entity->type == SCENE_START)
	{
		Scene->startPosition = (Vector3){mx, 0.0f, my};
	}

	else if (entity->type == SCENE_END)
	{
		Scene->endPosition = (Vector3){mx, 0.0f, my};
	}

	else if (entity->type == SCENE_ACTOR)
	{
		Scene->actors[id] = Actor_Add(mx, my, id, entity->fileName);
	}
}

// Parse scene plane textures from level.cfg
void Scene_LoadSceneConfig(void)
{
	const char *fileName = "./assets/levels/level1.cfg";
	const int bufferSize = Utilities_GetFileCharacterCount(fileName);

	FILE *filePointer = fopen(fileName, "r");
	if (NULL == filePointer)
	{
		printf("Failed to open level config file %s \n", fileName);
		return;
	}

	char buffer[bufferSize];
	while (fgets(buffer, bufferSize, filePointer))
	{
		char key[bufferSize];
		char value[bufferSize];
		Utilities_ParseKeyValuePair(buffer, key, "=", value);

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
	// TODO: somekind of utility for getting full asset path here
	char *texturesPath = "./assets/textures/";
	char *fullTexturePath = malloc(strlen(texturesPath) + strlen(value) + 1);
	strcpy(fullTexturePath, texturesPath);
	strcat(fullTexturePath, value);

	// NOTE: remove this snippet when done: else if (strcmp(key, "") == 0) { return true;}

	if (strcmp(key, "ceilingtexture") == 0)
	{
		Scene->ceilingPlaneTexture = LoadTexture(fullTexturePath);
		free(fullTexturePath);
		return true;
	}
	else if (strcmp(key, "floortexture") == 0)
	{
		Scene->floorPlaneTexture = LoadTexture(fullTexturePath);
		free(fullTexturePath);
		return true;
	}
	else if (strcmp(key, "name") == 0)
	{
		Scene->name = calloc(strlen(value) + 5, sizeof(char));
		strcpy(Scene->name, value);
		return true;
	}
	else if (strcmp(key, "height") == 0)
	{
		Scene->height = atoi(value);
		return true;
	}
	else if (strcmp(key, "width") == 0)
	{
		Scene->width = atoi(value);
		return true;
	}
	else if (strcmp(key, "data") == 0)
	{
		Scene->data = Utilities_ParseIntArray(value, &Scene->dataCount);
		return true; // Handle array after the parsing is done
	}
	else
	{
		printf("No such key: %s \n", key);
		return false;
	}
}
