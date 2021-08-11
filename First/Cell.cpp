#include "Cell.h"
#include <math.h>

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	column = c;
	parent = p;
	target_col = c;
	target_row = r;
	g = 0;
	ComputeH();
	ComputeF();

}

Cell::Cell(int r, int c, Cell * p, int tr, int tc,double g)
{
	row = r;
	column = c;
	parent = p;
	target_col = tc;
	target_row = tr;
	this->g = g;
	ComputeH();
	ComputeF();

}

Cell::Cell(const Cell &other) {
	row = other.row;
	column = other.column;
	parent = other.parent;
	target_col = other.target_col;
	target_row = other.target_row;
	g = other.g;
	ComputeH();
	ComputeF();
}

Cell::Cell(int r, int c, Cell* p, double h, int g)
{
	row = r;
	column = c;
	parent = p;
	this->h = h;
	this->g = g;

}

void Cell::ComputeH()
{
	// Euclidus distance
	h = sqrt(pow(row - target_row, 2) + pow(column - target_col, 2));
}

void Cell::ComputeF()
{
	f = g + h;
}
