#pragma once
class Cell
{
private:
	int row, column;
	Cell* parent;
	double g, h, f;
	int target_row, target_col;
public:
	Cell(const Cell& other);
	Cell(int r, int c, Cell* p);
	Cell(int r, int c, Cell* p, int tr, int tc,double g);
	Cell(int r, int c, Cell* p, double h = -1, int g = 0);
	int GetRow() { return row; }
	int GetColumn() { return column; }
	Cell* GetParent() { return parent; }
	double GetG() { return g; }
	void SetG(double value) { g = value; }
	double GetH() { return h; }
	double GetF() { return f; }
	void ComputeH();
	void ComputeF();
	bool operator == (const Cell &other) { return row == other.row && column == other.column; }
};

