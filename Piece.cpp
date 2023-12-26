#include "Piece.h"
Piece::Piece()
{
	rect = nullptr;
	info = 0;
	enPassantable = false;
	canCastle = true;
}
Piece::Piece(SDL_Rect* rectIn, uint8_t infoIn)
{
	rect = rectIn;
	info = infoIn;
	enPassantable = false;
	canCastle = true;
}
Piece::~Piece() {}