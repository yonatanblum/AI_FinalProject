#include "Granade.h"

Granade::Granade(double x, double y)
{
	double alpha,teta = 2*3.14/NUM_BULLETS;
	int i;
	this->x = x;
	this->y = y;
	isExploded = false;

	for (alpha = 0,i=0; i<NUM_BULLETS && alpha < 2 * 3.14; alpha += teta,i++)
	{
		bullets[i] = new Bullet(x, y, alpha);
	}


}

void Granade::Explode()
{
	isExploded = true;
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i]->Fire();
}

void Granade::Exploding(int maze[MSZ][MSZ])
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i]->Move(maze);
}

void Granade::SimulateExplosion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double hurt)
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i]->SimulateFire(maze, security_map,hurt);

}

void Granade::Show()
{
	for (int i = 0; i < NUM_BULLETS; i++)
		bullets[i]->ShowMe();
}
