
#pragma once
#include "Definitions.h"
#include "Granade.h"
#include "Bullet.h"

const int MAX_MEDICINES = 500;
const int MAX_BULLETS = 300;
const int MAXIMUM_GRANADES = 50;
const double RADIUS = 0.03;

enum storageType { Ammo, Medicines };

class Storage
{
private:
	int centerRow, centerCol;			// center of storage facility
	storageType storeType;
	int count;
public:
	Storage();
	~Storage();
	Storage(storageType type, int row, int col);
	int GetCenterRow() { return centerRow; }
	int GetCenterCol() { return centerCol; }
	int getCount() { return count; }
	void setCount(int remain) { count = remain; }
	storageType getType() { return storeType; }

	bool isEmpty();
	void fillPlayer(int amount);
	void drawStorage(storageType st, int i, int j, int maze[MSZ][MSZ]);
};

