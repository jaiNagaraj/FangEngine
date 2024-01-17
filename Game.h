#pragma once
#include <vector>
#include <unordered_map>
#include "Player.h"
#include "Piece.h"
#include "Move.h"
typedef unsigned long long ull;

class Game
{
	private:
		;
	public:
		std::vector<Piece*> piecesOnBoard;
		Player player1, player2;
		uint8_t board[8][8] = {0};
		std::unordered_map<std::string,int> positions;
		std::vector<std::string> fens;
		int turn = 0;
		int halfmoves = 0;

		Piece* makePiece(int x, int y, uint8_t info, int side = -1);
		bool isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY);
		Move* validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test = false);
		bool validCastle(Piece* piece, int initX, int initY, int kingX, int kingY);
		void makeMove(Move* move);
		void unmakeMove(Move* move);
		ull generateLegalMoves(std::vector<Move*>& moves);
		int isCheckmate(int turn);
		bool insufficientMaterial();
		std::string getFEN();
		void printBoard();
		ull perft(int depth /* assuming >= 1 */);
		ull perftCaps(int depth, bool hasCapture);
};