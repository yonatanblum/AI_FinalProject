#pragma once
#include "Granade.h"
#include "Bullet.h"
#include "Storage.h"
#include "Definitions.h"


class Player
{

private: 
	int id; 
	int type; // type can be attacker=0 or squire =1
	int healthPoints;
	int teamNum; 
	int numOfBullets;
	int numOfGranades;
	int numOfMedicine;
	int row, col;
	Granade* granades[MAX_GRANADES];
	Bullet* bullets[NUM_BULLETS];
	Bullet* bullet;													//////
	Granade* granade;												//////
	int mode; // mode can be survival=1 or attack=0 


public :
	Player(int x , int y, int id, int type, int teamNum );
	Player();

	int searchEnemy(Player* allPlayers , int maxPlayers);
	int searchToHelp(Player* allPlayers, int maxPlayers);
	int searchStorage(Storage* allStorage, int maxStorage);

	void getAmmoFromStorage(Storage* allStorage, int id);
	void getMedFromStorage(Storage* allStorage, int id);
	//void checkHealth();
	void escape();
	void ammoRefill();
	void improvingHealthPoints();

	void move();
	void heal(int points);
	void healPlayer(int hp);

	void setPosition(int x, int y);
	void setPlayer(int id,int  type, int teamNum);
	void setHealthPoints(int hp) { healthPoints = hp; };
	void setRow(int newRow) { row = newRow; };
	void setCol(int newCol) { col = newCol; };
	int getType() { return type; };
	int getRow() { return row; };
	int getCol() { return col; };	
	int getId() { return id; };
	int getTeamNum() { return teamNum; };
	int getHealthPoints() { return healthPoints; };
	int getNumOfBullets() { return numOfBullets; };
	int getNumOfGranades() { return numOfGranades; };
	int getNumOfMedicine() { return numOfMedicine; };
	
	void attack(int maze[MSZ][MSZ], double map[MSZ][MSZ],Player* allPlayers, int index,double angle,double dist);		//////
	void isHurt(int distOfShot);
	bool isAlive() { return healthPoints > 0; };
	bool isEmpty();
	void printPlayer();
};

