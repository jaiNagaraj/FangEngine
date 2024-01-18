#include "Move.h"

Move::Move() {}
Move::Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, bool isCas, bool lossCas,
	bool lossPass, Piece* lostPass, uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy, std::string FEN)
{
	piece = p;
	captured = cap;
	isPromoting = isPromo;
	isCapture = isCap;
	isEP = ispassant;
	isCastle = isCas;
	lossOfCastle = lossCas;
	lossOfEP = lossPass;
	lostEP = lostPass;
	promoPiece = proPiece;
	oldHalfmoves = oldHalves;
	oldX = oldx;
	oldY = oldy;
	newX = newx;
	newY = newy;
	fen = FEN;
}
Move* Move::cloneMove()
{
	Move* newMove = new Move(piece, captured, isPromoting, isCapture, isEP, 
		isCastle, lossOfCastle, lossOfEP, lostEP, promoPiece, oldHalfmoves,
		oldX, oldY, newX, newY, fen);
	return newMove;
}