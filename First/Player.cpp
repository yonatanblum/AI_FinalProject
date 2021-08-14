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
	this->numOfBullets = NUM_BULLETS, this->numOfGranades= MAX_GRANADES;

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