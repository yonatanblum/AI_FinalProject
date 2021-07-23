#include "Player.h"


Player::Player(int x, int y ,int id, int type , int teamNum)
{
	this->id=id;
	this->type = type; // type can be attacker=0 or squire =1
	this->healthPoints = MAX_HEALTH;
	this->teamNum = teamNum;
	this->x = x;
	this->y = y;

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

