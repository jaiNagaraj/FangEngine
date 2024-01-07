#pragma once
#include <vector>
#include "Player.h"
#include "Piece.h"
class Game
{
	private:
		;
	public:
		std::vector<Piece*> piecesOnBoard;
		Player player1, player2;
		uint8_t board[8][8] = {0};
		int turn = 0;

		bool isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY);
		bool validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test = false);
		bool validCastle(Piece* piece, int initX, int initY, int kingX, int kingY);
		int isCheckmate(int turn);
		void printBoard();
};