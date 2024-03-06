#pragma once
#include <vector>
#include <unordered_map>
#include "Move.h"
//#include "Tables.h"

#define WK_CASTLE 0b10000000
#define WQR_CASTLE 0b01000000
#define WKR_CASTLE 0b00100000
#define BK_CASTLE 0b00000100
#define BQR_CASTLE 0b00000010
#define BKR_CASTLE 0b00000001

#define WP_INDEX 0
#define WN_INDEX 1
#define WB_INDEX 2
#define WR_INDEX 3
#define WQ_INDEX 4
#define WK_INDEX 5
#define BP_INDEX 6
#define BN_INDEX 7
#define BB_INDEX 8
#define BR_INDEX 9
#define BQ_INDEX 10
#define BK_INDEX 11

typedef unsigned long long ull;

class Game
{
	private:
		;
	public:
		std::vector<Piece*> piecesOnBoard;

		uint8_t board[8][8] = {0};

		/*
		* Array of piece bitboards. Indices defined by macros above.
		*/
		uint64_t pieceBoards[12] = { 0 };

		// keep track of EP info. The first element is the square, and the second element is the captured piece square.
		int enPassantInfo[2];

		// List of current legal moves. Useful for debugging.
		std::vector<Move*> legalMoveList;

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

		int bitToIndex(uint64_t);
		Piece* makePiece(int x, int y, uint8_t info);
		bool isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY);
		Move* validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test = false);
		bool validCastle(Piece* piece, int initX, int initY, int kingX, int kingY);
		void makeMove(Move* move);
		void unmakeMove(Move* move);
		std::vector<Move*> bitsToMoves(uint64_t bitboard, unsigned long startSquare, uint8_t pieceType);
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