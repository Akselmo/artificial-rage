#pragma once

class Hud
{
  public:
	void draw();
	Hud();

  private:
	void healthText(char *text);
	void weaponText(char *text);
};
