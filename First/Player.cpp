#include "Player.h"
#include <iostream>


Player::Player(int x, int y ,int id, int type , int teamNum)
{
	this->id=id;
	this->type = type; // type can be attacker=0 or squire =1
	this->healthPoints = MAX_HEALTH;
	this->teamNum = teamNum;
	this->row = x;
	this->col = y;
	this->numOfBullets = NUM_BULLETS, this->numOfGranades= MAX_GRANADES, this->numOfMedicine= MAX_HEALTH;

	for (int i= 0; i < MAX_GRANADES ;i++)
	{
		granades[i] = new Granade(0,0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	mode=0; // mode can be survival=1 or attack=0 
}

Player::Player()
{
	this->healthPoints = MAX_HEALTH;

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

double Player::distaneToEnemy(Player* allPlayers,int enemyNum)		/////
{
	double dist = sqrt(pow(allPlayers[enemyNum].getRow() - this->row, 2) + pow(allPlayers[enemyNum].getCol() - this->col, 2));
	return dist;
}

void Player::attack(Player* allPlayers, int index,double angle)		/////
{
	int kindOfShot;
	bool hit;
	double dist = distaneToEnemy(allPlayers, index);
	double currX, currY;
	currX = (2 * col / MSZ) - 1;
	currY = (2 * row / MSZ) - 1;
	if (dist > MAX_RANGE_GRANADE)
	{
		if (numOfBullets != 0)
		{
			Bullet* current = bullets[numOfBullets - 1];
			current->setX(currX);
			current->setY(currY);
			current->setAngle(angle);
			current->Fire();
			kindOfShot = 1;
			hit = isAHit(kindOfShot, allPlayers[index]);
			bullets[numOfBullets - 1] = NULL;
			numOfBullets--;
		}
	}
	else
	{
		if (numOfGranades != 0)
		{
			Granade* current = granades[numOfGranades - 1];
			current->setX(currX);
			current->setY(currY);
			current->Explode();
			kindOfShot = 2;
			hit = isAHit(kindOfShot, allPlayers[index]);
			granades[numOfGranades - 1] = NULL;
			numOfGranades--;
		}
	}
	if (hit==true)
		allPlayers[index].isHurt(kindOfShot, dist);
}

bool Player::isAHit(int kindOfShot,Player player)	/////
{
	bool hit = false;
	if (kindOfShot == 1)
	{
		if (bullets[numOfBullets-1]->getX() == player.getRow() && bullets[numOfBullets - 1]->getY() == player.getCol())
			hit = true;
	}
	else
	{
		if (granades[numOfBullets - 1]->getX() == player.getRow() && granades[numOfBullets - 1]->getY() == player.getCol())
			hit = true;
	}
	return hit;
}

void Player::isHurt(int kind,int dist)		/////													
{
	int injuryLevel;
	if (kind == 1)			// hit by bullet (long range)
		injuryLevel = (int)((-1)*(dist - MAX_RANGE_BULLET + 1) / 10);			// differential formula based on distance from the shot.
	else                     // hit by granade
		injuryLevel = (int)((-1)*(dist - MAX_RANGE_GRANADE + 1) / 10);
	healthPoints = healthPoints - injuryLevel;
	if (healthPoints <= 0)
		healthPoints = 0;
}
