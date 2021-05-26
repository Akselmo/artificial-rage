#include"../../include/raylib.h"
#include"../../include/raymath.h"
#include <math.h>  

//Took some parts of raylib camera.h and made my own camera based on that for full control

#define CAMERA_MIN_CLAMP 89.0f
#define CAMERA_MAX_CLAMP -89.0f
#define CAMERA_PANNING_DIVIDER  5.1f

//Struct for all the camera data
typedef struct {
    float targetDistance;
    float playerEyesPosition;       
    Vector2 angle;                 
    int moveFront, moveBack, moveRight, moveLeft;
    float mouseSensitivity;
    float playerSpeed;       
} CameraData;

//Movement keys enum for directions
typedef enum {
    MOVE_FRONT = 0,
    MOVE_BACK = 1,
    MOVE_RIGHT = 2,
    MOVE_LEFT = 3
} CameraMove;

static CameraData CAMERA = {
    .targetDistance = 0,
    .playerEyesPosition = 1.85f,
    .angle = { 0 },
    .mouseSensitivity = 0.003f,
    .playerSpeed = 30.0f
};


Camera CustomFPSCamera(float fov, float pos_x, float pos_z, int moveFront, int moveBack, int moveRight, int moveLeft, float sensitivity)
{
    Camera camera = { 0 };
    
    camera.position = (Vector3){ pos_x, 2.5f, pos_z };
    camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = fov; //get fov from settings file
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 v1 = camera.position;
    Vector3 v2 = camera.target;

    //Distances
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;

    // Distance to target
    CAMERA.targetDistance = sqrtf(dx*dx + dy*dy + dz*dz);   

    // Camera angle calculation
    // Camera angle in plane XZ (0 aligned with Z, move positive CCW)
    CAMERA.angle.x = atan2f(dx, dz); 
    // Camera angle in plane XY (0 aligned with X, move positive CW)                       
    CAMERA.angle.y = atan2f(dy, sqrtf(dx*dx + dz*dz));      

    // Init player eyes position to camera Y position
    CAMERA.playerEyesPosition = camera.position.y;   

    //Setup custom movement keys
    CAMERA.moveFront = moveFront;
    CAMERA.moveBack = moveBack;
    CAMERA.moveRight = moveRight;
    CAMERA.moveLeft = moveLeft;
    CAMERA.mouseSensitivity = sensitivity;

    DisableCursor();

    return camera;
}



void UpdateFPSCamera(Camera *camera)
{
    static Vector2 previousMousePosition = { 0.0f, 0.0f };

    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();

    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;

    previousMousePosition = mousePosition;


    bool direction[4] = 
    {
        IsKeyDown(CAMERA.moveFront),
        IsKeyDown(CAMERA.moveBack),
        IsKeyDown(CAMERA.moveRight),
        IsKeyDown(CAMERA.moveLeft)
    };

    //Move camera around X pos
    camera->position.x += 
    (
        sinf(CAMERA.angle.x)*direction[MOVE_BACK] -
        sinf(CAMERA.angle.x)*direction[MOVE_FRONT] -
        cosf(CAMERA.angle.x)*direction[MOVE_LEFT] +
        cosf(CAMERA.angle.x)*direction[MOVE_RIGHT]
    )/CAMERA.playerSpeed;

    //Move camera around Y pos
    camera->position.y += 
    (
        sinf(CAMERA.angle.y)*direction[MOVE_FRONT] -
        sinf(CAMERA.angle.y)*direction[MOVE_BACK]
    )/CAMERA.playerSpeed;

    //Move camera around Z pos
    camera->position.z += 
    (
        cosf(CAMERA.angle.x)*direction[MOVE_BACK] -
        cosf(CAMERA.angle.x)*direction[MOVE_FRONT] +
        sinf(CAMERA.angle.x)*direction[MOVE_LEFT] -
        sinf(CAMERA.angle.x)*direction[MOVE_RIGHT]
    )/CAMERA.playerSpeed;

    // Camera orientation calculation
    CAMERA.angle.x += (mousePositionDelta.x*-CAMERA.mouseSensitivity);
    CAMERA.angle.y += (mousePositionDelta.y*-CAMERA.mouseSensitivity);

    // Angle clamp
    if (CAMERA.angle.y > CAMERA_MIN_CLAMP*DEG2RAD)
    {
        CAMERA.angle.y = CAMERA_MIN_CLAMP*DEG2RAD;
    } 
    else if (CAMERA.angle.y < CAMERA_MAX_CLAMP*DEG2RAD) 
    {
        CAMERA.angle.y = CAMERA_MAX_CLAMP*DEG2RAD;
    }

    // Recalculate camera target considering translation and rotation
    Matrix translation = MatrixTranslate(0, 0, (CAMERA.targetDistance/CAMERA_PANNING_DIVIDER));
    Matrix rotation = MatrixRotateXYZ((Vector3){ PI*2 - CAMERA.angle.y, PI*2 - CAMERA.angle.x, 0 });
    Matrix transform = MatrixMultiply(translation, rotation);

    //Move camera according to matrix position (where camera looks at)
    camera->target.x = camera->position.x - transform.m12;
    camera->target.y = camera->position.y - transform.m13;
    camera->target.z = camera->position.z - transform.m14;

    // Camera position update
    camera->position.y = CAMERA.playerEyesPosition;

    
}