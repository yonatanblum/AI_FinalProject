#include "glut.h"//
#include <time.h>//
#include "Cell.h"
#include <vector>
#include <iostream>
#include "Room.h"
#include <queue>
#include "CompareCells.h"
#include "Bullet.h"
#include "Granade.h"
#include <windows.h>
#include "Player.h"
#include "Storage.h"			
#include <math.h>

//NIR Branch
using namespace std;



const int NUM_ROOMS = 12;
const int NUM_TEAM_PLAYERS = 1; 
const int STORE_IN_ROOM = 1;
const int MAX_RANGE_ATTACK = 20;
const int MIN_RANGE_ATTACK = 0;

int maze[MSZ][MSZ] = { 0 };
double security_map [MSZ][MSZ] = { 0 };

bool runGame = false, runPlayer;

vector <Cell*> grayss[NUM_TEAM_PLAYERS*2];
Room rooms[NUM_ROOMS];
Storage ammoStore[NUM_ROOMS*STORE_IN_ROOM];			// 12x1				
Storage medicineStore[NUM_ROOMS*STORE_IN_ROOM];			// 12x1			
Bullet* pb = nullptr;
Granade* pg = nullptr;
Player allPlayers[NUM_TEAM_PLAYERS*2];

void InitMaze();
void InitRooms();
void DigTunnels();


void initAllStorages();																		
void addAllStoragesToRoom(int roomNum);														

void InitPlayers();
void AddPlayerToMaze(int id, int teamNum, int roomIndex);
int getTeamNum(int ix);
void CheckNeighborByPoint(Cell* pcurrent, int row, int col, int id, int rowT, int colT);
void AStarIterationByPoint(int runIndex, int rowT, int colT);



void init()
{
	int r, c;
	srand(time(0));
	glClearColor(1, 1, 1, 0);// color of window background
	glOrtho(-1, 1, -1, 1, 0, 1);

	InitMaze();
	InitRooms();

	for (int i = 0; i < 100; i++)
	{
		r = rand() % MSZ;
		c = rand() % MSZ;
		maze[r][c] = WALL;
	}
	DigTunnels();


	initAllStorages();															///

	InitPlayers();
	//InitPlayers(allPlayers2,PLAYER2);

}

void InitMaze()
{
	int i, j;

	for (i = 0; i < MSZ ; i++)
		for (j = 0; j < MSZ ; j++)
		{
					maze[i][j] = WALL;
		}
}



void InitPlayers()
{
	cout << "----------------Add players to maze -------------" << endl;
	int roomIndex;
	int teamNum;
	for (int i = 0; i < NUM_TEAM_PLAYERS*2; i++)
	{
		roomIndex = rand() % NUM_ROOMS;
		cout << "roomIndex=" << roomIndex << endl;
		if (i >= NUM_TEAM_PLAYERS) teamNum = PLAYER2;
		else teamNum = PLAYER1;
		AddPlayerToMaze(i, teamNum, roomIndex);
		allPlayers[i].printPlayer();
	}
}


void AddPlayerToMaze(int id,int teamNum, int roomIndex)
{
	int* res = rooms[roomIndex].getRandPosition(maze);
	int r = res[0], c = res[1];
	maze[r][c] = teamNum;
	cout << "r : " << r << ", c : " << c << endl;
	allPlayers[id].setPosition(r, c);
	//if (NUM_TEAM_PLAYERS > 1 && id == 0) 
		//allPlayers[id].setPlayer(id, 1, teamNum); // type=1 is squire 
	//else
	allPlayers[id].setPlayer(id,0, teamNum); // type=0 is attacker
	Cell* pc = new Cell(r, c, nullptr,9999.0,0);
	grayss[id].push_back(pc);
}

void ClearMaze(int id, int teamNum)
{
	int i, j;

	for (i = 1; i < MSZ - 1; i++)
		for (j = 1; j < MSZ - 1; j++)
		{
			if (maze[i][j] == BLACK || maze[i][j] == GRAY || maze[i][j] == PATH)
			{
				maze[i][j] = SPACE;
			}

		}

	for (i = 0; i < NUM_TEAM_PLAYERS*2; i++) {
		maze[allPlayers[i].getRow()][allPlayers[i].getCol()] = allPlayers[i].getTeamNum();
	}
	// mark out the start and the target cells

}
void MoveToCell(Cell* pcurrent , int id , int teamNum)
{
	while (maze[pcurrent->GetRow()][pcurrent->GetColumn()] != teamNum)
	{

		Cell* next = pcurrent->GetParent();

		if (maze[next->GetRow()][next->GetColumn()] == teamNum)
		{
			int r = allPlayers[id].getRow();
			int c = allPlayers[id].getCol();
			allPlayers[id].setPosition(pcurrent->GetRow(), pcurrent->GetColumn())  ;
			maze[r][c] = teamNum;
			Cell* pc = new Cell(*pcurrent);
			grayss[id].clear();
			grayss[id].push_back(pc); // the start cell is in grays
		//}


		}
		pcurrent = pcurrent->GetParent();
	}
	maze[pcurrent->GetRow()][pcurrent->GetColumn()] = SPACE;
}

int getTeamNum(int ix)
{
	if (ix >= NUM_TEAM_PLAYERS) {
		return PLAYER2;
	}
	return PLAYER1;
}

int getTeamTarget(int ix)
{
	if (ix >= NUM_TEAM_PLAYERS) {
		return PLAYER1;
	}
	return PLAYER2;
}

void AStarIterationByPoint(int runIndex,int rowT, int colT)
{

	Cell* pcurrent;

	if (grayss[runIndex].empty())
	{
		cout << "There is no solution\n";
	}
	else // there are gray cells
	{
		int index = 0;
		pcurrent = grayss[runIndex].front();  // save the FIRST element,
		for (int i = 1; i < grayss[runIndex].size(); i++) // find best route (find min F from array) 
		{
			double f1 = pcurrent->GetH();
			double f2 = grayss[runIndex][i]->GetH();
			if (f1 > f2)
			{
				pcurrent = grayss[runIndex][i];
				index = i;
			}
		}
		grayss[runIndex].erase(grayss[runIndex].begin() + index); // remove it from the queue
		if (maze[pcurrent->GetRow()][pcurrent->GetColumn()] != allPlayers[runIndex].getTeamNum())
			maze[pcurrent->GetRow()][pcurrent->GetColumn()] = BLACK;   // and paint it black

		// now check the neighbors
		// up
		CheckNeighborByPoint(pcurrent, pcurrent->GetRow() + 1, pcurrent->GetColumn(), runIndex , rowT ,colT);
		// down
		if (runPlayer)
			CheckNeighborByPoint(pcurrent, pcurrent->GetRow() - 1, pcurrent->GetColumn(), runIndex, rowT, colT);
		// right
		if (runPlayer)
			CheckNeighborByPoint(pcurrent, pcurrent->GetRow(), pcurrent->GetColumn() + 1, runIndex, rowT, colT);
		// left
		if (runPlayer)
			CheckNeighborByPoint(pcurrent, pcurrent->GetRow(), pcurrent->GetColumn() - 1, runIndex, rowT, colT);
	}

}

void CheckNeighborByPoint(Cell* pcurrent, int row, int col, int id,int rowT, int colT)
{
	// check the color of the neighbor cell

	if (maze[row][col] == SPACE || (row == rowT && col == colT))
	{
		if (row == rowT && col == colT) // the solution has been found
		{
			if (pcurrent->GetRow() == rowT && pcurrent->GetColumn() == colT) // 1 step before pacman
			{

				runGame = false;
			}

			//cout << "the solution has been found\n";
			MoveToCell(pcurrent, id, allPlayers[id].getTeamNum());
			ClearMaze(id, allPlayers[id].getTeamNum());

			runPlayer = false;

		}

		else  // it is white neighbor, so make it gray
		{
			int g = pcurrent->GetG() + 1;
			double h = sqrt(pow(rowT - row, 2) + pow(colT - col, 2));
			Cell* pc = new Cell(row, col, pcurrent, h, g);
			grayss[id].push_back(pc);
			maze[row][col] = GRAY;
		}
	}
}

double distanceOfPlayers(Player p1,Player p2)	// distance between attacker and enemy								/////
{
	double dist = sqrt(pow(p1.getRow() - p2.getRow(), 2) + pow(p1.getCol() - p2.getCol(), 2));
	return dist;
}

double calcAngleBetweenCells(int centerRow1, int centerCol1, int centerRow2, int centerCol2)	// angle between attacker and enemy		/////
{
	double angle;
	int lengthX = centerCol2 - centerCol1;
	int lengthY = centerRow2 - centerRow1;
	if (lengthX == 0 && lengthY == 0)
		angle = 0;
	else if (lengthX == 0 || lengthY==0)
		angle = 3.14 / 2;
	else
		angle = atan(lengthY / lengthX);
	return angle;
}

/*bool haveEyeContact(Player attacker, Player enemy,double angle,double dist)	// former function - not deleting it before we are sure the new one is ok																	/////
{
	double lengthY = enemy.getCol() - attacker.getCol();
	double lengthX = enemy.getRow() - attacker.getRow();
	double stepX = lengthX / dist;
	double stepY = lengthY / dist;
	double x = attacker.getRow(); double y = attacker.getCol();
	int row = attacker.getRow(), col = attacker.getCol();
	printf("attacker row = %d , attacker col = %d\n", attacker.getRow(), col = attacker.getCol());
	printf("enemy row = %d , enemy col = %d\n", enemy.getRow(), col = enemy.getCol());
	printf("stepX = %lf , stepY = %lf\n", stepX, stepY);
	printf("distance = %lf\n", dist);
	double dirx, diry;
	double dir_angle = atan2(lengthY,lengthX); // in rads
	dirx = cos(dir_angle);
	diry = sin(dir_angle);
	while (x!= enemy.getRow() || y!= enemy.getCol())
	{

		int angleInDegree = dir_angle * 180 / 3.14; //angle in degree
		printf("row = %d , col = %d", row, col);
		//x = x + stepX;
		//y = y + stepY;
		
		col = (int)(MSZ * (x + 1) / 2);
		row = (int)(MSZ * (y + 1) / 2);
		printf("row = %d , col = %d\n", row, col);
		if (maze[row][col] == WALL || maze[row][col] == AMMO_STORE || maze[row][col] == MEDICINE_STORE )  // toDO - add same team player
			return false;
		else
		{
			x += dirx;
			y += diry;
		}
	}
	return true;
}

bool canAttack(Player attacker, Player enemy,double angle,double dist)		// former function - not deleting it before we are sure the new one is ok
{
	if ( dist <= MAX_RANGE_ATTACK)
	{
		//if (haveEyeContact(attacker, enemy, angle,dist)) //TODO need to fix this function (Pass for now)
			return true;
	}
	return false;
}*/


bool haveEyeContact(Player attacker, Player enemy)	// new function - check the printing!!!																	/////
{
	int i, j, enemyi, enemyj;
	double enemyRow, enemyCol, row, col, lengthRow,lengthCol,distRC,stepRow,stepCol,di,dj;
	i = attacker.getRow(), j = attacker.getCol();		// (i,j) = (cell row index, cell col index)
	enemyi = enemy.getRow(), enemyj = enemy.getCol();	// enemy cell indexes
	row = (H / MSZ) * i; col = (W / MSZ) * j;		// start point (row,col) in board size 600x600
	enemyRow = (H / MSZ) * enemyi;  enemyCol = (W / MSZ) * enemyj;		// enemy (row,col) in board 600x600
	lengthRow = enemyRow - row;  lengthCol = enemyCol - col;		// distance between players rows and columns in board 600x600
	distRC = sqrt(pow(lengthRow, 2) + pow(lengthCol, 2));			// mathematical distance between players in board 600x600
	stepRow = lengthRow / distRC;  stepCol = lengthCol / distRC;		// the step that is done each time in row and in column
	printf("attacker i = %d , attacker j = %d\n", attacker.getRow(), attacker.getCol());
	printf("enemy i = %d , enemy j = %d\n", enemy.getRow(), enemy.getCol());
	printf("start row = %lf , start col = %lf\n", row, col);
	printf("row length = %lf , col length = %lf , distRC = %lf\n", lengthRow, lengthCol, distRC);
	printf("row step = %lf , col step = %lf\n", stepRow,stepCol);
	while (i!= enemyi || j!= enemyj)
	{
		col = col + stepCol;		
		row = row + stepRow;
		j = (int)(col / (W/MSZ));
		i = (int)(row / (H/MSZ));
		printf("row = %lf , col = %lf , i = %d , j = %d , cell = %d\n", row, col, i , j, maze[i][j]);
		if (maze[i][j] == WALL || maze[i][j] == AMMO_STORE || maze[i][j] == MEDICINE_STORE )  // toDO - add same team player
			return false;
	}
	return true;
}

bool canAttack(Player attacker, Player enemy,double angle,double dist)		// new function
{
	if (dist <= MAX_RANGE_ATTACK && dist >= MIN_RANGE_ATTACK)
	{
		if (haveEyeContact(attacker, enemy))
			return true;
	}
	return false;
}

bool isAHit()																				/////
{
	bool hit = false;
	if ((rand() % 100) < 30)
	{
		hit = true;
		cout << "-----------------it's a hit!!!!!!----------------------\n";
	}
	else
		cout << "---------------------it's a miss!!!!!----------------------\n";
	return hit;
}

double calcAngleBetweenPlayers(int idPlayer, int idEnemy) 
{
	return calcAngleBetweenCells(allPlayers[idPlayer].getRow(), allPlayers[idPlayer].getCol(), allPlayers[idEnemy].getRow(), allPlayers[idEnemy].getCol());
}

void movePlayer(int runIndex ,int enemyID)
{
	runPlayer = true;
	while (runPlayer)
	{
		AStarIterationByPoint(runIndex, allPlayers[enemyID].getRow(), allPlayers[enemyID].getCol());
	}
}

void attackPlayer(int runIndex, int enemyID, int angle , int dist)
{
	

	allPlayers[runIndex].attack(maze, security_map, allPlayers, enemyID, angle, dist);
	if (isAHit())
		allPlayers[enemyID].isHurt(dist);		// enemy is injured
	if (allPlayers[enemyID].getHealthPoints() == 0)								// if the attacked player is dead
	{
		maze[allPlayers[enemyID].getRow()][allPlayers[enemyID].getCol()] = SPACE;		// erase player image from maze
	}
}

void playerStorageSaction(int runIndex)
{
	int helpId = allPlayers[runIndex].searchToHelp(allPlayers, NUM_TEAM_PLAYERS * 2);
	if (helpId != -1)
	{
		while (runPlayer)
		{
			int mStorageId = allPlayers[runIndex].searchStorage(medicineStore, NUM_ROOMS * STORE_IN_ROOM);
			int aStorageId = allPlayers[runIndex].searchStorage(ammoStore, NUM_ROOMS * STORE_IN_ROOM);
			if (allPlayers[runIndex].getNumOfMedicine() == 0 && mStorageId != -1) //search for medicine 
			{
				AStarIterationByPoint(runIndex, medicineStore[mStorageId].GetCenterRow(), medicineStore[mStorageId].GetCenterCol());
				if (!runPlayer) allPlayers[runIndex].getAmmoFromStorage(medicineStore, mStorageId);
			}
			else if (allPlayers[runIndex].getNumOfBullets() + allPlayers[runIndex].getNumOfGranades() == 0 && aStorageId != -1)//search for ammo
			{
				AStarIterationByPoint(runIndex, ammoStore[aStorageId].GetCenterRow(), ammoStore[aStorageId].GetCenterCol());
				if (!runPlayer) allPlayers[runIndex].getAmmoFromStorage(ammoStore, aStorageId);
			}
			else AStarIterationByPoint(runIndex, allPlayers[helpId].getRow(), allPlayers[helpId].getCol()); //search for help
		}
	}
}

void DoAction(int runIndex)
{
	int teamNum = getTeamNum(runIndex);
	int targetTeam= getTeamTarget(runIndex);
	
	allPlayers[runIndex].printPlayer();
	if (allPlayers[runIndex].getType() == 0) // attacker 
	{
		int enemyID = allPlayers[runIndex].searchEnemy(allPlayers, NUM_TEAM_PLAYERS * 2);
		double angle = calcAngleBetweenPlayers(runIndex, enemyID);
		double dist = distanceOfPlayers(allPlayers[runIndex], allPlayers[enemyID]);
		if (allPlayers[enemyID].isEmpty()) // Do nothing (move or attack ) 
		{
			if (canAttack(allPlayers[runIndex], allPlayers[enemyID]))
			{
				attackPlayer(runIndex,enemyID,angle,dist);
			}
			else
			{
				movePlayer(runIndex,enemyID);
			}
		}
	}
	else
	{
		playerStorageSaction(runIndex);	
	}
}





void RunGame()
{
	for (int i = 0; i < NUM_TEAM_PLAYERS*2; i++)
	//for (int i = 1; i < 3; i++)
	{
		if (allPlayers[i].isAlive())
		{
			DoAction(i);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------CREATE MAZE----------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

bool HasOverlap(int r, int c, int h, int w)
{
	for (int i = r - h / 2 - 1; i <= r + h / 2 + 1; i++)
		for (int j = c - w / 2 - 1; j <= c + w / 2 + 1; j++)
			if (maze[i][j] == SPACE)
				return true;

	return false;
}

void InitRooms()
{
	int i;
	int row, col, w, h;
	int minW = 10, minH = 10;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		do
		{
			row = 8 + minH / 2 + rand() % (MSZ - minH - 16);
			col = 8 + minW / 2 + rand() % (MSZ - minW - 16);
			w = minW - 2 + rand() % 20;
			h = minH - 2 + rand() % 20;
		} while (HasOverlap(row, col, h, w));
		rooms[i].SetCenterRow(row);
		rooms[i].SetCenterCol(col);
		rooms[i].SetWidth(w);
		rooms[i].SetHeight(h);
		rooms[i].FillRoom(maze, SPACE);
	}

}

void initAllStorages()																				///
{
	for (int i = 0; i < NUM_ROOMS; i++)
		addAllStoragesToRoom(i);
}

void addAllStoragesToRoom(int roomNum)																///
{
	int* res;
	int r, c;
	int id;
	for (int storeNum = 0; storeNum < STORE_IN_ROOM; storeNum++)
	{
		id = roomNum * STORE_IN_ROOM + storeNum;
		res = rooms[roomNum].getRandPositionForStore(maze);
		r = res[0], c = res[1];
		ammoStore[id] = Storage(Ammo, r, c, id);
		ammoStore[id].drawStorage(Ammo, r, c, maze);
	}
	for (int storeNum = 0; storeNum < STORE_IN_ROOM; storeNum++)
	{
		id = roomNum * STORE_IN_ROOM + storeNum;
		res = rooms[roomNum].getRandPositionForStore(maze);
		r = res[0], c = res[1];
		medicineStore[id] = Storage(Medicines, r, c, id);
		medicineStore[id].drawStorage(Medicines, r, c, maze);
	}
}

void DigPath(Cell* pn)
{
	while (pn != nullptr)
	{
		if (maze[pn->GetRow()][pn->GetColumn()] == WALL)
			maze[pn->GetRow()][pn->GetColumn()] = SPACE;
		pn = pn->GetParent();
	}
}

void CheckNeighborDig(Cell* pn, vector <Cell>& grays, vector <Cell>& blacks, priority_queue<Cell,
	vector<Cell>, CompareCells>& pq, bool& go_on)
{
	vector<Cell>::iterator it_gray;
	vector<Cell>::iterator it_black;
	vector<Cell> tmp; //  we'll need it to update pq
	Cell* pc;
	// is it Target?
	if (pn->GetH() < 0.01) // this is the target! In our case this means the end of A*.
	{
		go_on = false;
		DigPath(pn);
		return;
	}
	else // it is not a target
	{
		// is pn white, gray or black?
		it_gray = find(grays.begin(), grays.end(), *pn);
		it_black = find(blacks.begin(), blacks.end(), *pn);
		if (it_gray == grays.end() && it_black == blacks.end())//it is white
		{
			pq.push(*pn); // add it to pq
			grays.push_back(*pn); // and paint it gray
		}
		else if (it_gray != grays.end()) // it is gray
		{
			if (pn->GetG() < it_gray->GetG()) // we have to update G and F in grays and we have to update pq
			{
				//				it_gray->SetG(pn->GetG()); // update G of a copy that is in grays
				//				it_gray->ComputeF(); // ???
				*it_gray = *pn;

				// update pq;
				pc = new Cell(pq.top());
				while (!pq.empty() && !(pc->GetRow() == pn->GetRow() && pc->GetColumn() == pn->GetColumn()))
				{
					pq.pop();
					tmp.push_back(*pc);
					pc = new Cell(pq.top());
				}
				// if pq is not empty pn has been found
				pq.pop();
				pq.push(*pn); // insert to pq the better copy of the neighbor
				while (!tmp.empty())
				{
					pq.push(tmp[tmp.size() - 1]);
					tmp.pop_back();
				}
			}
		}

	}
}


// runs A* from room i to room j
void PaveWay(int i, int j)
{
	vector <Cell> grays;
	vector <Cell> blacks;
	priority_queue<Cell, vector<Cell>, CompareCells> pq;

	vector<Cell>::iterator it_gray;

	Cell* pcurrent;
	Cell* pneighbor;
	bool go_on = true;

	double wall_cost = 3, space_cost = 0.1, cost;

	// starting cell is the center of the room i, target cell is the center of the room j
	Cell start = *(new Cell(rooms[i].GetCenterRow(), rooms[i].GetCenterCol(), nullptr,
		rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), 0));
	pq.push(start);
	grays.push_back(start);

	while (go_on)
	{
		if (pq.empty()) // there is no solution but not in our case
		{
			cout << "there is no solution: pq is empty\n";
			go_on = false;
			return;
		}
		else // pq is not empty
		{
			// Cell tmp_cell = pq.top();
			pcurrent = new Cell(pq.top()); // pick the best cell
			pq.pop(); // remove it from pq
			// pcurrent can be the TARGET. This is in case when we didn't stop when we first found the target
			// remove it from grays and paint it black
			it_gray = find(grays.begin(), grays.end(), *pcurrent);
			if (it_gray == grays.end()) // pcurrent wasn't found : this is ERROR
			{
				cout << "ERROR: pcurrent wasn't found in grays\n";
				go_on = false;
				return;
			}
			// pcurrent was found
			grays.erase(it_gray);
			blacks.push_back(*pcurrent);
			// now check the neighbors
			if (pcurrent->GetRow() < MSZ - 1) // up
			{
				if (maze[pcurrent->GetRow() + 1][pcurrent->GetColumn()] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow() + 1, pcurrent->GetColumn(), pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighborDig(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetRow() > 0) // down
			{
				if (maze[pcurrent->GetRow() - 1][pcurrent->GetColumn()] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow() - 1, pcurrent->GetColumn(), pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighborDig(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetColumn() > 0) // left
			{
				if (maze[pcurrent->GetRow()][pcurrent->GetColumn() - 1] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow(), pcurrent->GetColumn() - 1, pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighborDig(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetColumn() < MSZ - 1) // left
			{
				if (maze[pcurrent->GetRow()][pcurrent->GetColumn() + 1] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow(), pcurrent->GetColumn() + 1, pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighborDig(pneighbor, grays, blacks, pq, go_on);
			}


		}
	}

}

void DigTunnels()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
		for (j = 1; j < NUM_ROOMS; j++)
		{
			PaveWay(i, j); // A*
			cout << "Path from " << i << " to " << j << " has been paved\n";
		}
}

void CreateSecurityMap()
{
	int num_simulations = 5000;
	double one_bullet_hurt = 1.0 / num_simulations;
	double x, y;
	for (int i = 0; i < num_simulations; i++)
	{
		x = -1 + 2 * (rand() % 100) / 100.0;
		y = -1 + 2 * (rand() % 100) / 100.0;
		pg = new Granade(x, y);
		pg->SimulateExplosion(maze, security_map, one_bullet_hurt);
	}
}



//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//----------------------------------------CONFIGURATION MAZE----------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
void DrawMaze()
{
	int i, j;
	int xsz = W / MSZ;
	int ysz = H / MSZ;
	double x, y;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (maze[i][j])   // set color
			{
			case SPACE:
//				glColor3d(0.9, 0.9, 0.9);
				// assumption: the value of security_map[i][j] is in range (0,1)
				glColor3d(1-security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]); 
				break;
			case WALL:
				glColor3d(0.3, 0, 0);
				break;
			case START:
				glColor3d(0.5, 0.5, 1);
				break;
			case TARGET:
				glColor3d(1, 0, 0);
				break;
			case BLACK:
				glColor3d(0.9, 0.9, 0.5);
				break;
			case GRAY:
				glColor3d(0.2, 0.6, 0.2);
				break;
			case PATH:
				glColor3d(1, 0.6, 1);
				break;
			case PLAYER1:
				glColor3d(0, 0.9, 0.9); // turquoise
				break;
			case PLAYER2:
				glColor3d(0.9, 0.4, 0.5); // light red
				break;
			case AMMO_STORE:
				glColor3d(1, 0.6, 0); // orange?
				break;
			case MEDICINE_STORE:
				glColor3d(1, 1, 0); // yellow
				break;
			}

			// draw square
			x = 2.0 * (j * xsz) / W - 1; // value in range [-1,1)
			y = 2.0 * (i * ysz) / H - 1; // value in range [-1,1)
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + (2.0*ysz) / H);
			glVertex2d(x + 2.0*xsz / W, y + (2.0 * ysz) / H);
			glVertex2d(x + 2.0 * xsz / W, y);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();

	if (pb != nullptr)
		pb->ShowMe();

	if (pg)
		pg->Show();

	
	glutSwapBuffers(); // show all
}

void idle()
{
	
	if (pb != nullptr)
	{
		if (pb->getIsFired())
			pb->Move(maze);
	}

	if (pg != nullptr && pg->getIsExploded())
	{
		pg->Exploding(maze);
	}

	if (runGame) RunGame();
	glutPostRedisplay(); // go to display
}

void menu(int choice)
{
	if (choice == 1) // fire bullet
	{
		if (pb != nullptr)
			pb->Fire();
	}
	else if (choice == 2) // throw grenade
	{
		if (pg != nullptr)
			pg->Explode(); // only once per granade
	}
	else if (choice == 3) // create security map
	{
		CreateSecurityMap();
	}
	else if (choice == 4) // create security map
	{
		runGame = true;
	}

}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		double xx, yy;
		xx = 2 * ((double)x) / W - 1;
		yy = 2 * ((double)(H-y)) / H - 1;
		// create bullet at (x,y)
//		pb = new Bullet(xx, yy); // we shall need a bullet to fire it from riffle (only wall or enemy body will stop it)
		pg = new Granade(xx, yy); // in the game the granade's bullets are flying to the limited distance
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display); // display is window refresh function
	glutIdleFunc(idle);  // idle is background function
	glutMouseFunc(mouse);

	// menu
	glutCreateMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutAddMenuEntry("Fire bullet", 1);
	glutAddMenuEntry("Throw grenade", 2);
	glutAddMenuEntry("Create Security Map", 3);
	glutAddMenuEntry("Start Game", 4);

	init();

	glutMainLoop();
}
