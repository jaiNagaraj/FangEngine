#pragma once
#include "Piece.h"
class Move
{
	private:
		;
	public:
		Piece* piece; // the piece that's moving
		Piece* captured; // the piece captured
		bool isPromoting;
		bool isCapture;
		bool isEP; // en passant capture
		int oldPassantSquare;
		int oldPassantPieceSquare;
		bool isCastle;
		uint8_t oldCastlingRights;
		// bool lossOfEP;
		// Piece* lostEP; // piece that lost en passant
		uint8_t promoPiece; // the piece being promoted to
		int oldHalfmoves;
		//std::string fen;

		// the old and new move coordinates
		int oldX;
		int oldY;
		int newX;
		int newY;

		// table for MVV-LVA ordering
		int MVVLVA_TABLE[6][6] =
		{
			{ 50, 51, 52, 53, 54, 55 }, // victim Q, attacker K, Q, R, B, N, P
			{ 40, 41, 42, 43, 44, 45 }, // victim R, attacker K, Q, R, B, N, P
			{ 30, 31, 32, 33, 34, 35 }, // victim B, attacker K, Q, R, B, N, P
			{ 20, 21, 22, 23, 24, 25 }, // victim N, attacker K, Q, R, B, N, P
			{ 10, 11, 12, 13, 14, 15 }, // victim P, attacker K, Q, R, B, N, P
			{  0,  0,  0,  0,  0,  0 }, // victim n/a, attacker K, Q, R, B, N, P
		};

		Move();
		Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, int oldPasSq, int oldPasP, bool isCas, uint8_t oldCas,
			/*bool lossPass, Piece* lostPass,*/ uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy/*, std::string FEN */);
		~Move();
		Move* cloneMove();
		void printMove();
		bool operator< (const Move* m);
};