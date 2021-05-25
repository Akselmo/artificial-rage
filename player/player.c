#include"../include/raylib.h"

#define CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER  0.0f
#define CAMERA_FIRST_PERSON_STEP_DIVIDER                00.0f
#define CAMERA_FIRST_PERSON_WAVING_DIVIDER              000.0f

//TODO: override headbob

Camera PlayerCamera(float fov)
{
    Camera camera = { 0 };
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = fov; //get fov from settings file
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}