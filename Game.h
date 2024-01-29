#pragma once
#include <vector>
#include <unordered_map>
#include "Piece.h"
#include "Move.h"

#define WK_CASTLE 0b10000000
#define WQR_CASTLE 0b01000000
#define WKR_CASTLE 0b00100000
#define BK_CASTLE 0b00000100
#define BQR_CASTLE 0b00000010
#define BKR_CASTLE 0b00000001
typedef unsigned long long ull;

class Game
{
	private:
		;
	public:
		std::vector<Piece*> piecesOnBoard;
		//Player player1, player2;
		uint8_t board[8][8] = {0};
		int whiteAttack[8][8] = { 0 }; // the squares white controls
		int blackAttack[8][8] = { 0 }; // the squares black controls
		std::unordered_map<std::string,int> positions;
		std::vector<std::string> fens;
		int turn = 0;
		int halfmoves = 0;
		bool whiteKingsideRookCanCastle = true;
		bool whiteQueensideRookCanCastle = true;
		bool whiteKingCanCastle = true;
		bool blackKingsideRookCanCastle = true;
		bool blackQueensideRookCanCastle = true;
		bool blackKingCanCastle = true;

		Piece* makePiece(int x, int y, uint8_t info, int side = -1);
		bool isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY);
		void updateAttackBoard();
		Move* validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test = false);
		bool validCastle(Piece* piece, int initX, int initY, int kingX, int kingY);
		void makeMove(Move* move);
		void unmakeMove(Move* move);
		ull generateLegalMoves(std::vector<Move*>& moves);
		int isCheckmate(int turn);
		int oldIsCheckmate(int turn);
		bool insufficientMaterial();
		std::string getFEN();
		void buildFromFEN(std::string fen);
		void printBoard();
		ull perft(int depth /* assuming >= 1 */);
		ull perftCaps(int depth, bool hasCapture);

		~Game();
};