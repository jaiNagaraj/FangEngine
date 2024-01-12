#include "Move.h"

Move::Move() {}
Move::Move(Piece* p, Piece* cap, bool isCap, bool ispassant, int oldx, int oldy, int newx, int newy)
{
	piece = p;
	captured = cap;
	isCapture = isCap;
	isEP = ispassant;
	oldX = oldx;
	oldY = oldy;
	newX = newx;
	newY = newy;
}