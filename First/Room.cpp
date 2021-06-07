#include "Room.h"



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
