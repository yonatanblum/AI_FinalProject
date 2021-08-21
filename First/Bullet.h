#pragma once
#include "Definitions.h"
const double SPEED = 0.001;

//test by yonatan

class Bullet
{
private:
	double x, y;
	int bulletRow, bulletCol;	/////
	double dir_angle;
	bool isFired;
public:
	Bullet(double x, double y);
	Bullet(double x, double y, double angle);
	
	double getX() { return x; }
	double getY() { return y; }
	int getRow() { return bulletRow; }
	int getCol() { return bulletCol; }
	double getAngle() { return dir_angle; }
	void setX(double newX) { x=newX; }
	void setY(double newY) { y = newY; }
	void setAngle(double angle) { dir_angle = angle; }
	
	void Fire();
	void Move(int maze[MSZ][MSZ]);
	void ShowMe();
	bool getIsFired() { return isFired; }
	void SimulateFire(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double hurt);
};

