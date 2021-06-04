#include"../../include/raylib.h"

void InitializeWeaponKeys();
void ChangeWeapon();
void FireWeapon(Vector3 playerPosition, Vector3 direction, int levelSize, int entities);
float TestLevelHit(Ray rayCast);
float TestEntityHit(Ray rayCast, int entityAmount);