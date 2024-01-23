#include "Piece.h"
Piece::Piece()
{
	rect = nullptr;
	info = 0;
	enPassantable = false;
	canCastle = true;
}
Piece::Piece(SDL_Rect* rectIn, uint8_t infoIn, int side = -1)
{
	rect = rectIn;
	info = infoIn;
	enPassantable = false;
	canCastle = true;
	rookSide = side;
}
Piece::~Piece() {}