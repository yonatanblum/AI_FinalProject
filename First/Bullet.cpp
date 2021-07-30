#include "Bullet.h"
#include "GLUT.H"
#include <math.h>

Bullet::Bullet(double x, double y)// 1234
{
	this->x = x;
	this->y = y;
	dir_angle = (rand() % 360)*3.14/180;// in radians
	isFired = false;
}

Bullet::Bullet(double x, double y, double angle)
{
	this->x = x;
	this->y = y;
	dir_angle = angle;// in radians
	isFired = false;
}

void Bullet::Fire()
{
	if (!isFired)
	{
		isFired = true;
	}
}

void Bullet::Move(int maze[MSZ][MSZ])
{
	if (isFired)
	{
		double dirx, diry;
		int row, column;
		dirx = cos(dir_angle);
		diry = sin(dir_angle);
		x += SPEED * dirx;
		y += SPEED * diry;
		// check if the bullet is still above SPACE
		column = (int)(MSZ * (x + 1) / 2);
		row = (int)(MSZ * (y + 1) / 2);
		if (maze[row][column] == WALL)
			isFired = false;
	}
}

void Bullet::ShowMe()
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.01, y);
	glVertex2d(x , y+ 0.01);
	glVertex2d(x + 0.01, y);
	glVertex2d(x, y - 0.01);
	glEnd();
}

void Bullet::SimulateFire(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double hurt)
{
	bool simulate = true;
	double dirx, diry;
	int row, column;

	dirx = cos(dir_angle);
	diry = sin(dir_angle);

	while (simulate)
	{
		column = (int)(MSZ * (x + 1) / 2);
		row = (int)(MSZ * (y + 1) / 2);
		if (maze[row][column] == WALL)
			simulate = false;
		else // maze[row][column] is SPACE
		{
			security_map[row][column] += hurt;
			x += SPEED * dirx;
			y += SPEED * diry;
		}
	}//while
}
