#pragma once
#include "Definitions.h"
const double SPEED = 0.001;

//test by yonatan

class Bullet
{
private:
	double x, y;
	double dir_angle;
	bool isFired;
public:
	Bullet(double x, double y);
	Bullet(double x, double y, double angle);
	void Fire();
	void Move(int maze[MSZ][MSZ]);
	void ShowMe();
	bool getIsFired() { return isFired; }
	void SimulateFire(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double hurt);
};

