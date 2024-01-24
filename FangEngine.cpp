#include "FangEngine.h"

FangEngine::FangEngine()
{
	playing = true;
}

FangEngine::FangEngine(Game* g)
{
	playing = true;
	game = g;
}

// uses Minimax algorithm
Move* FangEngine::search()
{
	std::vector<Move*> moves;
	game->generateLegalMoves(moves);
	return nullptr;
}

int FangEngine::eval()
{
	return 0;
}