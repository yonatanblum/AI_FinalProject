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

void Room::AddPlayer(int maze[MSZ][MSZ],int color  )
{
	int r=0, c=0;	
	do
	{
	r = (rand() % height)+ (centerRow - height / 2);
	c = (rand() % width) + (centerCol - width / 2);
	} while (!isValidCellForNewPlayer(maze,r,c));
	maze[r][c] = color;
	cout << "r : "<<r<< ", c : "<<c <<endl ;


}

bool Room::isValidCellForNewPlayer(int maze[MSZ][MSZ],int r,int c)
{
	if (maze[r][c] == WALL) return false;
	if (maze[r][c] == PLAYER1) return false;
	if (maze[r][c] == PLAYER2) return false;
	return true;
}
