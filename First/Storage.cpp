
#include "Storage.h"
#include <math.h>

Storage::Storage()
{

}

Storage::~Storage()
{

}

Storage::Storage(storageType type, int row, int col,int id)
{
	storeType = type;
	centerRow = row;
	centerCol = col;
	this->id = id;
	if (type == Ammo)
		count = MAXIMUM_GRANADES + MAX_BULLETS;
	else
		count = MAX_MEDICINES;
}

bool Storage::isEmpty()
{
	return(count == 0);
}

void Storage::fillPlayer(int amount)			// toDO - in parallel, update the player's supply relatively		
{
	if (count >= amount)
		count = count - amount;
	else
		count = 0;
}

void Storage::drawStorage(storageType st, int i, int j,int maze[MSZ][MSZ])
{
	int content;
	if (st == Ammo)
		content = AMMO_STORE;
	else
		content = MEDICINE_STORE;
	maze[i][j] = content;
	maze[i+1][j] = content;
	maze[i][j+1] = content;
	maze[i+1][j+1] = content;

}

int Storage::getCharge(int chargeNum)
{

	if (this->count < chargeNum)
	{
		int tmp = this->count;
		this->count = 0;
		return tmp;
	}
	else
	{
		this->count = this->count - chargeNum;
		return chargeNum;
	}
}




