#pragma once
#include "Player.h"
class RandomPlayer :
    public Player
{
    public:
        RandomPlayer();
        RandomPlayer(Game* g);
        ~RandomPlayer();
        Move* search(int depth);
};

