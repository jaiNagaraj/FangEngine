#pragma once
#include "Player.h"
class FangEngine :
    public Player
{
    public:
        FangEngine();
        FangEngine(Game* g);
        ~FangEngine();

        Move* search(int depth);
        int minimax(int depth, bool maxer, int alpha, int beta);
        int eval();
};

