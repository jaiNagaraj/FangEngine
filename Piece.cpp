#include "Piece.h"
Piece::Piece()
{
	rect = nullptr;
	info = 0;
	enPassantable = false;
}
Piece::Piece(SDL_Rect* rectIn, uint8_t infoIn)
{
	rect = rectIn;
	info = infoIn;
	enPassantable = false;
}
Piece::~Piece()
{
	delete rect;
}