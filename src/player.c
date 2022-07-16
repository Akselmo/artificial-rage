#include "player.h"
#include "hud.h"
#include "weapon.h"

// Took some parts of raylib camera.h and made my own camera based on that for full control

// Initialize Custom Camera
static Player_CustomCameraData Player_CustomCamera = {
    .targetDistance     = 0,
    .playerEyesPosition = 1.85f,
    .angle              = {0},
    .mouseSensitivity   = 0.003f,
    .playerSpeed        = 1.75f,
};

// Initialize player
Player_Data Player = {
    .health = PLAYER_MAX_HEALTH,
    .dead   = false,
};

// Private functions
Vector3 Player_size;
Vector3 Player_position;
BoundingBox Player_boundingBox;
float Player_nextFire = 0.0f;

Camera Player_InitializeCamera(float pos_x, float pos_z)
{
    Camera camera = {0};

    // Get Settings
    float fov              = Settings_CameraFov;
    float mouseSensitivity = Settings_MouseSensitivity;
    int moveFront          = Settings_GetCustomInput(KEY_W);
    int moveBack           = Settings_GetCustomInput(KEY_S);
    int moveRight          = Settings_GetCustomInput(KEY_D);
    int moveLeft           = Settings_GetCustomInput(KEY_A);
    int fireGun            = MOUSE_LEFT_BUTTON;

    // Place camera and apply settings
    camera.position   = (Vector3) {pos_x, PLAYER_START_POSITION_Y, pos_z};
    camera.target     = (Vector3) {0.0f, 0.5f, 0.0f};
    camera.up         = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy       = fov;  // get fov from settings file
    camera.projection = CAMERA_PERSPECTIVE;

    // Distances
    Vector3 v1 = camera.position;
    Vector3 v2 = camera.target;
    float dx   = v2.x - v1.x;
    float dy   = v2.y - v1.y;
    float dz   = v2.z - v1.z;

    // Distance to target
    Player_CustomCamera.targetDistance = sqrtf(dx * dx + dy * dy + dz * dz);

    // Camera angle calculation
    // Camera angle in plane XZ (0 aligned with Z, move positive CCW)
    Player_CustomCamera.angle.x = atan2f(dx, dz);
    // Camera angle in plane XY (0 aligned with X, move positive CW)
    Player_CustomCamera.angle.y = atan2f(dy, sqrtf(dx * dx + dz * dz));

    // Init player eyes position to camera Y position
    Player_CustomCamera.playerEyesPosition = camera.position.y;

    // Setup custom movement keys
    Player_CustomCamera.moveFront        = moveFront;
    Player_CustomCamera.moveBack         = moveBack;
    Player_CustomCamera.moveRight        = moveRight;
    Player_CustomCamera.moveLeft         = moveLeft;
    Player_CustomCamera.mouseSensitivity = mouseSensitivity;

    Weapon_InitializeKeys();

    // Set player size for bounding box
    Player_size = (Vector3) {0.1f, 0.1f, 0.1f};
    Weapon_SelectDefault();

    return camera;
}

void Player_Update(Camera* camera)
{

    Vector3 oldPlayerPos = camera->position;

    Player_boundingBox = Utilities_MakeBoundingBox(camera->position, Player_size);

    static Vector2 previousMousePosition = {0.0f, 0.0f};
    Vector2 mousePositionDelta           = {0.0f, 0.0f};
    Vector2 mousePosition                = GetMousePosition();
    float mouseWheelMove                 = GetMouseWheelMove();

    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;

    previousMousePosition = mousePosition;

    bool direction[4] = {IsKeyDown(Player_CustomCamera.moveFront),
                         IsKeyDown(Player_CustomCamera.moveBack),
                         IsKeyDown(Player_CustomCamera.moveRight),
                         IsKeyDown(Player_CustomCamera.moveLeft)};

    // Move camera around X pos
    camera->position.x += ((sinf(Player_CustomCamera.angle.x) * direction[MOVE_BACK] -
                            sinf(Player_CustomCamera.angle.x) * direction[MOVE_FRONT] -
                            cosf(Player_CustomCamera.angle.x) * direction[MOVE_LEFT] +
                            cosf(Player_CustomCamera.angle.x) * direction[MOVE_RIGHT]) *
                           Player_CustomCamera.playerSpeed) *
                          GetFrameTime();

    // Move camera around Y pos
    camera->position.y += ((sinf(Player_CustomCamera.angle.y) * direction[MOVE_FRONT] -
                            sinf(Player_CustomCamera.angle.y) * direction[MOVE_BACK]) *
                           Player_CustomCamera.playerSpeed) *
                          GetFrameTime();

    // Move camera around Z pos
    camera->position.z += ((cosf(Player_CustomCamera.angle.x) * direction[MOVE_BACK] -
                            cosf(Player_CustomCamera.angle.x) * direction[MOVE_FRONT] +
                            sinf(Player_CustomCamera.angle.x) * direction[MOVE_LEFT] -
                            sinf(Player_CustomCamera.angle.x) * direction[MOVE_RIGHT]) *
                           Player_CustomCamera.playerSpeed) *
                          GetFrameTime();
    // Camera orientation calculation
    Player_CustomCamera.angle.x += (mousePositionDelta.x * -Player_CustomCamera.mouseSensitivity);
    Player_CustomCamera.angle.y += (mousePositionDelta.y * -Player_CustomCamera.mouseSensitivity);

    // Angle clamp
    if(Player_CustomCamera.angle.y > PLAYER_CAMERA_MIN_CLAMP * DEG2RAD)
    {
        Player_CustomCamera.angle.y = PLAYER_CAMERA_MIN_CLAMP * DEG2RAD;
    }
    else if(Player_CustomCamera.angle.y < PLAYER_CAMERA_MAX_CLAMP * DEG2RAD)
    {
        Player_CustomCamera.angle.y = PLAYER_CAMERA_MAX_CLAMP * DEG2RAD;
    }

    // Recalculate camera target considering translation and rotation
    Matrix translation =
        MatrixTranslate(0, 0, (Player_CustomCamera.targetDistance / PLAYER_CAMERA_PANNING_DIVIDER));
    Matrix rotation = MatrixRotateXYZ(
        (Vector3) {PI * 2 - Player_CustomCamera.angle.y, PI * 2 - Player_CustomCamera.angle.x, 0});
    Matrix transform = MatrixMultiply(translation, rotation);

    // Move camera according to matrix position (where camera looks at)
    camera->target.x = camera->position.x - transform.m12;
    camera->target.y = camera->position.y - transform.m13;
    camera->target.z = camera->position.z - transform.m14;

    // Camera position update
    camera->position.y = Player_CustomCamera.playerEyesPosition;

    if(Level_CheckCollision(camera->position, Player_size, PLAYER_ID))
    {
        camera->position = oldPlayerPos;
    }

    Player_position = camera->position;

    // Check if we need to switch weapon
    Weapon_GetSwitchInput();
}

BoundingBox GetPlayerBoundingBox()
{
    return Player_boundingBox;
}

Vector3 GetPlayerPosition()
{
    return Player_position;
}

// Use minus for removing health
void Player_SetHealth(int healthToAdd)
{
    Player.health += healthToAdd;
    if(Player.health > PLAYER_MAX_HEALTH)
    {
        Player.health = PLAYER_MAX_HEALTH;
    }
    else if(Player.health <= 0)
    {
        Player.dead = true;
    }
}

void Player_FireWeapon(Camera* camera)
{
    // Calculate fire rate countdown here
    Player_nextFire -= GetFrameTime();
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Player_nextFire = Weapon_Fire(camera, Player_nextFire);
    }
}
