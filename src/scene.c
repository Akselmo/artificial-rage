#include "scene.h"
// Level has level data, Level_enemies, Level_items and Level_Projectiles
// Level is basically the "scene"

// Public variables
// TODO: Initialize scene properly
Scene *scene = NULL;

// Private functions
void Scene_PlaceEntities(void);
void Scene_AllocateMeshData(Mesh *mesh, int triangleCount);
void Scene_UpdateProjectiles(void);
void Scene_AddEntityToScene(Entity *entity, float mx, float my, int id);
void Scene_LoadSceneConfig(void);
bool Scene_ParseConfig(char *key, char *value);

Camera Scene_Initialize(void)
{
	Scene_Build();
	return Player_InitializeCamera(scene->startPosition.x, scene->startPosition.z);
}

// TODO: Add integer so you can select which level to load
//	   Load textures from file, instead of being built into EXE
//
void Scene_Build(void)
{
	// Create scene
	scene = calloc(1, sizeof(Scene));
	// TODO: Free all scene data

	// Initialize entity types
	Entity_InitList();

	// Load scene data from the config file
	Scene_LoadSceneConfig();

	// Get map image data to be used for collision detection
	Scene_PlaceEntities();
}

void Scene_PlaceEntities(void)
{

	const float mapPosZ = (float)scene->height;
	const float mapPosX = (float)scene->width;

	// Prepare plane textures for level
	Scene_LoadSceneConfig();

	scene->ceilingPlane = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));
	scene->floorPlane   = LoadModelFromMesh(Scene_MakeCustomPlaneMesh(mapPosZ, mapPosX, 1.0f));

	scene->ceilingPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = scene->ceilingPlaneTexture;
	scene->floorPlane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture   = scene->floorPlaneTexture;

	scene->position = (Vector3){ -mapPosX / 2, 0.5f, -mapPosZ / 2 };
	scene->size     = scene->height * scene->width;

	scene->entities    = calloc(scene->size, sizeof(Entity));
	scene->actors      = calloc(scene->size, sizeof(Actor));
	scene->projectiles = calloc(MAX_PROJECTILE_AMOUNT, sizeof(Projectile));

	for (int entity = 0; entity < scene->dataCount; entity++)
	{

		int entityPosX = entity % scene->width;
		int entityPosY = entity / scene->width;
		const float mx = scene->position.x - 0.5f + entityPosX * 1.0f;
		const float my = scene->position.z - 0.5f + entityPosY * 1.0f;

		Scene_AddEntityToScene(Entity_list[scene->data[entity]], mx, my, entity);
	}

	printf("Level has total %d entities \n", scene->size);
}

void Scene_Update(void)
{
	if (!Game_isStarted)
	{
		return;
	}

	DrawModel(scene->floorPlane, (Vector3){ scene->position.x, 0.0f, scene->position.z }, 1.0f, WHITE);
	DrawModelEx(
		scene->ceilingPlane,
		(Vector3){ scene->position.x, 1.0f, -scene->position.z },
		(Vector3){ -1.0f, 0.0f, 0.0f },
		180.0f,
		(Vector3){ 1.0f, 1.0f, 1.0f },
		WHITE
	);

	for (int i = 0; i < scene->size; i++)
	{
		Entity *data = &scene->entities[i];
		Actor *actor = &scene->actors[i];
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
		Projectile *projectile = &scene->projectiles[i];
		if (projectile != NULL)
		{
			Projectile_Update(projectile);
		}
	}
}

bool Scene_CheckCollision(Vector3 entityPos, Vector3 entitySize, int entityId)
{
	const BoundingBox entityBox = Utilities_MakeBoundingBox(entityPos, entitySize);

	for (int i = 0; i < scene->size; i++)
	{
		// Level entities

		// Player and walls/enemies
		if (CheckCollisionBoxes(entityBox, scene->entities[i].boundingBox))
		{
			return true;
		}
		// Actor and wall/other enemies
		// Actors ignore themselves so they dont collide to themselve. Actors also ignore their
		// own projectiles
		else if (CheckCollisionBoxes(entityBox, scene->actors[i].boundingBox) && scene->actors[i].id != entityId)
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

	float vertices[] = { 0, 0, 0, width, 0, height, width, 0, 0, 0, 0, 0, 0, 0, height, width, 0, height };

	float normals[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };

	float texcoords[] = { 0, 0, width / textureSize,  height / textureSize, width / textureSize, 0, 0,
		                  0, 0, height / textureSize, width / textureSize,  height / textureSize };

	mesh.vertices  = vertices;
	mesh.normals   = normals;
	mesh.texcoords = texcoords;

	UploadMesh(&mesh, false);

	return mesh;
}

void Scene_AllocateMeshData(Mesh *mesh, int triangleCount)
{
	mesh->vertexCount   = triangleCount * 3;
	mesh->triangleCount = triangleCount;

	mesh->vertices  = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
	mesh->texcoords = (float *)MemAlloc(mesh->vertexCount * 2 * sizeof(float));
	mesh->normals   = (float *)MemAlloc(mesh->vertexCount * 3 * sizeof(float));
}

void Scene_AddEntityToScene(Entity *entity, float mx, float my, int id)
{
	if (entity->type == SCENE_NONE)
	{
		return;
	}
	if (entity->type == SCENE_WALL)
	{
		Image textureImage = LoadImage(entity->fileName);
		// The image has to be flipped since its loaded upside down
		ImageFlipVertical(&textureImage);
		const Texture2D texture = LoadTextureFromImage(textureImage);
		// Set map diffuse texture
		const Mesh cube                                           = GenMeshCube(1.0f, 1.0f, 1.0f);
		Model cubeModel                                           = LoadModelFromMesh(cube);
		cubeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

		scene->entities[id].model    = cubeModel;
		scene->entities[id].position = (Vector3){ mx, scene->position.y, my };
		scene->entities[id].id       = WALL_MODEL_ID;
		scene->entities[id].size     = (Vector3){ 1.0f, 1.0f, 1.0f };
		scene->entities[id].boundingBox =
			Utilities_MakeBoundingBox((Vector3){ mx, scene->position.y, my }, (Vector3){ 1.0f, 1.0f, 1.0f });
	}

	else if (entity->type == SCENE_START)
	{
		scene->startPosition = (Vector3){ mx, 0.0f, my };
	}

	else if (entity->type == SCENE_END)
	{
		scene->endPosition = (Vector3){ mx, 0.0f, my };
	}

	else if (entity->type == SCENE_ACTOR)
	{
		scene->actors[id] = Actor_Add(mx, my, id, entity->fileName);
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
	char *texturesPath    = "./assets/textures/";
	char *fullTexturePath = malloc(strlen(texturesPath) + strlen(value) + 1);
	strcpy(fullTexturePath, texturesPath);
	strcat(fullTexturePath, value);

	// NOTE: remove this snippet when done: else if (strcmp(key, "") == 0) { return true;}

	if (strcmp(key, "ceilingtexture") == 0)
	{
		scene->ceilingPlaneTexture = LoadTexture(fullTexturePath);
		free(fullTexturePath);
		return true;
	}
	else if (strcmp(key, "floortexture") == 0)
	{
		scene->floorPlaneTexture = LoadTexture(fullTexturePath);
		free(fullTexturePath);
		return true;
	}
	else if (strcmp(key, "name") == 0)
	{
		scene->name = calloc(strlen(value) + 5, sizeof(char));
		strcpy(scene->name, value);
		return true;
	}
	else if (strcmp(key, "height") == 0)
	{
		scene->height = atoi(value);
		return true;
	}
	else if (strcmp(key, "width") == 0)
	{
		scene->width = atoi(value);
		return true;
	}
	else if (strcmp(key, "data") == 0)
	{
		scene->data = Utilities_ParseIntArray(value, &scene->dataCount);
		return true; // Handle array after the parsing is done
	}
	else
	{
		printf("No such key: %s \n", key);
		return false;
	}
}
