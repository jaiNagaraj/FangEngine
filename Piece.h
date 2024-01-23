#pragma once
#include <SDL.h>
#include <iostream>
#define WHITE_PAWN   0b10000001
#define WHITE_KNIGHT 0b10000010
#define WHITE_BISHOP 0b10000100
#define WHITE_ROOK	 0b10001000
#define WHITE_QUEEN  0b10010000
#define WHITE_KING   0b10100000
#define BLACK_PAWN   0b01000001
#define BLACK_KNIGHT 0b01000010
#define BLACK_BISHOP 0b01000100
#define BLACK_ROOK	 0b01001000
#define BLACK_QUEEN  0b01010000
#define BLACK_KING   0b01100000
#define PAWN         0b00000001
#define KNIGHT       0b00000010
#define BISHOP       0b00000100
#define ROOK         0b00001000
#define QUEEN        0b00010000
#define KING         0b00100000
#define BLACK        0b01000000
#define WHITE        0b10000000
using namespace std;
class Piece
{
	private:
		;
	public:
		// gives position/area of piece
		SDL_Rect* rect;	

		/*
			8-bit sequence for piece data. It looks like this:
			w b k q r b n p
		*/
		uint8_t info;
		bool enPassantable;
		bool canCastle;
		int rookSide = -1; // 0 for queen, 1 for king, -1 otherwise
		Piece();
		Piece(SDL_Rect*, uint8_t, int);
		~Piece();
};

