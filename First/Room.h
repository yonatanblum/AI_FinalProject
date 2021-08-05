#pragma once
#include "Definitions.h"
#include "Player.h"

class Room
{
private:
	int centerRow, centerCol, width, height;
public:
	Room();
	~Room();
	void SetCenterRow(int r) { centerRow = r; }
	void SetCenterCol(int c) { centerCol = c; }
	void SetWidth(int w) { width = w; }
	void SetHeight(int h) { height = h; }
	void FillRoom(int maze[MSZ][MSZ], int value);
	int GetCenterRow() { return centerRow; }
	int GetCenterCol() { return centerCol; }
	int GetHeight() { return height; }
	int GetWidth(){ return width; }

	bool isValidCellForNewPlayer(int maze[MSZ][MSZ], int r, int c);
	//void AddPlayer(int maze[MSZ][MSZ], int color , Player p);
	int* getRandPosition(int maze[MSZ][MSZ]);
	bool isValidCell(int maze[MSZ][MSZ], int r, int c);		///
	int* getRandPositionForStore(int maze[MSZ][MSZ]);		///

};

