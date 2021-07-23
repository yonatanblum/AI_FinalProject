#include "Player.h"


Player::Player(int id, int type)
{
	id=id;
	type = type; // type can be attacker=0 or squire =1
	healthPoints = MAX_HEALTH;
	
	for (int i= 0; i < MAX_GRANADES ;i++)
	{
		granades[i] = new Granade(0,0); //TODO need to change x and y to the player position or to target shot
	}

	for (int i = 0; i < NUM_PLAYER_BULLETS; i++)
	{
		bullets[i] = new Bullet(0, 0); //TODO need to change x and y to the player position or to target shot
	}

	mode=0; // mode can be survival=1 or attack=0 
	// Team * team;
}

