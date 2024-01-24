#include "FangEngine.h"

// uses Minimax algorithm
Move* Player::search()
{
	std::vector<Move*> moves;
	game->generateLegalMoves(moves);
}

int Player::eval()
{
	;
}