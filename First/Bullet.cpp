#include "Bullet.h"
#include "GLUT.H"
#include <math.h>
#include <stdlib.h>		
#include <iostream>

Bullet::Bullet(double x, double y)
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
	bulletCol = (int)(MSZ * (x + 1) / 2);
	bulletRow = (int)(MSZ * (y + 1) / 2);	
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
		if (maze[row][column] == WALL ||  maze[row][column] == AMMO_STORE || maze[row][column] == MEDICINE_STORE)
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
	int currentRow,currentCol,row, column,newRow,newCol,cellType,count; 
	count = 0;
	dirx = cos(dir_angle);
	diry = sin(dir_angle);

	newRow = 999; newCol = 999;
	currentCol = (int)(MSZ * (x + 1) / 2);
	currentRow = (int)(MSZ * (y + 1) / 2);
	printf("bullet cell = (%d,%d) , cell type = %d\n", currentRow, currentCol, maze[currentRow][currentCol]);
	while (simulate && count<10)
	{
		column = (int)(MSZ * (x + 1) / 2);
		row = (int)(MSZ * (y + 1) / 2);
		if (row == currentRow && column == currentCol)
		{
			x = x + SPEED*dirx;
			y = y + SPEED*diry;
		}
		else
		{
			if (maze[row][column] == WALL || maze[row][column] == AMMO_STORE || maze[row][column] == MEDICINE_STORE || maze[row][column] == PLAYER1 || maze[row][column] == PLAYER2)
			{
				printf("bullet cell = (%d,%d) , cell type = %d\n", row, column, maze[row][column]);
				simulate = false;
			}
			else							// maze[row][column] is SPACE
			{
				security_map[row][column] += hurt;
				if (row != newRow || column != newCol)
				{
					printf("bullet cell = (%d,%d) , cell type = %d\n", row, column, maze[row][column]);
					count++;
				}
				newRow = row; newCol = column;
				//ShowMe();
				//maze[row][column] = BULLET;
				x += SPEED * dirx;
				y += SPEED * diry;
			}
		}
	}
	/*for (int i = 1; i < MSZ - 1; i++)			// clear the maze from bullets
		for (int j = 1; j < MSZ - 1; j++)
		{
			if (maze[i][j] == BULLET)
				maze[i][j] = SPACE;
		}*/
}
