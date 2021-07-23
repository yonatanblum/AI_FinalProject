#include "glut.h"
#include <time.h>
#include "Cell.h"
#include <vector>
#include <iostream>
#include "Room.h"
#include <queue>
#include "CompareCells.h"
#include "Bullet.h"
#include "Granade.h"
#include<windows.h>
#include "Player.h"

using namespace std;



const int NUM_ROOMS = 12;
const int NUM_TEAM_PLAYERS = 5; 

int maze[MSZ][MSZ] = { 0 };
double security_map [MSZ][MSZ] = { 0 };

Room rooms[NUM_ROOMS];
Bullet* pb = nullptr;
Granade* pg = nullptr;
Player allPlayers[2 * NUM_TEAM_PLAYERS];

void InitMaze();
void InitRooms();
void DigTunnels();
void InitPlayers(int teamNum);


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
	InitPlayers(PLAYER1 );
	InitPlayers(PLAYER2);
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

bool HasOverlap(int r, int c, int h, int w)
{
	for (int i = r - h / 2-1; i <= r + h / 2+1; i++)
		for (int j = c - w / 2-1; j <= c + w / 2+1; j++)
			if (maze[i][j] == SPACE)
				return true;

	return false;
}

void InitRooms()
{
	int i;
	int row, col, w, h;
	int minW=10, minH=10;

	for (i = 0; i < NUM_ROOMS; i++)
	{
		do
		{
			row = 8+minH/2 + rand() % (MSZ - minH-16);
			col = 8+minW/2+ rand() % (MSZ - minW-16);
			w = minW - 2 + rand() % 20;
			h = minH - 2 + rand() % 20;
		} while (HasOverlap(row,col,h,w));
			rooms[i].SetCenterRow(row);
			rooms[i].SetCenterCol(col);
			rooms[i].SetWidth(w);
			rooms[i].SetHeight(h);
			rooms[i].FillRoom(maze, SPACE);
	}

}


void InitPlayers(int teamNum )
{
	cout << "----------------Add players to maze -------------" << endl;
	int roomIndex;
	for (int i = 0; i < NUM_TEAM_PLAYERS; i++)
	{
		roomIndex = rand() % NUM_ROOMS;
		cout << "roomIndex=" << roomIndex << endl;
		rooms[roomIndex].AddPlayer(maze, teamNum );
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

void CheckNeighbor(Cell* pn, vector <Cell> &grays, vector <Cell> &blacks, priority_queue<Cell, 
	vector<Cell>, CompareCells> &pq, bool &go_on)
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
				while (!pq.empty() && !( pc->GetRow() == pn->GetRow() && pc->GetColumn() == pn->GetColumn()))
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

	double wall_cost=3, space_cost=0.1,cost;

	// starting cell is the center of the room i, target cell is the center of the room j
	Cell start = *(new Cell(rooms[i].GetCenterRow(),rooms[i].GetCenterCol(),nullptr, 
											rooms[j].GetCenterRow(), rooms[j].GetCenterCol(),0));
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
			
				pneighbor = new Cell(pcurrent->GetRow()+1,pcurrent->GetColumn(),pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(),pcurrent->GetG()+cost);

				CheckNeighbor(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetRow() > 0) // down
			{
				if (maze[pcurrent->GetRow() - 1][pcurrent->GetColumn()] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow() - 1, pcurrent->GetColumn(), pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighbor(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetColumn() > 0) // left
			{
				if (maze[pcurrent->GetRow() ][pcurrent->GetColumn()- 1] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow() , pcurrent->GetColumn()- 1, pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighbor(pneighbor, grays, blacks, pq, go_on);
			}
			if (go_on && pcurrent->GetColumn() < MSZ-1) // left
			{
				if (maze[pcurrent->GetRow()][pcurrent->GetColumn() + 1] == WALL)
					cost = wall_cost;
				else
					cost = space_cost; // space or target

				pneighbor = new Cell(pcurrent->GetRow(), pcurrent->GetColumn() + 1, pcurrent,
					rooms[j].GetCenterRow(), rooms[j].GetCenterCol(), pcurrent->GetG() + cost);

				CheckNeighbor(pneighbor, grays, blacks, pq, go_on);
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
		x =-1+2* (rand() % 100)/100.0;
		y = -1 + 2 * (rand() % 100) / 100.0;
		pg = new Granade(x, y);
		pg->SimulateExplosion(maze, security_map, one_bullet_hurt);
	}
}

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

	init();

	glutMainLoop();
}