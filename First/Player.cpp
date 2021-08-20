#include "Player.h"
#include <iostream>
#include <math.h>
using namespace std;

const int MAX_RANGE_GRANADE = 15;
const int MAX_RANGE_BULLET = 30;



Player::Player(int x, int y ,int id, int type , int teamNum)
{
	this->id=id;
	this->type = type; // type can be attacker=0 or squire =1
	this->healthPoints = MAX_HEALTH;
	this->teamNum = teamNum;
	this->row = x;
	this->col = y;
	x = ((2 * col / MSZ) - 1);
	y = ((2 * row / MSZ) - 1);
	numOfBullets = NUM_BULLETS;
	//cout << "numOfBullets" << numOfBullets<< endl; 
	this->numOfGranades = MAX_GRANADES;
	this->numOfMedicine = MAX_HEALTH;

	for (int i= 0; i < MAX_GRANADES ;i++)
	{
		granades[i] = new Granade(0,0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	mode=0; // mode can be survival=1 or attack=0 
	//this->printPlayer();
}

Player::Player()
{
	healthPoints = MAX_HEALTH;
	numOfBullets = NUM_BULLETS;
	numOfGranades = MAX_GRANADES;
	numOfMedicine = MAX_HEALTH;

	for (int i = 0; i < MAX_GRANADES; i++)
	{
		granades[i] = new Granade(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}
}

void Player::setPosition(int x, int y)
{
	this->row = x;
	this->col = y;
	mode = 0; // mode can be survival=1 or attack=0 
}

void Player::setPlayer(int id, int  type, int teamNum)
{
	this->id = id;
	this->type = type; // type can be attacker=0 or squire =1
	this->teamNum = teamNum;
}


int Player::searchEnemy(Player * allPlayers, int maxPlayers)
{
	int nearest = this->id;
	int minDist = 99999;
	int dist;
	for (int i= 0 ; i< maxPlayers;i++)
	{
		if (allPlayers[i].getTeamNum() != this->teamNum && allPlayers[i].getHealthPoints()!=0)
		{
			 dist = sqrt(pow(allPlayers[i].getRow() - this->row, 2) + pow(allPlayers[i].getCol() - this->col, 2));
			 if (dist < minDist)
			 {
				 minDist = dist;
				 nearest = i; 
			 }
		}
	}
	if (nearest == this->id) return -1;
	return nearest;
}

int Player::searchToHelp(Player* allPlayers, int maxPlayers)
{
	int bestScore = this->id;
	int minScore = 200;
	int score = 9999;
	for (int i = 0; i < maxPlayers; i++)
	{
		if (i != this->id && allPlayers[i].getTeamNum() == this->teamNum && allPlayers[i].getHealthPoints() != 0 )
		{
			score = allPlayers[i].getHealthPoints() + allPlayers[i].getNumOfBullets() + allPlayers[i].getNumOfGranades();
			if (score < minScore)
			{
				minScore = score;
				bestScore = i;
			}

		}
	}
	if (bestScore == this->id) return -1;
	return bestScore;
}

int Player::searchStorage(Storage* allStorage, int maxStorage)
{
	int nearest = -1;
	int minDist = 99999;
	int dist;
	for (int i = 0; i < maxStorage; i++)
	{
		if (!allStorage[i].isEmpty())
		{
			dist = sqrt(pow(allStorage[i].GetCenterRow() - this->row, 2) + pow(allStorage[i].GetCenterCol() - this->col, 2));
			if (dist < minDist)
			{
				minDist = dist;
				nearest = i;
			}
		}
	}
	return nearest;
}

void Player::getAmmpFromStorage(Storage* allStorage, int id)
{
	if (this->numOfBullets == 0) this->numOfBullets = allStorage[id].getCharge(NUM_PLAYER_BULLETS);
	if (this->numOfGranades == 0) this->numOfGranades = allStorage[id].getCharge(MAX_GRANADES);

}

void Player::attack(int maze[MSZ][MSZ],double map[MSZ][MSZ],Player* allPlayers, int index,double angle,double dist)																			/////
{
	double currX, currY;
	double hurt = 1;
	currX = (2 * col / MSZ) - 1;
	currY = (2 * row / MSZ) - 1;
	if (dist > MAX_RANGE_GRANADE)
	{
		if (numOfBullets != 0)
		{
			bullet = new Bullet(currX, currY, angle);
			bullet->Fire();
			numOfBullets--;
			cout << "------ Shut Bullet!!!! ----------\n";
			bullet->SimulateFire(maze, map, hurt);
			bullet = NULL;
		}
	}
	else
	{
		if (numOfGranades != 0)
		{
			granade = new Granade(currX, currY);
			granade->Explode();
			numOfGranades--;
			cout << "------ Throw Granade!!!! ----------\n";
			granade->SimulateExplosion(maze, map, hurt);
			granade = NULL;
		}
	}
}

void Player::isHurt(int distOfShot)																								/////
{
	int injuryLevel;
	if (distOfShot > MAX_RANGE_GRANADE)			// hit by bullet (long range)
		injuryLevel = (int)((-1)*(distOfShot - MAX_RANGE_BULLET + 1) / 10);			// differential formula based on distance from the shot.
	else                     // hit by granade
		injuryLevel = (int)((-1)*(distOfShot - MAX_RANGE_GRANADE + 1) / 10);
	healthPoints = healthPoints - injuryLevel;
	if (healthPoints <= 0)
		healthPoints = 0;
}


void Player::printPlayer()
{
	cout << "--------Print Player num : " << id <<" -----------------------" <<endl;
	cout << "Cell : (" << row << "," << col << ") | type : " << type <<" mode : "<< mode << " | healthPoints : " << healthPoints<< " teamNum : "<< teamNum<< endl;
	cout <<"numOfBullets : " <<numOfBullets  <<" numOfGranades : " << numOfGranades << " numOfMedicine " << numOfMedicine <<endl;
	cout << "End to Print Player num : " << id <<" -----------------------"<<  endl;
}
