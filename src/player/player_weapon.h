#ifndef PLAYER_WEAPON
#define PLAYER_WEAPON

#include "../../include/raylib.h"

void InitializeWeaponKeys();
void ChangeWeapon();
void SelectDefaultWeapon();
float FireWeapon(Vector3 playerPosition, Vector3 direction, float nextFire);
float TestLevelHit(Ray rayCast);
float TestEntityHit(Ray rayCast, int entityAmount);

#endif
