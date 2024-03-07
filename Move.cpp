#include "Move.h"

Move::Move() {}
Move::Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, int oldPasSq, int oldPasP, bool isCas, uint8_t oldCas,
	bool lossPass, Piece* lostPass, uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy, std::string FEN)
{
	piece = p;
	captured = cap;
	isPromoting = isPromo;
	isCapture = isCap;
	isEP = ispassant;
	oldPassantSquare = oldPasSq;
	oldPassantPieceSquare = oldPasP;
	isCastle = isCas;
	oldCastlingRights = oldCas;
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
Move::~Move()
{
	;
}
Move* Move::cloneMove()
{
	Move* newMove = new Move(piece, captured, isPromoting, isCapture, isEP, 
		oldPassantSquare, oldPassantPieceSquare, isCastle, oldCastlingRights, lossOfEP, lostEP, promoPiece, 
		oldHalfmoves, oldX, oldY, newX, newY, fen);
	return newMove;
}

void Move::printMove()
{
	std::cout << "Piece: ";
	if (!piece) std::cout << "Not defined\n";
	else
	{
		switch (piece->info)
		{
			case WHITE_PAWN:
				std::cout << "White pawn\n";
				break;
			case WHITE_KNIGHT:
				std::cout << "White knight\n";
				break;
			case WHITE_BISHOP:
				std::cout << "White bishop\n";
				break;
			case WHITE_ROOK:
				std::cout << "White rook\n";
				break;
			case WHITE_QUEEN:
				std::cout << "White queen\n";
				break;
			case WHITE_KING:
				std::cout << "White king\n";
				break;
			case BLACK_PAWN:
				std::cout << "Black pawn\n";
				break;
			case BLACK_KNIGHT:
				std::cout << "Black knight\n";
				break;
			case BLACK_BISHOP:
				std::cout << "Black bishop\n";
				break;
			case BLACK_ROOK:
				std::cout << "Black rook\n";
				break;
			case BLACK_QUEEN:
				std::cout << "Black queen\n";
				break;
			case BLACK_KING:
				std::cout << "Black king\n";
				break;
		}
	}

	int oldIndex = 8 * (7 - oldY) + oldX;
	char oldFile = (oldIndex % 8) + 'a';
	int oldRank = (oldIndex / 8) + 1;
	int newIndex = 8 * (7 - newY) + newX;
	char newFile = (newIndex % 8) + 'a';
	int newRank = (newIndex / 8) + 1;
	std::cout << "Moving from " << oldFile << oldRank << " to " << newFile << newRank << '\n';
	
	std::cout << "Is capture: " << isCapture << '\n' << "	Captured piece: ";
	if (isCapture)
	{
		switch (captured->info)
		{
		case WHITE_PAWN:
			std::cout << "White pawn\n";
			break;
		case WHITE_KNIGHT:
			std::cout << "White knight\n";
			break;
		case WHITE_BISHOP:
			std::cout << "White bishop\n";
			break;
		case WHITE_ROOK:
			std::cout << "White rook\n";
			break;
		case WHITE_QUEEN:
			std::cout << "White queen\n";
			break;
		case WHITE_KING:
			std::cout << "White king\n";
			break;
		case BLACK_PAWN:
			std::cout << "Black pawn\n";
			break;
		case BLACK_KNIGHT:
			std::cout << "Black knight\n";
			break;
		case BLACK_BISHOP:
			std::cout << "Black bishop\n";
			break;
		case BLACK_ROOK:
			std::cout << "Black rook\n";
			break;
		case BLACK_QUEEN:
			std::cout << "Black queen\n";
			break;
		case BLACK_KING:
			std::cout << "Black king\n";
			break;
		}
	}
	else std::cout << "N/A\n";

	std::cout << "Is promoting: " << isPromoting << '\n' << "	Promoting to: ";
	if (isPromoting)
	{
		switch (promoPiece)
		{
		case WHITE_PAWN:
			std::cout << "White pawn\n";
			break;
		case WHITE_KNIGHT:
			std::cout << "White knight\n";
			break;
		case WHITE_BISHOP:
			std::cout << "White bishop\n";
			break;
		case WHITE_ROOK:
			std::cout << "White rook\n";
			break;
		case WHITE_QUEEN:
			std::cout << "White queen\n";
			break;
		case WHITE_KING:
			std::cout << "White king\n";
			break;
		case BLACK_PAWN:
			std::cout << "Black pawn\n";
			break;
		case BLACK_KNIGHT:
			std::cout << "Black knight\n";
			break;
		case BLACK_BISHOP:
			std::cout << "Black bishop\n";
			break;
		case BLACK_ROOK:
			std::cout << "Black rook\n";
			break;
		case BLACK_QUEEN:
			std::cout << "Black queen\n";
			break;
		case BLACK_KING:
			std::cout << "Black king\n";
			break;
		}
	}
	else std::cout << "N/A\n";

	std::cout << "Is castle: " << isCastle << '\n';
	std::cout << "Is en passant: " << isEP << "\n\n\n";
}