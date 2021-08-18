#include "Room.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Room::Room()
{
}


Room::~Room()
{
}

void Room::FillRoom(int maze[MSZ][MSZ], int value)
{
	int i, j;
	for (i = centerRow - height / 2; i <= centerRow + height / 2; i++)
		for (j = centerCol - width / 2; j <= centerCol + width / 2; j++)
			maze[i][j] = value;
}


bool Room::isValidCellForNewPlayer(int maze[MSZ][MSZ],int r,int c)
{
	if (maze[r][c] == WALL) return false;
	if (maze[r][c] == AMMO_STORE) return false;
	if (maze[r][c] == MEDICINE_STORE) return false;
	if (maze[r][c] == PLAYER1) return false;
	if (maze[r][c] == PLAYER2) return false;
	return true;
}


int* Room::getRandPosition(int maze[MSZ][MSZ])
{
	int res[2] = { 0 };
	do
	{
		res[0] = (rand() % height) + (centerRow - height / 2);
		res[1] = (rand() % width) + (centerCol - width / 2);
	} while (!isValidCellForNewPlayer(maze, res[0], res[1]));
	return res;
}

bool Room::isValidCell(int maze[MSZ][MSZ], int r, int c)							
{
	if (maze[r][c] != SPACE || maze[r + 1][c] != SPACE || maze[r][c + 1] != SPACE || maze[r + 1][c + 1] != SPACE || 
	    maze[r][c-1] != SPACE || maze[r + 1][c-1] != SPACE || maze[r-1][c] != SPACE || maze[r - 1][c + 1] != SPACE || 
	    maze[r][c+2] != SPACE || maze[r + 1][c+2] != SPACE || maze[r + 2][c+1] != SPACE || maze[r + 2][c] != SPACE )		
		return false;
	return true;
}

int* Room::getRandPositionForStore(int maze[MSZ][MSZ])
{
	int res[2] = { 0 };
	do
	{
		res[0] = (rand() % height) + (centerRow - height / 2);
		res[1] = (rand() % width) + (centerCol - width / 2);
	} while (!isValidCell(maze, res[0], res[1]));
	return res;
}
