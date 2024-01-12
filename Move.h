#pragma once
#include "Piece.h"
class Move
{
	private:
		;
	public:
		Piece* piece; // the piece that's moving
		Piece* captured; // the piece captured
		bool isCapture;
		bool isEP; // en passant capture
		int oldX;
		int oldY;
		int newX;
		int newY;

		Move();
		Move(Piece* p, Piece* cap, bool isCap, bool ispassant, int oldx, int oldy, int newx, int newy);
};