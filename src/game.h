#pragma once
#include <raylib-cpp.hpp>

class Game
{
  public:
	void initialize(void);
	void update(void);
	static raylib::Camera camera;
	static bool isStarted;

  private:
	void hudUpdate(void);
	void menuUpdate(void);
};
