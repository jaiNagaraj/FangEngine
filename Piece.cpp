#include "Piece.h"
Piece::Piece()
{
	rect = nullptr;
	info = 0;
}
Piece::Piece(SDL_Rect* rectIn, uint8_t infoIn)
{
	rect = rectIn;
	info = infoIn;
}
Piece::~Piece() {}