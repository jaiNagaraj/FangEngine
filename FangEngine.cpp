#include "FangEngine.h"
#define INF INT_MAX

FangEngine::FangEngine()
{
	playing = true;
}

FangEngine::FangEngine(Game* g)
{
	playing = true;
	game = g;
}

// calls Minimax algorithm
Move* FangEngine::search(int depth)
{
	int evaluation = minimax(depth, (game->turn % 2 == 0), -INF, INF);
	std::vector<Move*> moves;
	game->generateLegalMoves(moves);
	return nullptr;
}

// uses alpha-beta pruning
int FangEngine::minimax(int depth, bool maxer, int alpha, int beta)
{
	std::vector<Move*> moves;
	game->generateLegalMoves(moves);
	if (depth == 0 || moves.size() == 0)
	{
		return eval();
	}
	if (maxer) // white maximizes
	{
		int maxEval, eval;
		maxEval = -INF; // worst possible eval for white
		for (auto move : moves)
		{
			game->makeMove(move);
			eval = minimax(depth - 1, false, alpha, beta);
			maxEval = max(maxEval, eval);
			alpha = max(alpha, eval); // change lower bound if necessary
			game->unmakeMove(move);
			if (beta <= alpha) break; // black will never go down this branch; has better option
		}
		return maxEval;
	}
	else // black minimizes
	{
		int minEval, eval;
		minEval = INF; // worst possible eval for black
		for (auto move : moves)
		{
			game->makeMove(move);
			eval = minimax(depth - 1, true, alpha, beta);
			minEval = min(minEval, eval);
			beta = min(beta, eval); // change upper bound if necessary
			game->unmakeMove(move);
			if (beta <= alpha) break; // black will never go down this branch; has better option
		}
		return minEval;
	}
}

int FangEngine::eval()
{
	return 0;
}