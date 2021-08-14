#pragma once
#include "Granade.h"
#include "Bullet.h"

const int MAX_HEALTH = 100;
const int MAX_GRANADES = 3;
const int NUM_PLAYER_BULLETS = 50;

class Player
{

private: 
	int id; 
	int type; // type can be attacker=0 or squire =1
	int healthPoints;
	int teamNum; 
	int row, col;
	Granade* granades[MAX_GRANADES];
	Bullet* bullets[NUM_BULLETS];
	int numOfBullets, numOfGranades;
	int mode; // mode can be survival=1 or attack=0 


public :
	Player(int x , int y, int id, int type, int teamNum );
	Player();

	int searchEnemy(Player* allPlayers , int maxPlayers);
	int searchToHelp(Player* allPlayers, int maxPlayers);
	//void checkHealth();
	void escape();
	void ammoRefill();
	void improvingHealthPoints();

	void fire();
	void move();
	void heal(int points);

	void setPosition(int x, int y);
	void setPlayer(int id,int  type, int teamNum);

	int getType() { return type; };
	int getRow() { return row; };
	int getCol() { return col; };
	int getId() { return id; };
	int getTeamNum() { return teamNum; };
	int getHealthPoints() { return healthPoints; };
	int getNumOfBullets() { return numOfBullets; };
	int getNumOfGranades() { return numOfGranades; };
};

