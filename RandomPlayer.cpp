#include "RandomPlayer.h"
#include <random>

RandomPlayer::RandomPlayer()
{
	playing = true;
}

RandomPlayer::RandomPlayer(Game* g)
{
	playing = true;
	game = g;
}

RandomPlayer::~RandomPlayer()
{
	//delete game;
}

Move* RandomPlayer::search(int depth)
{
	for (auto p : moves)
	{
		delete p;
	}
	moves.clear();
	game->generateLegalMoves(moves);
	if (moves.size() == 0) return nullptr;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d1(0, (int)(moves.size() - 1));
	int index = d1(gen);
	return moves[index];
}