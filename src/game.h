#pragma once
#include "hud.h"
#include <raylib-cpp.hpp>

class Game
{
  public:
	Game();
	void update(void);
	static raylib::Camera camera;
	static bool isStarted;
	Hud hud;

  private:
	void hudUpdate(void);
	void menuUpdate(void);
};
