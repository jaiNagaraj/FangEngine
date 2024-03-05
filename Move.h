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
		int oldPassantSquare;
		int oldPassantPieceSquare;
		bool isCastle;
		uint8_t oldCastlingRights;
		bool lossOfEP;
		Piece* lostEP; // piece that lost en passant
		uint8_t promoPiece; // the piece being promoted to
		int oldHalfmoves;
		std::string fen;

		// the old and new move coordinates
		int oldX;
		int oldY;
		int newX;
		int newY;

		Move();
		Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, int oldPasSq, int oldPasP, bool isCas, uint8_t oldCas,
			bool lossPass, Piece* lostPass, uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy, std::string FEN);
		~Move();
		Move* cloneMove();
};