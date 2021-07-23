#pragma once
#include "Definitions.h"


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
	void AddPlayer(int maze[MSZ][MSZ], int color);
	bool isValidCellForNewPlayer(int maze[MSZ][MSZ], int r, int c);
	bool isValidCellForNewPlayer(int maze[MSZ][MSZ]);
	bool isValidCellForNewPlayer();
	void AddPlayer(int maze[MSZ][MSZ]);
	int GetCenterRow() { return centerRow; }
	int GetCenterCol() { return centerCol; }
};

