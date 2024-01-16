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
		bool lossOfEP;
		bool lossOfCastle;
		uint8_t promoPiece; // the piece being promoted to

		// the old and new move coordinates
		int oldX;
		int oldY;
		int newX;
		int newY;

		Move();
		Move(Piece* p, Piece* cap, bool isCap, bool ispassant, int oldx, int oldy, int newx, int newy);
};