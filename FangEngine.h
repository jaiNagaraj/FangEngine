#pragma once
#include "Player.h"
class FangEngine :
    public Player
{
    public:
        FangEngine();
        FangEngine(Game* g);
        ~FangEngine();

        Move* search();
        int eval();
};

