#include"../../include/raylib.h"


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