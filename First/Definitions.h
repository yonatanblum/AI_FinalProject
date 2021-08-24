#pragma once
const int MSZ = 120;
const int W = 600;
const int H = 600;

// maze : // 
const int WALL = 1;
const int SPACE = 0;
const int START = 2;
const int TARGET = 3;
const int BLACK = 4; // visited
const int GRAY = 5;
const int PATH = 6;
const int PLAYER1 = 7; 
const int PLAYER2 = 8;
const int AMMO_STORE = 9;
const int MEDICINE_STORE = 10;
const int BULLET = 11;													


const int MAX_RANGE_GRANADE = 15;
const int MAX_RANGE_BULLET = 30;

const int NUM_ROOMS = 12;
const int NUM_TEAM_PLAYERS = 5;
const int STORE_IN_ROOM = 1;
const int MAX_RANGE_ATTACK = 30;
const int MIN_RANGE_ATTACK = 11;

const int MAX_HEALTH = 100;
const int MAX_GRANADES = 10;
const int NUM_PLAYER_BULLETS = 100;


class Definitions
{
};

