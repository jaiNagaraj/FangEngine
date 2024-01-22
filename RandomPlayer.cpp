#include "RandomPlayer.h"
#include <random>

RandomPlayer::RandomPlayer()
{
	playing = true;
}

Move* Player::search()
{
	for (auto p : moves)
	{
		delete p;
	}
	moves.clear();
	game->generateLegalMoves(moves);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d1(1, moves.size());
	int index = d1(gen);
	return moves[index];
}