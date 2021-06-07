#pragma once
#include "Bullet.h"

const int NUM_BULLETS = 20;

class Granade
{
private:
	Bullet* bullets[NUM_BULLETS];
	double x, y;
	bool isExploded;

public:
	Granade(double x, double y);

	void Explode();
	void Exploding(int maze[MSZ][MSZ]);
	void SimulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double hurt);
	void Show();
	bool getIsExploded() { return isExploded; }

};

