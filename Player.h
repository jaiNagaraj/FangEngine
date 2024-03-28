#pragma once
#include "Game.h"
class Player
{
	public:
		Game* game; // should share same game as Window
		std::vector<Move*> moves;
		bool playing = false;

		Player();
		~Player();

		Move* search(int depth);
		int eval();
};

