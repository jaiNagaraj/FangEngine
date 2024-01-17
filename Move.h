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
		bool isCastle;
		bool lossOfCastle;
		bool lossOfEP;
		Piece* lostEP; // piece that lost en passant
		uint8_t promoPiece; // the piece being promoted to
		int oldHalfmoves;

		// the old and new move coordinates
		int oldX;
		int oldY;
		int newX;
		int newY;

		Move();
		Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, bool isCas, bool lossCas,
			bool lossPass, Piece* lostPass, uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy);
		Move* cloneMove();
};