#include "Move.h"

Move::Move()
{
	piece = nullptr;
	captured = nullptr;
	isPromoting = false;
	isCapture = false;
	isEP = false;
	oldPassantSquare = -1;
	oldPassantPieceSquare = -1;
	isCastle = false;
	oldCastlingRights = 0;
	//lossOfEP = false;
	//lostEP = nullptr;
	promoPiece = 0;
	oldHalfmoves = -1;
	oldX = -1;
	oldY = -1;
	newX = -1;
	newY = -1;
	//fen = "";
}
Move::Move(Piece* p, Piece* cap, bool isPromo, bool isCap, bool ispassant, int oldPasSq, int oldPasP, bool isCas, uint8_t oldCas,
	/*bool lossPass, Piece* lostPass,*/ uint8_t proPiece, int oldHalves, int oldx, int oldy, int newx, int newy/*, std::string FEN */) :
	piece(p),
	captured(cap),
	isPromoting(isPromo),
	isCapture(isCap),
	isEP(ispassant),
	oldPassantSquare(oldPasSq),
	oldPassantPieceSquare(oldPasP),
	isCastle(isCas),
	oldCastlingRights(oldCas),
	//lossOfEP(lossPass),
	//lostEP(lostPass),
	promoPiece(proPiece),
	oldHalfmoves(oldHalves),
	oldX(oldx),
	oldY(oldy),
	newX(newx),
	newY(newy)
{
	/*
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
	//fen = FEN;
	*/
}
Move::~Move()
{
	;
}

Move* Move::cloneMove()
{
	Move* newMove = new Move(piece, captured, isPromoting, isCapture, isEP, 
		oldPassantSquare, oldPassantPieceSquare, isCastle, oldCastlingRights, /*lossOfEP, lostEP,*/ promoPiece,
		oldHalfmoves, oldX, oldY, newX, newY);
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

// on the basis of MVV LVA for move ordering.
bool Move::operator< (const Move* m)
{
	int value1, value2, attack1, attack2, vic1, vic2;

	// get first move's attacker index
	if (piece->info & KING)
	{
		attack1 = 5;
	}
	else if (piece->info & PAWN)
	{
		attack1 = 4;
	}
	else if (piece->info & KNIGHT)
	{
		attack1 = 3;
	}
	else if (piece->info & BISHOP)
	{
		attack1 = 2;
	}
	else if (piece->info & ROOK)
	{
		attack1 = 1;
	}
	else if (piece->info & QUEEN)
	{
		attack1 = 0;
	}

	// get second move's attacker index
	if (m->piece->info & KING)
	{
		attack1 = 5;
	}
	else if (m->piece->info & PAWN)
	{
		attack1 = 4;
	}
	else if (m->piece->info & KNIGHT)
	{
		attack1 = 3;
	}
	else if (m->piece->info & BISHOP)
	{
		attack1 = 2;
	}
	else if (m->piece->info & ROOK)
	{
		attack1 = 1;
	}
	else if (m->piece->info & QUEEN)
	{
		attack1 = 0;
	}

	// get first move's victim index
	if (captured)
	{
		if (captured->info & PAWN)
		{
			vic1 = 4;
		}
		else if (captured->info & KNIGHT)
		{
			vic1 = 3;
		}
		else if (captured->info & BISHOP)
		{
			vic1 = 2;
		}
		else if (captured->info & ROOK)
		{
			vic1 = 1;
		}
		else if (captured->info & QUEEN)
		{
			vic1 = 0;
		}
	}
	else vic1 = 5;

	// get second move's victim index
	if (m->captured)
	{
		if (m->captured->info & PAWN)
		{
			vic2 = 4;
		}
		else if (m->captured->info & KNIGHT)
		{
			vic2 = 3;
		}
		else if (m->captured->info & BISHOP)
		{
			vic2 = 2;
		}
		else if (m->captured->info & ROOK)
		{
			vic2 = 1;
		}
		else if (m->captured->info & QUEEN)
		{
			vic2 = 0;
		}
	}
	else vic2 = 5;

	// now, get capture values from table
	value1 = MVVLVA_TABLE[vic1][attack1], value2 = MVVLVA_TABLE[vic2][attack2];
	return (value1 > value2); // if the first value is greater, the first move should come before the other
}