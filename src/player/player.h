#include"../../include/raylib.h"


Camera CustomFPSCamera(float fov, float pos_x, float pos_z, int moveFront, int moveBack, int moveRight, int moveLeft, float sensitivity);
void UpdateFPSCamera(Camera *camera);