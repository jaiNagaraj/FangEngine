#include "Game.h"
#include "Tables.h"
#include <bitset>
#include <string>
#include <sstream>
#include <algorithm>
typedef unsigned long long ull;

int Game::bitToIndex(uint64_t num)
{
	return (int) std::log2(num);
}

Piece* Game::makePiece(int x, int y, uint8_t info)
{
	// update respective bitboard
	int index = 8 * (7 - y) + x;
	switch (info)
	{
		case WHITE_PAWN:
			pieceBoards[WP_INDEX] |= (1ULL << index);
			break;
		case WHITE_KNIGHT:
			pieceBoards[WN_INDEX] |= (1ULL << index);
			break;
		case WHITE_BISHOP:
			pieceBoards[WB_INDEX] |= (1ULL << index);
			break;
		case WHITE_ROOK:
			pieceBoards[WR_INDEX] |= (1ULL << index);
			break;
		case WHITE_QUEEN:
			pieceBoards[WQ_INDEX] |= (1ULL << index);
			break;
		case WHITE_KING:
			pieceBoards[WK_INDEX] |= (1ULL << index);
			break;
		case BLACK_PAWN:
			pieceBoards[BP_INDEX] |= (1ULL << index);
			break;
		case BLACK_KNIGHT:
			pieceBoards[BN_INDEX] |= (1ULL << index);
			break;
		case BLACK_BISHOP:
			pieceBoards[BB_INDEX] |= (1ULL << index);
			break;
		case BLACK_ROOK:
			pieceBoards[BR_INDEX] |= (1ULL << index);
			break;
		case BLACK_QUEEN:
			pieceBoards[BQ_INDEX] |= (1ULL << index);
			break;
		case BLACK_KING:
			pieceBoards[BK_INDEX] |= (1ULL << index);
			break;
	}
	// set up graphic rect
	SDL_Rect* dstRect = new SDL_Rect;
	dstRect->x = x * 60;
	dstRect->y = y * 60;
	dstRect->w = 60;
	dstRect->h = 60;
	// make piece, add it to board!
	Piece* tmp = new Piece(dstRect, info);
	board[y][x] = tmp;
	piecesOnBoard.push_back(tmp);
	return tmp;
}

bool Game::isInCheck(int turn)
{
	uint64_t bitmoves = 0;
	uint64_t whitePieces = 0;
	uint64_t blackPieces = 0;
	uint64_t whiteNoKing = 0;
	uint64_t blackNoKing = 0;
	whitePieces |= pieceBoards[WP_INDEX] | pieceBoards[WN_INDEX] | pieceBoards[WB_INDEX] | pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX] | pieceBoards[WK_INDEX];
	blackPieces |= pieceBoards[BP_INDEX] | pieceBoards[BN_INDEX] | pieceBoards[BB_INDEX] | pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX] | pieceBoards[BK_INDEX];
	whiteNoKing = whitePieces & ~pieceBoards[WK_INDEX];
	blackNoKing = blackPieces & ~pieceBoards[BK_INDEX];
	int attackers = 0;
	// white
	if (turn % 2 == 0)
	{
		// get the king position
		unsigned long kingPos;
		if (!(_BitScanForward64(&kingPos, pieceBoards[WK_INDEX]))) std::cout << "WHITE KING NOT FOUND!!\n";
		// then, look for pieces in the king's POV
		
		// black pawn
		uint64_t blackAttackingPawns = 0;
		blackAttackingPawns |= whitePawnAttacks[kingPos] & pieceBoards[BP_INDEX];

		// black knight
		uint64_t blackAttackingKnights = 0;
		blackAttackingKnights |= knightMoves[kingPos] & pieceBoards[BN_INDEX];

		// diagonal rays (black bishop/queen)
		uint64_t blockers = whiteNoKing | blackPieces;
		uint64_t diagonalAttacks = 0;
		// iterate over all rays
		for (int i = 0; i < 8; i++)
		{
			if (i % 2 == 0) continue; // only diagonal rays allowed
			uint64_t maskedBlockers = rays[i][kingPos] & blockers;
			unsigned long i2;
			unsigned char c;
			if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
			else c = _BitScanForward64(&i2, maskedBlockers);

			if (c)
			{
				bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
			}
			else bitmoves = rays[i][kingPos]; // no blockers

			diagonalAttacks |= bitmoves;
		}
		diagonalAttacks &= (pieceBoards[BB_INDEX] | pieceBoards[BQ_INDEX]);

		// adjacent rays (black rook/queen)
		blockers = whiteNoKing | blackPieces;
		uint64_t adjAttacks = 0;
		// iterate over all rays
		for (int i = 0; i < 8; i++)
		{
			if (i % 2 != 0) continue; // only adjacent rays allowed
			uint64_t maskedBlockers = rays[i][kingPos] & blockers;
			unsigned long i2;
			unsigned char c;
			if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
			else c = _BitScanForward64(&i2, maskedBlockers);

			if (c)
			{
				bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
			}
			else bitmoves = rays[i][kingPos]; // no blockers

			adjAttacks |= bitmoves;
		}
		adjAttacks &= (pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX]);

		// count number of attackers
		uint64_t attackBoard = blackAttackingKnights | blackAttackingPawns | diagonalAttacks | adjAttacks;
		attackers += std::bitset<64>(attackBoard).count();
	}
	// black
	else
	{
		// get the king position
		unsigned long kingPos;
		if (!_BitScanForward64(&kingPos, pieceBoards[BK_INDEX])) std::cout << "BLACK KING NOT FOUND!!\n";
		// then, look for pieces in the king's POV
		
		// white pawn
		uint64_t whiteAttackingPawns = 0;
		whiteAttackingPawns |= blackPawnAttacks[kingPos] & pieceBoards[WP_INDEX];

		// white knight
		uint64_t whiteAttackingKnights = 0;
		whiteAttackingKnights |= knightMoves[kingPos] & pieceBoards[WN_INDEX];

		// diagonal rays (white bishop/queen)
		uint64_t blockers = blackNoKing | whitePieces;
		uint64_t diagonalAttacks = 0;
		// iterate over all rays
		for (int i = 0; i < 8; i++)
		{
			if (i % 2 == 0) continue; // only diagonal rays allowed
			uint64_t maskedBlockers = rays[i][kingPos] & blockers;
			unsigned long i2;
			unsigned char c;
			if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
			else c = _BitScanForward64(&i2, maskedBlockers);

			if (c)
			{
				bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
			}
			else bitmoves = rays[i][kingPos]; // no blockers

			diagonalAttacks |= bitmoves;
		}
		diagonalAttacks &= (pieceBoards[WB_INDEX] | pieceBoards[WQ_INDEX]);

		// adjacent rays (white rook/queen)
		blockers = blackNoKing | whitePieces;
		uint64_t adjAttacks = 0;
		// iterate over all rays
		for (int i = 0; i < 8; i++)
		{
			if (i % 2 != 0) continue; // only adjacent rays allowed
			uint64_t maskedBlockers = rays[i][kingPos] & blockers;
			unsigned long i2;
			unsigned char c;
			if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
			else c = _BitScanForward64(&i2, maskedBlockers);

			if (c)
			{
				bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
			}
			else bitmoves = rays[i][kingPos]; // no blockers

			adjAttacks |= bitmoves;
		}
		adjAttacks &= (pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX]);

		// count number of attackers
		uint64_t attackBoard = whiteAttackingKnights | whiteAttackingPawns | diagonalAttacks | adjAttacks;
		attackers += std::bitset<64>(attackBoard).count();
	}

	if (attackers > 0) return true;
	else return false;
}

/*
	Checks if a move is valid. If so, returns Move object, else returns null.
*/
Move* Game::validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test)
{
	int oldXCoord = oldX / 60, oldYCoord = oldY / 60, newXCoord = newX / 60, newYCoord = newY / 60;
	
	#ifdef USING_BITS
	
	int oldIndex = 8 * (7 - oldYCoord) + oldXCoord;
	int newIndex = 8 * (7 - newYCoord) + newXCoord;
	for (Move* m : legalMoveList)
	{
		if (m->oldX == oldXCoord && m->oldY == oldYCoord && m->newX == newXCoord && m->newY == newYCoord && m->piece->info == piece->info)
		{
			m->printMove();
			return m;
		}
	}
	return nullptr;

	#else

	bool isCapturing = false;
	bool isEP = false;
	bool isCastling = false;
	Piece* passantPiece = nullptr;

	// first, is the move on the board?
	if (newXCoord > 7 || newXCoord < 0 || newYCoord > 7 || newYCoord < 0) return nullptr;

	// if it's white's turn but black played
	if (turn % 2 == 0 && !(piece->info & WHITE)) return nullptr;
	// if it's black's turn but white played
	if (turn % 2 != 0 && !(piece->info & BLACK)) return nullptr;

	int distMovedX = newXCoord - oldXCoord;
	int distMovedY = newYCoord - oldYCoord;
	//std::cout << distMovedX << " " << distMovedY << '\n';

	/*
			********PAWN MOVES********
	*/

	if ((piece->info & WHITE_PAWN) == WHITE_PAWN)
	{
		// if the pawn moved impossibly
		if ((abs(distMovedX) > 1) || (distMovedY > -1) || (distMovedY < -2)) return nullptr;

		//if the pawn wants to capture
		if (abs(distMovedX) == 1)
		{
			// if it didn't move diagonally by one
			if (!(distMovedY == -1)) return nullptr;
			// if the capturing square does not hold a black piece
			if ((board[newYCoord][newXCoord] & BLACK) != BLACK)
			{
				// if there's a black pawn beside it, check for en passant
				if ((board[oldYCoord][newXCoord] & BLACK_PAWN) == BLACK_PAWN)
				{
					// iterate thru pieces to get piece on that square
					for (Piece* p : piecesOnBoard)
					{
						if (p->rect->x == newXCoord * 60 && p->rect->y == oldYCoord * 60)
						{
							// if that piece was enPassantable
							if (p->enPassantable)
							{
								isEP = true;
								passantPiece = p;
								break;
							}
							else return nullptr;
						}
					}
				}
				else return nullptr;
			}
			isCapturing = true;
		}

		// if the pawn moved one space
		else if (distMovedY == -1)
		{
			// if something is blocking the pawn
			if (!(board[newYCoord][newXCoord] == 0b00000000)) return nullptr;
		}
		// if the pawn moved two spaces
		else if (distMovedY == -2)
		{
			// if the pawn isn't on the starting rank
			if (oldYCoord != 6) return nullptr;
			// if something is blocking the pawn
			else if (!(board[newYCoord][newXCoord] == 0b00000000) || !(board[newYCoord + 1][newXCoord] == 0b00000000)) return nullptr;
		}
	}
	else if ((piece->info & BLACK_PAWN) == BLACK_PAWN)
	{
		// if the pawn moved impossibly
		if ((abs(distMovedX) > 1) || (distMovedY < 1) || (distMovedY > 2)) return nullptr;

		//if the pawn wants to capture
		if (abs(distMovedX) == 1)
		{
			// if it didn't move diagonally by one
			if (distMovedY != 1) return nullptr;
			// if the capturing square does not hold a white piece
			if ((board[newYCoord][newXCoord] & WHITE) != WHITE)
			{
				// if there's a white pawn beside it, check for en passant
				if ((board[oldYCoord][newXCoord] & WHITE_PAWN) == WHITE_PAWN)
				{
					// iterate thru pieces to get piece on that square
					for (Piece* p : piecesOnBoard)
					{
						if (p->rect->x == newXCoord * 60 && p->rect->y == oldYCoord * 60)
						{
							// if that piece was enPassantable
							if (p->enPassantable)
							{
								isEP = true;
								passantPiece = p;
								break;
							}
							else return nullptr;
						}
					}
				}
				else return nullptr;
			}
			isCapturing = true;
		}

		// if the pawn moved one space
		else if (distMovedY == 1)
		{
			// if something is blocking the pawn
			if (!(board[newYCoord][newXCoord] == 0b00000000)) return nullptr;
		}
		// if the pawn moved two spaces
		else if (distMovedY == 2)
		{
			// if the pawn isn't on the starting rank
			if (oldYCoord != 1) return nullptr;
			// if something is blocking the pawn
			else if (board[newYCoord][newXCoord] != 0b00000000 || board[newYCoord - 1][newXCoord] != 0b00000000) return nullptr;
		}
	}

	/*
			********KNIGHT MOVES********
	*/


	else if ((piece->info & KNIGHT) == KNIGHT)
	{
		// if horsey didnt move in L shape :(
		if (!((abs(distMovedX) == 2 && abs(distMovedY) == 1) || (abs(distMovedY) == 2 && abs(distMovedX) == 1))) return nullptr;

		//if the knight wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE) 
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return nullptr;
			isCapturing = true;
		}
	}

	/*
			********BISHOP MOVES********
	*/


	else if ((piece->info & BISHOP) == BISHOP)
	{
		// if bishop doesn't move diagonally
		if (abs(distMovedX) != abs(distMovedY)) return nullptr;

		//if the bishop wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return nullptr;
			isCapturing = true;
		}

		// check for any obstructions in bishop's path
		int tmpX = oldXCoord, tmpY = oldYCoord;
		tmpX += newXCoord > oldXCoord ? 1 : -1;
		tmpY += newYCoord > oldYCoord ? 1 : -1;
		while (tmpX != newXCoord && tmpY != newYCoord)
		{
			if (board[tmpY][tmpX] != 0b00000000) return nullptr;
			tmpX += newXCoord > oldXCoord ? 1 : -1;
			tmpY += newYCoord > oldYCoord ? 1 : -1;
		}
	}

	/*
			********ROOK MOVES********
	*/


	else if ((piece->info & ROOK) == ROOK)
	{
		// if rook doesn't move along rank/file
		if (!((abs(distMovedX) > 0 && abs(distMovedY) == 0) || (abs(distMovedY) > 0 && abs(distMovedX) == 0))) return nullptr;

		//if the rook wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return nullptr;
			isCapturing = true;
		}

		// check for any obstructions in rook's path
		int tmpX = oldXCoord, tmpY = oldYCoord;
		if (abs(distMovedX) > 0)
		{
			tmpX += newXCoord > oldXCoord ? 1 : -1;
			while (tmpX != newXCoord)
			{
				if (board[tmpY][tmpX] != 0b00000000) return nullptr;
				tmpX += newXCoord > oldXCoord ? 1 : -1;
			}
		}
		else
		{
			tmpY += newYCoord > oldYCoord ? 1 : -1;
			while (tmpY != newYCoord)
			{
				if (board[tmpY][tmpX] != 0b00000000) return nullptr;
				tmpY += newYCoord > oldYCoord ? 1 : -1;
			}
		}
	}

	/*
			********QUEEN MOVES********
	*/


	else if ((piece->info & QUEEN) == QUEEN)
	{
		//if the queen wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return nullptr;
			isCapturing = true;
		}

		// if queen moves like rook
		if ((abs(distMovedX) > 0 && abs(distMovedY) == 0) || (abs(distMovedY) > 0 && abs(distMovedX) == 0))
		{

			// check for any obstructions in queen's path
			int tmpX = oldXCoord, tmpY = oldYCoord;
			if (abs(distMovedX) > 0)
			{
				tmpX += newXCoord > oldXCoord ? 1 : -1;
				while (tmpX != newXCoord)
				{
					if (board[tmpY][tmpX] != 0b00000000) return nullptr;
					tmpX += newXCoord > oldXCoord ? 1 : -1;
				}
			}
			else
			{
				tmpY += newYCoord > oldYCoord ? 1 : -1;
				while (tmpY != newYCoord)
				{
					if (board[tmpY][tmpX] != 0b00000000) return nullptr;
					tmpY += newYCoord > oldYCoord ? 1 : -1;
				}
			}
		}
		// if queen moves like bishop
		else if (abs(distMovedX) == abs(distMovedY))
		{
			// check for any obstructions in bishop's path
			int tmpX = oldXCoord, tmpY = oldYCoord;
			tmpX += newXCoord > oldXCoord ? 1 : -1;
			tmpY += newYCoord > oldYCoord ? 1 : -1;
			while (tmpX != newXCoord && tmpY != newYCoord)
			{
				if (board[tmpY][tmpX] != 0b00000000) return nullptr;
				tmpX += newXCoord > oldXCoord ? 1 : -1;
				tmpY += newYCoord > oldYCoord ? 1 : -1;
			}
		}
		// if queen moves impossibly
		else return nullptr;
	}

	/*
			********KING MOVES********
	*/


	else if ((piece->info & KING) == KING)
	{
		// if king wants to castle
		if (abs(distMovedX) == 2 && distMovedY == 0)
		{
			if (validCastle(piece, oldXCoord, oldYCoord, newXCoord, newYCoord)) isCastling = true;
			else return nullptr;
		}
		// if the king moves illegally (more than a square)
		else if (abs(distMovedX) > 1 || abs(distMovedY) > 1) return nullptr;
		// if the king wants to capture
		else if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece OR if the piece is the king
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)
				|| ((board[newYCoord][newXCoord] & KING) == KING)) return nullptr;
			isCapturing = true;
		}
	}
	

	/*
		******MAKE SURE THE KING WON'T BE IN CHECK AFTER THE MOVE******
	*/
	if (!isCastling) // validCastle already looks for check
	{
		uint8_t tmp = board[newYCoord][newXCoord];
		uint8_t tmpEP;
		board[newYCoord][newXCoord] = board[oldYCoord][oldXCoord];
		board[oldYCoord][oldXCoord] = 0;
		if (isEP)
		{
			tmpEP = board[oldYCoord][newXCoord];
			board[oldYCoord][newXCoord] = 0;
		}

		int kingX, kingY;
		// for white
		if (turn % 2 == 0)
		{
			for (int i = 0; i < 8; i++)
			{
				bool breakout = false;
				for (int j = 0; j < 8; j++)
				{
					if ((board[i][j] & WHITE_KING) == WHITE_KING)
					{
						kingX = j;
						kingY = i;
						breakout = true;
						break;
					}
				}
				if (breakout) break;
			}
			if (isInCheck(board, 0, kingX, kingY))
			{
				// reset board state
				board[oldYCoord][oldXCoord] = board[newYCoord][newXCoord];
				board[newYCoord][newXCoord] = tmp;
				if (isEP) board[oldYCoord][newXCoord] = tmpEP;
				return nullptr;
			}
		}
		// for black
		else
		{
			for (int i = 0; i < 8; i++)
			{
				bool breakout = false;
				for (int j = 0; j < 8; j++)
				{
					if ((board[i][j] & BLACK_KING) == BLACK_KING)
					{
						kingX = j;
						kingY = i;
						breakout = true;
						break;
					}
				}
				if (breakout) break;
			}
			if (isInCheck(board, 1, kingX, kingY))
			{
				// reset board state
				board[oldYCoord][oldXCoord] = board[newYCoord][newXCoord];
				board[newYCoord][newXCoord] = tmp;
				if (isEP) board[oldYCoord][newXCoord] = tmpEP;
				return nullptr;
			}
		}

		// reset board state
		board[oldYCoord][oldXCoord] = board[newYCoord][newXCoord];
		board[newYCoord][newXCoord] = tmp;
		if (isEP) board[oldYCoord][newXCoord] = tmpEP;
	}


	// checks valid; create Move object
	Move* move = new Move;
	move->isCapture = isCapturing;
	move->isEP = isEP;
	move->isCastle = isCastling;
	move->piece = piece;
	move->oldHalfmoves = halfmoves;

	move->newX = newXCoord;
	move->newY = newYCoord;
	move->oldX = oldXCoord;
	move->oldY = oldYCoord;

	// record old castling rights
	uint8_t castlingRights = 0;
	if (whiteKingCanCastle) castlingRights |= WK_CASTLE;
	if (whiteQueensideRookCanCastle) castlingRights |= WQR_CASTLE;
	if (whiteKingsideRookCanCastle) castlingRights |= WKR_CASTLE;
	if (blackKingCanCastle) castlingRights |= BK_CASTLE;
	if (blackQueensideRookCanCastle) castlingRights |= BQR_CASTLE;
	if (blackKingsideRookCanCastle) castlingRights |= BKR_CASTLE;
	move->oldCastlingRights = castlingRights;

	// check for loss of en passant
	bool noLoss = true;
	for (Piece* p : piecesOnBoard)
	{
		if (p->enPassantable)
		{
			move->lossOfEP = true;
			move->lostEP = p;
			noLoss = false;
			break;
		}
	}
	if (noLoss)
	{
		move->lossOfEP = false;
		move->lostEP = nullptr;
	}

	// define capture state
	if (move->isCapture)
	{
		if (move->isEP) move->captured = passantPiece;
		else
		{
			for (Piece* p : piecesOnBoard)
			{
				if (p->rect->x == newXCoord * 60 && p->rect->y == newYCoord * 60 && p != piece) move->captured = p;
			}
			if (!(move->captured))
			{
				std::cout << "Unable to find captured piece in validMove!!!\n";
				std::cout << "Board:\n";
				printBoard();
				std::cout << "Piece movement: (" << oldXCoord << ", " << oldYCoord << ") to (" << newXCoord << ", " << newYCoord << ")\n";
				std::cout << "Turn: " << turn % 2 << '\n';
				std::cout << "Current FEN: " << getFEN() << '\n';
			}
		}
	}
	else move->captured = nullptr;

	if (move->captured && (move->captured->info & KING) == KING)
	{
		std::cout << "ALERT!! Capturing a king!\n";
	}

	// check for end-rank promotion in white
	if ((move->piece->info & WHITE_PAWN) == WHITE_PAWN && move->newY == 0)
	{
		move->isPromoting = true;
	}
	// check for end-rank promotion in black
	else if ((move->piece->info & BLACK_PAWN) == BLACK_PAWN && move->newY == 7)
	{
		move->isPromoting = true;
	}
	else move->isPromoting = false;

	// passed the gauntlet!
	return move;

	#endif
}

/*
	Function to execute a Move object and update game data
*/
void Game::makeMove(Move* move)
{
	int newIndex = 8 * (7 - move->newY) + move->newX;
	int oldIndex = 8 * (7 - move->oldY) + move->oldX;
	int distMovedX = move->newX - move->oldX;
	int distMovedY = move->newY - move->oldY;
	if (move->isCapture)
	{
		// update respective bitboard
		switch (move->captured->info)
		{
			case WHITE_PAWN:
				pieceBoards[WP_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_KNIGHT:
				pieceBoards[WN_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_BISHOP:
				pieceBoards[WB_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_ROOK:
				pieceBoards[WR_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_QUEEN:
				pieceBoards[WQ_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_PAWN:
				pieceBoards[BP_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_KNIGHT:
				pieceBoards[BN_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_BISHOP:
				pieceBoards[BB_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_ROOK:
				pieceBoards[BR_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_QUEEN:
				pieceBoards[BQ_INDEX] &= ~(1ULL << newIndex);
				break;
		}

		// use the combination of erase and remove to capture piece
		std::vector<Piece*>::iterator it = std::find(piecesOnBoard.begin(), piecesOnBoard.end(), move->captured);
		if (it != piecesOnBoard.end()) piecesOnBoard.erase(it);
		else std::cout << "Error: Could not capture piece!!!\n";
	}
	
	// if en passant, update board to reflect this
	if (move->isEP)
	{
		board[move->oldY][move->newX] = 0;
		// update respective bitboard
		if (move->piece->info & WHITE) pieceBoards[BP_INDEX] &= ~(1ULL << enPassantInfo[1]);
		else pieceBoards[WP_INDEX] &= ~(1ULL << enPassantInfo[1]);
	}

	if (move->isCastle)
	{
		// preemptively move rook to correct spot
		if ((move->piece->info & WHITE) == WHITE) // for white
		{
			whiteKingCanCastle = false; // revoke king castling rights
			if (move->newX < move->oldX) // queenside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the left rook
					if ((p->info & WHITE_ROOK) == WHITE_ROOK && p->rect->x == 0)
					{
						p->rect->x = (move->newX + 1) * 60; // move to the right of the king
						board[move->newY][move->newX + 1] = board[7][0];
						board[7][0] = 0;
						whiteQueensideRookCanCastle = false; // remove queenside castling
						// update bitboards
						pieceBoards[WR_INDEX] &= ~(1ULL << 0); // remove left rook from a1
						pieceBoards[WR_INDEX] |= (1ULL << 3); // add left rook to d1
						break;
					}
				}
			}
			else // kingside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the right rook
					if ((p->info & WHITE_ROOK) == WHITE_ROOK && p->rect->x == 420)
					{
						p->rect->x = (move->newX - 1) * 60; // move to the left of the king
						board[move->newY][move->newX - 1] = board[7][7];
						board[7][7] = 0;
						whiteKingsideRookCanCastle = false; // remove kingside castling
						// update bitboards
						pieceBoards[WR_INDEX] &= ~(1ULL << 7); // remove right rook from h1
						pieceBoards[WR_INDEX] |= (1ULL << 5); // add right rook to f1
						break;
					}
				}
			}
		}
		else // for black
		{
			blackKingCanCastle = false;
			if (move->newX < move->oldX) // queenside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the left rook
					if ((p->info & BLACK_ROOK) == BLACK_ROOK && p->rect->x == 0)
					{
						p->rect->x = (move->newX + 1) * 60; // move to the right of the king
						board[move->newY][move->newX + 1] = board[0][0];
						board[0][0] = 0;
						blackQueensideRookCanCastle = false; // revoke queenside castling
						// update bitboards
						pieceBoards[BR_INDEX] &= ~(1ULL << 56); // remove left rook from a8
						pieceBoards[BR_INDEX] |= (1ULL << 59); // add left rook to d8
						break;
					}
				}
			}
			else // kingside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the right rook
					if ((p->info & BLACK_ROOK) == BLACK_ROOK && p->rect->x == 420)
					{
						p->rect->x = (move->newX - 1) * 60; // move to the left of the king
						board[move->newY][move->newX - 1] = board[0][7];
						board[0][7] = 0;
						blackKingsideRookCanCastle = false; // revoke kingside castling
						// update bitboards
						pieceBoards[BR_INDEX] &= ~(1ULL << 63); // remove right rook from h8
						pieceBoards[BR_INDEX] |= (1ULL << 61); // add right rook to f8
						break;
					}
				}
			}
		}
	}
	else
	{
		// determine castling rights based on piece moved
		switch (move->piece->info)
		{
			case WHITE_KING:
				whiteKingCanCastle = false;
				break;
			case BLACK_KING:
				blackKingCanCastle = false;
				break;
			// NOTE: for rooks, it suffices to check their origin square rather than their side identity
			case WHITE_ROOK:
				if (move->oldX == 0) whiteQueensideRookCanCastle = false;
				else if (move->oldX == 7) whiteKingsideRookCanCastle = false;
				break;
			case BLACK_ROOK:
				if (move->oldX == 0) blackQueensideRookCanCastle = false;
				else if (move->oldX == 7) blackKingsideRookCanCastle = false;
				break;
		}
	}

	// reset all previously enPassantable pieces
	for (Piece* p : piecesOnBoard)
	{
		// if it is white's turn
		if (turn % 2 == 0 && (p->info & BLACK) == BLACK)
		{
			p->enPassantable = false;
		}
		// if it is black's turn
		if (turn % 2 == 1 && (p->info & WHITE) == WHITE)
		{
			p->enPassantable = false;
		}
	}
	// reset en passant info
	enPassantInfo[0] = -1;
	enPassantInfo[1] = -1;
	
	// if pawn passes checks for two spaces
	if ((move->piece->info & PAWN) == PAWN && abs(distMovedY) == 2)
	{
		move->piece->enPassantable = true;
		enPassantInfo[1] = newIndex;
		if (move->piece->info & WHITE) enPassantInfo[0] = 8 * (7 - (move->newY + 1)) + move->newX;
		else enPassantInfo[0] = 8 * (7 - (move->newY - 1)) + move->newX;
	}

	// if pawn move or capture, reset halfmove counter
	if (move->isCapture || (move->piece->info & PAWN) == PAWN) halfmoves = 0;
	else halfmoves++;


	// if promotion
	if (move->isPromoting)
	{
		// first, clear old pawn from respective bitboard
		if (move->piece->info & WHITE) pieceBoards[WP_INDEX] &= ~(1ULL << oldIndex);
		else pieceBoards[BP_INDEX] &= ~(1ULL << oldIndex);
		// update board and piece info
		move->piece->info = move->promoPiece;
	}

	// update board and turn
	board[move->newY][move->newX] = board[move->oldY][move->oldX];
	board[move->oldY][move->oldX] = 0;
	turn++;
	// update respective bitboard
	switch (move->piece->info)
	{
		case WHITE_PAWN:
			pieceBoards[WP_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WP_INDEX] |= (1ULL << newIndex);
			break;
		case WHITE_KNIGHT:
			pieceBoards[WN_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WN_INDEX] |= (1ULL << newIndex);
			break;
		case WHITE_BISHOP:
			pieceBoards[WB_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WB_INDEX] |= (1ULL << newIndex);
			break;
		case WHITE_ROOK:
			pieceBoards[WR_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WR_INDEX] |= (1ULL << newIndex);
			break;
		case WHITE_QUEEN:
			pieceBoards[WQ_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WQ_INDEX] |= (1ULL << newIndex);
			break;
		case WHITE_KING:
			pieceBoards[WK_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[WK_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_PAWN:
			pieceBoards[BP_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BP_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_KNIGHT:
			pieceBoards[BN_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BN_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_BISHOP:
			pieceBoards[BB_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BB_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_ROOK:
			pieceBoards[BR_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BR_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_QUEEN:
			pieceBoards[BQ_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BQ_INDEX] |= (1ULL << newIndex);
			break;
		case BLACK_KING:
			pieceBoards[BK_INDEX] &= ~(1ULL << oldIndex);
			pieceBoards[BK_INDEX] |= (1ULL << newIndex);
			break;
	}

	// update piece graphic coordinates
	move->piece->rect->x = move->newX * 60;
	move->piece->rect->y = move->newY * 60;
}

void Game::unmakeMove(Move* move)
{
	int newIndex = 8 * (7 - move->newY) + move->newX;
	int oldIndex = 8 * (7 - move->oldY) + move->oldX;
	int distMovedX = move->newX - move->oldX;
	int distMovedY = move->newY - move->oldY;
	if (move->isCapture)
	{
		// Add the captured piece back
		piecesOnBoard.push_back(move->captured);

		// update respective bitboard
		switch (move->captured->info)
		{
			case WHITE_PAWN:
				if (!(move->isEP)) pieceBoards[WP_INDEX] |= (1ULL << newIndex);
				break;
			case WHITE_KNIGHT:
				pieceBoards[WN_INDEX] |= (1ULL << newIndex);
				break;
			case WHITE_BISHOP:
				pieceBoards[WB_INDEX] |= (1ULL << newIndex);
				break;
			case WHITE_ROOK:
				pieceBoards[WR_INDEX] |= (1ULL << newIndex);
				break;
			case WHITE_QUEEN:
				pieceBoards[WQ_INDEX] |= (1ULL << newIndex);
				break;
			case BLACK_PAWN:
				if (!(move->isEP)) pieceBoards[BP_INDEX] |= (1ULL << newIndex);
				break;
			case BLACK_KNIGHT:
				pieceBoards[BN_INDEX] |= (1ULL << newIndex);
				break;
			case BLACK_BISHOP:
				pieceBoards[BB_INDEX] |= (1ULL << newIndex);
				break;
			case BLACK_ROOK:
				pieceBoards[BR_INDEX] |= (1ULL << newIndex);
				break;
			case BLACK_QUEEN:
				pieceBoards[BQ_INDEX] |= (1ULL << newIndex);
				break;
		}
	}

	// if en passant, put passanted pawn back on board
	if (move->isEP)
	{
		board[move->oldY][move->newX] = move->captured;
		if (move->piece->info & WHITE) pieceBoards[BP_INDEX] |= (1ULL << (8 * (7 - move->oldY) + move->newX));
		else pieceBoards[WP_INDEX] |= (1ULL << (8 * (7 - move->oldY) + move->newX));
	}

	if (move->isCastle)
	{
		// preemptively move rook back
		if ((move->piece->info & WHITE) == WHITE) // for white
		{
			if (move->newX < move->oldX) // queenside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the left rook
					if ((p->info & WHITE_ROOK) == WHITE_ROOK && p->rect->x == (move->newX + 1) * 60)
					{
						p->rect->x = 0; // move back to the left-most square
						board[7][0] = board[move->newY][move->newX + 1];
						board[move->newY][move->newX + 1] = 0;
						// update bitboards
						pieceBoards[WR_INDEX] &= ~(1ULL << 3); // remove left rook from d1
						pieceBoards[WR_INDEX] |=  (1ULL << 0); // add left rook to a1
						break;
					}
				}
			}
			else // kingside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the right rook
					if ((p->info & WHITE_ROOK) == WHITE_ROOK && p->rect->x == (move->newX - 1) * 60)
					{
						p->rect->x = 7 * 60; // move back to the right-most square
						board[7][7] = board[move->newY][move->newX - 1];
						board[move->newY][move->newX - 1] = 0;
						// update bitboards
						pieceBoards[WR_INDEX] &= ~(1ULL << 5); // remove right rook from f1
						pieceBoards[WR_INDEX] |=  (1ULL << 7); // add right rook to h1
						break;
					}
				}
			}
		}
		else // for black
		{
			if (move->newX < move->oldX) // queenside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the left rook
					if ((p->info & BLACK_ROOK) == BLACK_ROOK && p->rect->x == (move->newX + 1) * 60)
					{
						p->rect->x = 0; // move back to the left-most square
						board[0][0] = board[move->newY][move->newX + 1];
						board[move->newY][move->newX + 1] = 0;
						// update bitboards
						pieceBoards[BR_INDEX] &= ~(1ULL << 59); // remove left rook from d8
						pieceBoards[BR_INDEX] |=  (1ULL << 56); // add left rook to a8
						break;
					}
				}
			}
			else // kingside castle
			{
				for (Piece* p : piecesOnBoard)
				{
					// if we found the right rook
					if ((p->info & BLACK_ROOK) == BLACK_ROOK && p->rect->x == (move->newX - 1) * 60)
					{
						p->rect->x = 7 * 60; // move back to the right-most square
						board[0][7] = board[move->newY][move->newX - 1];
						board[move->newY][move->newX - 1] = 0;
						// update bitboards
						pieceBoards[BR_INDEX] &= ~(1ULL << 61); // remove right rook from f8
						pieceBoards[BR_INDEX] |=  (1ULL << 63); // add right rook to h8
						break;
					}
				}
			}
		}
	}

	#ifndef USING_BITS

	// reset previously enPassantable piece
	if (move->lossOfEP)
	{
		move->lostEP->enPassantable = true;
	}
	
	// if the piece was pawn that moved two spaces, un-passant it
	if ((move->piece->info & PAWN) == PAWN && abs(distMovedY) == 2) move->piece->enPassantable = false;
	
	#endif // !USING_BITS

	// reset en passant info
	enPassantInfo[0] = move->oldPassantSquare;
	enPassantInfo[1] = move->oldPassantPieceSquare;

	// reset castling rights
	if (move->oldCastlingRights & WK_CASTLE) whiteKingCanCastle = true;
	if (move->oldCastlingRights & WQR_CASTLE) whiteQueensideRookCanCastle = true;
	if (move->oldCastlingRights & WKR_CASTLE) whiteKingsideRookCanCastle = true;
	if (move->oldCastlingRights & BK_CASTLE) blackKingCanCastle = true;
	if (move->oldCastlingRights & BQR_CASTLE) blackQueensideRookCanCastle = true;
	if (move->oldCastlingRights & BKR_CASTLE) blackKingsideRookCanCastle = true;


	// reinstate halfmove counter
	halfmoves = move->oldHalfmoves;


	// if promotion, unpromote
	if (move->isPromoting)
	{
		// update respective promoPiece bitboard
		switch (move->promoPiece)
		{
			case WHITE_KNIGHT:
				pieceBoards[WN_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_BISHOP:
				pieceBoards[WB_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_ROOK:
				pieceBoards[WR_INDEX] &= ~(1ULL << newIndex);
				break;
			case WHITE_QUEEN:
				pieceBoards[WQ_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_KNIGHT:
				pieceBoards[BN_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_BISHOP:
				pieceBoards[BB_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_ROOK:
				pieceBoards[BR_INDEX] &= ~(1ULL << newIndex);
				break;
			case BLACK_QUEEN:
				pieceBoards[BQ_INDEX] &= ~(1ULL << newIndex);
				break;
		}
		// update board and piece info
		if ((turn - 1) % 2 == 0) move->piece->info = WHITE_PAWN;
		else move->piece->info = BLACK_PAWN;
		// note: updating bitboards is unnecessary since it will anyway clear the bit

	}

	// update board and turn
	board[move->oldY][move->oldX] = board[move->newY][move->newX];
	if (move->isCapture && !(move->isEP))
	{
		// already updated bitboards for capture earlier, update array now
		board[move->newY][move->newX] = move->captured;
	}
	else board[move->newY][move->newX] = 0;
	// update respective bitboard
	switch (move->piece->info)
	{
		case WHITE_PAWN:
			pieceBoards[WP_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WP_INDEX] &= ~(1ULL << newIndex);
			break;
		case WHITE_KNIGHT:
			pieceBoards[WN_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WN_INDEX] &= ~(1ULL << newIndex);
			break;
		case WHITE_BISHOP:
			pieceBoards[WB_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WB_INDEX] &= ~(1ULL << newIndex);
			break;
		case WHITE_ROOK:
			pieceBoards[WR_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WR_INDEX] &= ~(1ULL << newIndex);
			break;
		case WHITE_QUEEN:
			pieceBoards[WQ_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WQ_INDEX] &= ~(1ULL << newIndex);
			break;
		case WHITE_KING:
			pieceBoards[WK_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[WK_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_PAWN:
			pieceBoards[BP_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BP_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_KNIGHT:
			pieceBoards[BN_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BN_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_BISHOP:
			pieceBoards[BB_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BB_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_ROOK:
			pieceBoards[BR_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BR_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_QUEEN:
			pieceBoards[BQ_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BQ_INDEX] &= ~(1ULL << newIndex);
			break;
		case BLACK_KING:
			pieceBoards[BK_INDEX] |=  (1ULL << oldIndex);
			pieceBoards[BK_INDEX] &= ~(1ULL << newIndex);
			break;
	}
	turn--;

	// reset piece graphic coordinates
	move->piece->rect->x = move->oldX * 60;
	move->piece->rect->y = move->oldY * 60;

	/*
	// finaly, delete fen and position data
	fens.pop_back(); // assuming we unmake right after we make...
	std::string pos = move->fen.substr(0, move->fen.find(" "));
	if (positions.count(pos) == 0) std::cout << "Something is afoot!\n";
	else positions[pos]--; // the position should already exist, so just decrease counter
	if (positions[pos] == 0) positions.erase(pos); // prevent unnecessarily large map
	*/
}

inline void Game::bitsToMoves(uint64_t bitboard, unsigned long startSquare, uint8_t pieceType, std::vector<Move*> &moves) const
{
	Piece* startPiece = nullptr;
	// first, search for piece on the origin square
	int startX = startSquare % 8;
	int startY = 7 - startSquare / 8;
	startPiece = board[startY][startX];
	/*
	for (Piece* p : piecesOnBoard)
	{
		if (p->rect->x / 60 == startX && p->rect->y / 60 == startY)
		{
			startPiece = p;
			break;
		}
	}
	*/
	if (!startPiece)
	{
		std::cout << "uh oh, couldn't find piece\n";
		std::cout << "Tried to get piece with type ";
		switch (pieceType)
		{
			case WHITE_PAWN:
				std::cout << "White pawn";
				break;
			case WHITE_KNIGHT:
				std::cout << "White knight";
				break;
			case WHITE_BISHOP:
				std::cout << "White bishop";
				break;
			case WHITE_ROOK:
				std::cout << "White rook";
				break;
			case WHITE_QUEEN:
				std::cout << "White queen";
				break;
			case WHITE_KING:
				std::cout << "White king";
				break;
			case BLACK_PAWN:
				std::cout << "Black pawn";
				break;
			case BLACK_KNIGHT:
				std::cout << "Black knight";
				break;
			case BLACK_BISHOP:
				std::cout << "Black bishoP";
				break;
			case BLACK_ROOK:
				std::cout << "Black rook";
				break;
			case BLACK_QUEEN:
				std::cout << "Black queen";
				break;
			case BLACK_KING:
				std::cout << "Black king";
				break;
		}
		std::cout << " on square " << (char)(97 + startSquare % 8) << startSquare / 8 + 1 << '\n';
		std::cout << "Board state:\n";
		printBoard();
	}
	// loop through moves in bitboard
	while (bitboard)
	{
		Piece* piece = nullptr; // the piece that's moving
		Piece* captured = nullptr; // the piece captured
		bool isPromoting = false;
		bool isCapture;
		bool isEP; // en passant capture
		int oldPassantSquare;
		int oldPassantPieceSquare;
		bool isCastle;
		uint8_t oldCastlingRights;
		uint8_t promoPiece; // the piece being promoted to
		int oldHalfmoves;
		//std::string fen;

		// the old and new move coordinates
		int oldX;
		int oldY;
		int newX;
		int newY;

		// prepare Move materials
		// Move* move = new Move;
		unsigned long index;
		char code = _BitScanForward64(&index, bitboard);
		if (code)
		{
			// set move coordinates
			newX = index % 8;
			newY = 7 - index / 8;
			oldX = startX;
			oldY = startY;

			// miscellaneous
			piece = startPiece;
			oldHalfmoves = halfmoves;
			oldPassantSquare = enPassantInfo[0];
			oldPassantPieceSquare = enPassantInfo[1];
			// record old castling rights
			oldCastlingRights = 0;
			if (whiteKingCanCastle) oldCastlingRights |= WK_CASTLE;
			if (whiteQueensideRookCanCastle) oldCastlingRights |= WQR_CASTLE;
			if (whiteKingsideRookCanCastle) oldCastlingRights |= WKR_CASTLE;
			if (blackKingCanCastle) oldCastlingRights |= BK_CASTLE;
			if (blackQueensideRookCanCastle) oldCastlingRights |= BQR_CASTLE;
			if (blackKingsideRookCanCastle) oldCastlingRights |= BKR_CASTLE;
			
			// check for en passant capture
			isEP = false;
			captured = nullptr;
			isCapture = false;
			if ((pieceType & PAWN) && index == enPassantInfo[0])
			{
				isEP = true;
				isCapture = true;
				captured = board[oldY][newX];
			}

			// check for normal capture and en passant loss
			if (!isEP)
			{
				if (board[newY][newX])
				{
					captured = board[newY][newX];
					isCapture = true;
				}
			}
			/*
			for (Piece* p : piecesOnBoard)
			{
				if ((p->rect->x == move->newX * 60 && p->rect->y == move->newY * 60) || 
					move->isEP && (8 * (7 - p->rect->y / 60) + p->rect->x / 60) == enPassantInfo[1])
				{
					move->captured = p;
					move->isCapture = true;
					break;
				}
			}
			*/

			#ifndef USING_BITS

			// check for loss of en passant
			move->lossOfEP = false;
			move->lostEP = nullptr;
			for (Piece* p : piecesOnBoard)
			{
				if (p->enPassantable)
				{
					move->lossOfEP = true;
					move->lostEP = p;
					break;
				}
			}
			
			#endif // !USING_BITS

			// check for castle
			if ((pieceType & KING) && std::abs(newX - oldX) == 2)
			{
				// it's a castle!
				isCastle = true;
			}
			else isCastle = false;

			// check for end-rank promotion in white
			promoPiece = 0;
			if ((pieceType == WHITE_PAWN) && newY == 0)
			{
				isPromoting = true;
				/*
				// spawn multiple moves for different promotions
				Move* knightPromo = move->cloneMove();
				knightPromo->promoPiece = WHITE_KNIGHT;
				moves.push_back(knightPromo);
				Move* bishopPromo = move->cloneMove();
				bishopPromo->promoPiece = WHITE_BISHOP;
				moves.push_back(bishopPromo);
				Move* rookPromo = move->cloneMove();
				rookPromo->promoPiece = WHITE_ROOK;
				moves.push_back(rookPromo);
				// leave queen promotion to original
				move->promoPiece = WHITE_QUEEN;
				*/
			}
			else if ((pieceType == BLACK_PAWN) && newY == 7)
			{
				isPromoting = true;
				/*
				// spawn multiple moves for different promotions
				Move* knightPromo = move->cloneMove();
				knightPromo->promoPiece = BLACK_KNIGHT;
				moves.push_back(knightPromo);
				Move* bishopPromo = move->cloneMove();
				bishopPromo->promoPiece = BLACK_BISHOP;
				moves.push_back(bishopPromo);
				Move* rookPromo = move->cloneMove();
				rookPromo->promoPiece = BLACK_ROOK;
				moves.push_back(rookPromo);
				// leave queen promotion to original
				move->promoPiece = BLACK_QUEEN;
				*/
			}
			else isPromoting = false;
		}
		else std::cout << "PROBLEM!!\n";

		// remove move from bitboard
		bitboard &= bitboard - 1;

		// create move
		Move* move = new Move(piece, captured, isPromoting, isCapture, isEP, oldPassantSquare, oldPassantPieceSquare, isCastle, oldCastlingRights, promoPiece,
			oldHalfmoves, oldX, oldY, newX, newY);
		
		// check for any promotions
		if (isPromoting)
		{
			if (pieceType & WHITE)
			{
				// spawn multiple moves for different promotions
				Move* knightPromo = move->cloneMove();
				knightPromo->promoPiece = WHITE_KNIGHT;
				moves.push_back(knightPromo);
				Move* bishopPromo = move->cloneMove();
				bishopPromo->promoPiece = WHITE_BISHOP;
				moves.push_back(bishopPromo);
				Move* rookPromo = move->cloneMove();
				rookPromo->promoPiece = WHITE_ROOK;
				moves.push_back(rookPromo);
				// leave queen promotion to original
				move->promoPiece = WHITE_QUEEN;
			}
			else
			{
				// spawn multiple moves for different promotions
				Move* knightPromo = move->cloneMove();
				knightPromo->promoPiece = BLACK_KNIGHT;
				moves.push_back(knightPromo);
				Move* bishopPromo = move->cloneMove();
				bishopPromo->promoPiece = BLACK_BISHOP;
				moves.push_back(bishopPromo);
				Move* rookPromo = move->cloneMove();
				rookPromo->promoPiece = BLACK_ROOK;
				moves.push_back(rookPromo);
				// leave queen promotion to original
				move->promoPiece = BLACK_QUEEN;
			}
		}

		// add move object to vector
		moves.push_back(move);

	}
}

ull Game::generateLegalMoves(std::vector<Move*>& moves)
{
	uint64_t bitmoves = 0;
	uint64_t whitePieces = 0;
	uint64_t blackPieces = 0;
	uint64_t whiteNoKing = 0;
	uint64_t blackNoKing = 0;
	whitePieces |= pieceBoards[WP_INDEX] | pieceBoards[WN_INDEX] | pieceBoards[WB_INDEX] | pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX] | pieceBoards[WK_INDEX];
	blackPieces |= pieceBoards[BP_INDEX] | pieceBoards[BN_INDEX] | pieceBoards[BB_INDEX] | pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX] | pieceBoards[BK_INDEX];
	whiteNoKing = whitePieces & ~pieceBoards[WK_INDEX];
	blackNoKing = blackPieces & ~pieceBoards[BK_INDEX];
	if (turn % 2 == 0) // for white
	{
		#ifdef USING_BITS
		{
			// first, look for checks
			// get the king position
			unsigned long kingPos;
			if (!(_BitScanForward64(&kingPos, pieceBoards[WK_INDEX]))) std::cout << "WHITE KING NOT FOUND!!\n";
			// then, look for pieces in the king's POV
			int attackers = 0;
			// black pawn
			uint64_t blackAttackingPawns = 0;
			blackAttackingPawns |= whitePawnAttacks[kingPos] & pieceBoards[BP_INDEX];

			// black knight
			uint64_t blackAttackingKnights= 0;
			blackAttackingKnights |= knightMoves[kingPos] & pieceBoards[BN_INDEX];

			// diagonal rays (black bishop/queen)
			uint64_t blockers = whiteNoKing | blackPieces;
			uint64_t diagonalAttacks = 0;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 0) continue; // only diagonal rays allowed
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else bitmoves = rays[i][kingPos]; // no blockers

				diagonalAttacks |= bitmoves;
			}
			diagonalAttacks &= (pieceBoards[BB_INDEX] | pieceBoards[BQ_INDEX]);

			// adjacent rays (black rook/queen)
			blockers = whiteNoKing | blackPieces;
			uint64_t adjAttacks = 0;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 != 0) continue; // only adjacent rays allowed
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else bitmoves = rays[i][kingPos]; // no blockers

				adjAttacks |= bitmoves;
			}
			adjAttacks &= (pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX]);

			


			// Next, check for pins
			uint64_t pinMask = 0;
			// first, calculate all sliding piece moves from king square
			blockers = whiteNoKing | blackPieces;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				uint64_t kingSlides = 0;
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				uint64_t rayMoves = 0;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					rayMoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else rayMoves = rays[i][kingPos]; // no blockers

				kingSlides |= rayMoves;
			

				// next, check for all enemy sliding piece moves
				uint64_t enemySlidingAttacks = 0;
				// for black bishops
				bitmoves = 0;
				uint64_t holder = pieceBoards[BB_INDEX];
				//if (!holder) std::cout << "No black bishops detected!\n";
				// loop over each bishop
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						if (oppIndex % 2 == 0) break; // only diagonal rays allowed
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;
					}
					else std::cout << "PROBLEM WITH BLACK BISHOPS!\n";
					// clear board of that bit
					holder &= holder - 1;
				}


				// for black rooks
				bitmoves = 0;
				holder = pieceBoards[BR_INDEX];
				//if (!holder) std::cout << "No black rooks detected!\n";
				// loop over each rook
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						if (oppIndex % 2 == 1) break; // only adjacent rays allowed
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;
					
					}
					else std::cout << "PROBLEM WITH BLACK ROOKS!\n";

					// clear board of that bit
					holder &= holder - 1;
				}

				// for black queens
				bitmoves = 0;
				holder = pieceBoards[BQ_INDEX];
				//if (!holder) std::cout << "No black queens detected!\n";
				// loop over each queen
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;
					
					}
					else std::cout << "PROBLEM WITH BLACK QUEENS!\n";

					// clear board of that bit
					holder &= holder - 1;
				}

				// add the overlap of king attack rays, opposite enemy moves, and the white pieces to the pin mask
				pinMask |= enemySlidingAttacks & kingSlides & whitePieces;
			}


			// Now, calculate the pseudo-legal move rays for the pinned pieces
			uint64_t pinnedPieceMoves[64];
			for (int i = 0; i < 64; i++) pinnedPieceMoves[i] = ULLONG_MAX; // for pieces that aren't pinned, allow all squares
			uint64_t holder = pinMask;
			while (holder)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, holder);
				if (code)
				{
					// first, figure out the ray along which the piece is pinned
					int rayIndex;
					for (int i = 0; i < 8; i++)
					{
						if (rays[i][kingPos] & (1ULL << index))
						{
							rayIndex = i;
							break;
						}
					}
					// then, find the ray from the king to the pinner
					uint64_t pinRay = 0;
					uint64_t maskedBlockers = rays[rayIndex][kingPos] & blackPieces;
					unsigned long i2;
					unsigned char c;
					if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
					else c = _BitScanForward64(&i2, maskedBlockers);

					if (c)
					{
						pinRay = rays[rayIndex][kingPos] & ~rays[rayIndex][i2]; // mask our regular ray with our pinner's ray
					}
					else std::cout << "ERROR! False pin detected!\n";

					// check what piece this is
					if ((1ULL << index) & pieceBoards[WP_INDEX])
					{
						// it's a pawn! Check for possible captures
						uint64_t possiblePawnCaptures = whitePawnAttacks[index] & blackPieces;
						// verify the captures and pushes against the pin ray
						uint64_t pawnMoves = (possiblePawnCaptures | whitePawnPushes[index]) & pinRay;
						pinnedPieceMoves[index] = pawnMoves;
					}
					else if ((1ULL << index) & pieceBoards[WN_INDEX])
					{
						// it's a knight! ...it can't move
						pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[WB_INDEX])
					{
						// it's a bishop! Check if the pin ray is diagonal
						if (rayIndex % 2 == 1) pinnedPieceMoves[index] = pinRay;
						else pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[WR_INDEX])
					{
						// it's a rook! Check if the pin ray is adjacent
						if (rayIndex % 2 == 0) pinnedPieceMoves[index] = pinRay;
						else pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[WQ_INDEX])
					{
						// it's a queen! It can only move along the ray
						pinnedPieceMoves[index] = pinRay;
					}
					else std::cout << "ERROR! Either pinned piece is a king or pinned piece not found.\n";
				}

				// clear board of that bit
				holder &= holder - 1;
			}

			

			// Next, calculate danger squares
			// this is done by calculating all possible moves for black w/o king on the board for black pawns
			bitmoves = 0;
			uint64_t blackAttack = 0;

			uint64_t tmp = pieceBoards[BP_INDEX];
			//if (!tmp) std::cout << "No black pawns detected!\n";
			// loop over each pawn
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = (blackPawnAttacks[index]);
					blackAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH BLACK PAWNS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for black knights
			bitmoves = 0;
			tmp = pieceBoards[BN_INDEX];
			//if (!tmp) std::cout << "No black knights detected!\n";
			// loop over each knight
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = knightMoves[index];
					blackAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH BLACK KNIGHTS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for black bishops
			bitmoves = 0;
			tmp = pieceBoards[BB_INDEX];
			//if (!tmp) std::cout << "No black bishops detected!\n";
			// loop over each bishop
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = whiteNoKing | blackPieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						if (i % 2 == 0) continue; // only diagonal rays allowed
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						blackAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH BLACK BISHOPS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for black rooks
			bitmoves = 0;
			tmp = pieceBoards[BR_INDEX];
			//if (!tmp) std::cout << "No black rooks detected!\n";
			// loop over each rook
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = whiteNoKing | blackPieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						if (i % 2 == 1) continue; // only adjacent rays allowed
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						blackAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH BLACK ROOKS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for black queens
			bitmoves = 0;
			tmp = pieceBoards[BQ_INDEX];
			//if (!tmp) std::cout << "No black queens detected!\n";
			// loop over each queen
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = whiteNoKing | blackPieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						blackAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH BLACK QUEENS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for black king
			bitmoves = 0;
			tmp = pieceBoards[BK_INDEX];
			if (!tmp) std::cout << "ERROR! Black king not detected!\n";
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = kingMoves[index] & whiteNoKing;
					blackAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH BLACK KING!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// count number of attackers
			uint64_t attackBoard = blackAttackingKnights | blackAttackingPawns | diagonalAttacks | adjAttacks;
			attackers += std::bitset<64>(attackBoard).count();


			// Next, check for number of attackers (0, 1, 2)
			uint64_t legalKingMoves;
			uint64_t checkRay;
			int attackerSquare;
			switch (attackers)
			{
				// double check
				case 2:
					//std::cout << "Double check!\n";
					// only king moves are allowed; mask them with the danger squares
					legalKingMoves = kingMoves[kingPos] & ~blackAttack & ~whitePieces;
					bitsToMoves(legalKingMoves, kingPos, WHITE_KING, moves);
					break;

				// single check
				case 1:
					//std::cout << "Single check!\n";
					// Step 1: calculate legal king moves
					legalKingMoves = kingMoves[kingPos] & ~blackAttack & ~whitePieces;
					bitsToMoves(legalKingMoves, kingPos, WHITE_KING, moves);

					// Step 2: calculate ways to capture or block the attacker
					attackerSquare = bitToIndex(attackBoard);
					checkRay = 0;

					// if checker is a pawn or knight, the check ray should only be the attacker square
					if (attackBoard & (pieceBoards[BP_INDEX] | pieceBoards[BN_INDEX])) checkRay = attackBoard;
					else
					{
						// first, figure out the ray along which the king is in check
						int rayIndex;
						for (int i = 0; i < 8; i++)
						{
							if (rays[i][kingPos] & attackBoard)
							{
								rayIndex = i;
								break;
							}
						}
						// then, calculate the ray from the king to the checker
						checkRay = rays[rayIndex][kingPos] & ~rays[rayIndex][attackerSquare]; // mask our regular ray with the checker's ray
					}
					//std::cout << "Check ray: " << checkRay << '\n';

					// can any pawn move?
					bitmoves = 0;
					tmp = pieceBoards[WP_INDEX];
					//if (!tmp) std::cout << "No white pawns detected!\n";
					// loop over each pawn
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							// check for possible en passant move
							uint64_t passant = 0;
							if (enPassantInfo[0] != -1 && (1ULL << enPassantInfo[1]) & pieceBoards[BP_INDEX])
							{
								passant = (1ULL << enPassantInfo[0]);
								// check for rare en passant pin
								if (whitePawnAttacks[index] & passant)
								{
									// check if the left or right ray is in line with the pawn
									int rayIndex;
									if (rays[2][kingPos] & (1ULL << index)) rayIndex = 2;
									else if (rays[6][kingPos] & (1ULL << index)) rayIndex = 6;
									else rayIndex = -1;
									if (rayIndex != -1)
									{
										uint64_t blackWithoutPassant = blackPieces & ~(1ULL << enPassantInfo[1]);;
										uint64_t whiteWithoutPawn = whitePieces & ~(1ULL << index);
										uint64_t blockersEP = whiteWithoutPawn | blackWithoutPassant;
										uint64_t maskedBlockersEP = rays[rayIndex][kingPos] & blockersEP;
										unsigned long i2;
										unsigned char c;
										if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockersEP); // for negative rays
										else c = _BitScanForward64(&i2, maskedBlockersEP);

										if (c)
										{
											// check if the nearest "blocker" is a rook or queen
											if ((1ULL << i2) & (pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX])) passant = 0; // pin found!!
										}
										else; // do nothing; no pin

									}
								}
							}
							uint64_t pawnPushes = whitePawnPushes[index];
							// for first-row pawns
							if (index >= 8 && index < 16)
							{
								// if something blocks the pawn
								if ((1ULL << (index + 8)) & (whitePieces | blackPieces)) pawnPushes = 0;
							}
							// mask pushes with blockers, mask captures with black pieces
							bitmoves = (pawnPushes & ~(whitePieces | blackPieces)) |
								(whitePawnAttacks[index] & (blackPieces | passant));
							// make sure the pawn doesn't disobey a pin or disregard check
							bitmoves &= pinnedPieceMoves[index];
							bitmoves &= checkRay;
							// finally, check for en passant capture check evasion
							if ((attackBoard & pieceBoards[BP_INDEX]) && bitToIndex(attackBoard) == enPassantInfo[1]) bitmoves |= whitePawnAttacks[index] & passant;
							//std::cout << "Pawn Moves: " << bitmoves << '\n';

							bitsToMoves(bitmoves, index, WHITE_PAWN, moves);
						}
						else std::cout << "PROBLEM WITH WHITE PAWNS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any knight move?
					bitmoves = 0;
					tmp = pieceBoards[WN_INDEX];
					//if (!tmp) std::cout << "No white knights detected!\n";
					// loop over each knight
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							// all-in-one check
							bitmoves = (knightMoves[index] & ~whitePieces & pinnedPieceMoves[index]) & checkRay;
							bitsToMoves(bitmoves, index, WHITE_KNIGHT, moves);
						}
						else std::cout << "PROBLEM WITH WHITE KNIGHTS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any bishop move?
					bitmoves = 0;
					tmp = pieceBoards[WB_INDEX];
					//if (!tmp) std::cout << "No white bishops detected!\n";
					// loop over each bishop
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								if (i % 2 == 0) continue; // only diagonal rays allowed
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];

								// now check the moves against the check ray
								bitmoves &= checkRay;

								bitsToMoves(bitmoves, index, WHITE_BISHOP, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE BISHOPS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any rook move?
					bitmoves = 0;
					tmp = pieceBoards[WR_INDEX];
					//if (!tmp) std::cout << "No white rooks detected!\n";
					// loop over each rook
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								if (i % 2 == 1) continue; // only adjacent rays allowed
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];

								// now check the moves against the check ray
								bitmoves &= checkRay;

								bitsToMoves(bitmoves, index, WHITE_ROOK, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE ROOKS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any queen move?
					bitmoves = 0;
					tmp = pieceBoards[WQ_INDEX];
					//if (!tmp) std::cout << "No white queens detected!\n";
					// loop over each queen
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];

								// now check the moves against the check ray
								bitmoves &= checkRay;

								bitsToMoves(bitmoves, index, WHITE_QUEEN, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE QUEENS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					break;


				// no checks
				case 0:
					//std::cout << "No check!\n";
					// Debugging info:
					//std::cout << "KING POSITION: " << kingPos << '\n';
					//std::cout << "PAWN BITBOARD: " << pieceBoards[WP_INDEX] << '\n';
					//std::cout << "KNIGHT BITBOARD: " << pieceBoards[WN_INDEX] << '\n';
					//std::cout << "BISHOP BITBOARD: " << pieceBoards[WB_INDEX] << '\n';
					//std::cout << "ROOK BITBOARD: " << pieceBoards[WR_INDEX] << '\n';
					//std::cout << "QUEEN BITBOARD: " << pieceBoards[WQ_INDEX] << '\n';
					//std::cout << "KING BITBOARD: " << pieceBoards[WK_INDEX] << '\n';
					// Step 1: calculate legal king moves
					legalKingMoves = 0;
					// we are now allowed to castle, so check for that
					if (whiteKingCanCastle) // king hasn't moved
					{
						if (whiteKingsideRookCanCastle) // kingside rook hasn't moved
						{
							uint64_t whiteKingsideCastle = ((1ULL << (kingPos + 1)) | (1ULL << (kingPos + 2)));
							// this ternary ensures that the king isn't moving through pieces or check in the castle
							legalKingMoves |= (whiteKingsideCastle & ~(whitePieces | blackPieces) & ~blackAttack) == whiteKingsideCastle ? (1ULL << (kingPos + 2)) : 0;
						}
						if (whiteQueensideRookCanCastle)
						{
							uint64_t whiteQueensideCastle = ((1ULL << (kingPos - 1)) | (1ULL << (kingPos - 2)));
							// this ternary ensures that the king isn't moving through pieces or check in the castle
							legalKingMoves |= (whiteQueensideCastle & ~(whitePieces | blackPieces) & ~blackAttack) == whiteQueensideCastle ? (1ULL << (kingPos - 2)) : 0;
						}
					}
					// tack on any other legal king moves
					legalKingMoves |= (kingMoves[kingPos] & ~blackAttack) & ~whitePieces;
					//std::cout << "Black Attack: " << blackAttack << '\n';
					//std::cout << "King Moves: " << legalKingMoves << '\n';
					bitsToMoves(legalKingMoves, kingPos, WHITE_KING, moves);

					// Step 2: calculate all other pieces' moves

					// can any pawn move?
					bitmoves = 0;
					tmp = pieceBoards[WP_INDEX];
					uint64_t passant;
					int rayIndex;
					uint64_t blackWithoutPassant;
					uint64_t whiteWithoutPawn;
					uint64_t blockersEP;
					uint64_t maskedBlockersEP;
					uint64_t pawnPushes;
					//if (!tmp) std::cout << "No white pawns detected!\n";
					// loop over each pawn
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							// check for possible en passant move
							if (enPassantInfo[0] != -1 && (1ULL << enPassantInfo[1]) & pieceBoards[BP_INDEX])
							{
								passant = (1ULL << enPassantInfo[0]);
								// check for rare en passant pin
								if (whitePawnAttacks[index] & passant)
								{
									// check if the left or right ray is in line with the pawn
									if (rays[2][kingPos] & (1ULL << index)) rayIndex = 2;
									else if (rays[6][kingPos] & (1ULL << index)) rayIndex = 6;
									else rayIndex = -1;
									if (rayIndex != -1)
									{
										blackWithoutPassant = blackPieces & ~(1ULL << enPassantInfo[1]);
										whiteWithoutPawn = whitePieces & ~(1ULL << index);
										blockersEP = whiteWithoutPawn | blackWithoutPassant;
										maskedBlockersEP = rays[rayIndex][kingPos] & blockersEP;
										unsigned long i2;
										unsigned char c;
										if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockersEP); // for negative rays
										else c = _BitScanForward64(&i2, maskedBlockersEP);

										if (c)
										{
											// check if the nearest "blocker" is a rook or queen
											if ((1ULL << i2) & (pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX])) passant = 0; // pin found!!
										}
										else; // do nothing; no pin

									}
								}
							}
							else passant = 0;
							pawnPushes = whitePawnPushes[index];
							// for first-row pawns
							if (index >= 8 && index < 16)
							{
								// if something blocks the pawn
								if ((1ULL << (index + 8)) & (whitePieces | blackPieces)) pawnPushes = 0;
							}
							// mask pushes with blockers, mask captures with black pieces
							bitmoves = (pawnPushes & ~(whitePieces | blackPieces)) |
								(whitePawnAttacks[index] & (blackPieces | passant));
							// make sure the pawn doesn't disobey a pin
							bitmoves &= pinnedPieceMoves[index];
							//std::cout << "Pawn Moves: " << bitmoves << '\n';

							bitsToMoves(bitmoves, index, WHITE_PAWN, moves);
						}
						else std::cout << "PROBLEM WITH WHITE PAWNS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any knight move?
					bitmoves = 0;
					tmp = pieceBoards[WN_INDEX];
					//if (!tmp) std::cout << "No white knights detected!\n";
					// loop over each knight
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							// all-in-one check
							bitmoves = (knightMoves[index] & ~whitePieces & pinnedPieceMoves[index]);
							//std::cout << "Knight Moves: " << bitmoves << '\n';
							bitsToMoves(bitmoves, index, WHITE_KNIGHT, moves);
						}
						else std::cout << "PROBLEM WITH WHITE KNIGHTS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any bishop move?
					bitmoves = 0;
					tmp = pieceBoards[WB_INDEX];
					//if (!tmp) std::cout << "No white bishops detected!\n";
					// loop over each bishop
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								if (i % 2 == 0) continue; // only diagonal rays allowed
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];
								//std::cout << "Bishop Moves: " << bitmoves << '\n';

								bitsToMoves(bitmoves, index, WHITE_BISHOP, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE BISHOPS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any rook move?
					bitmoves = 0;
					tmp = pieceBoards[WR_INDEX];
					//if (!tmp) std::cout << "No white rooks detected!\n";
					// loop over each rook
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								if (i % 2 == 1) continue; // only adjacent rays allowed
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];
								//std::cout << "Rook Moves: " << bitmoves << '\n';

								bitsToMoves(bitmoves, index, WHITE_ROOK, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE ROOKS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}

					// can any queen move?
					bitmoves = 0;
					tmp = pieceBoards[WQ_INDEX];
					//if (!tmp) std::cout << "No white queens detected!\n";
					// loop over each queen
					while (tmp)
					{
						unsigned long index;
						unsigned char code = _BitScanForward64(&index, tmp);
						if (code)
						{
							uint64_t blockers = whitePieces | blackPieces;
							// iterate over all rays
							for (int i = 0; i < 8; i++)
							{
								uint64_t maskedBlockers = rays[i][index] & blockers;
								unsigned long i2;
								unsigned char c;
								if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
								else c = _BitScanForward64(&i2, maskedBlockers);

								if (c)
								{
									bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
									bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
								}
								else bitmoves = rays[i][index]; // no blockers

								// ensure we follow pin rules
								bitmoves &= pinnedPieceMoves[index];
								//std::cout << "Queen Moves: " << bitmoves << '\n';

								bitsToMoves(bitmoves, index, WHITE_QUEEN, moves);
							}
						}
						else std::cout << "PROBLEM WITH WHITE QUEENS!\n";

						// clear board of that bit
						tmp &= tmp - 1;
					}
					break;

				default:
					std::cout << "ERROR: Triple check (or higher) detected\n";
					std::cout << "Attackers detected: " << attackers << '\n';
			}
		}

		#else
		for (Piece* p : piecesOnBoard)
		{
			Move* potentialMove;
			if ((p->info & WHITE) == WHITE)
			{
				// go thru all pieces
				/* PAWN MOVES */
				if ((p->info & PAWN) == PAWN)
				{
					// one space up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
					// two spaces up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// capture left
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
					// capture right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = WHITE_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
				}

				/* KNIGHT MOVES */
				else if ((p->info & KNIGHT) == KNIGHT)
				{
					// up-left
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// up-right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// down-left
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// down-right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// left-up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// left-down
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// right-up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// right-down
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
				}

				/* BISHOP MOVES */
				else if ((p->info & BISHOP) == BISHOP)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check up-right diagonal
					while (tmpX <= 7 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check down-left diagonal
					while (tmpX >= 0 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check down-right diagonal
					while (tmpX <= 7 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY++;
					}
				}

				/* ROOK MOVES */
				else if ((p->info & ROOK) == ROOK)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check right
					while (tmpX <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
					}

					tmpX = initX, tmpY = initY;
					// check down
					while (tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check left
					while (tmpX >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
					}
				}

				/* QUEEN MOVES */
				else if ((p->info & QUEEN) == QUEEN) // fix
				{
					// CHECK DIAGONALS

					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check up-right diagonal
					while (tmpX <= 7 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check down-left diagonal
					while (tmpX >= 0 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check down-right diagonal
					while (tmpX <= 7 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY++;
					}


					// CHECK ADJACENCIES

					tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check right
					while (tmpX <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
					}

					tmpX = initX, tmpY = initY;
					// check down
					while (tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check left
					while (tmpX >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
					}
				}

				/* KING MOVES */
				else if ((p->info & KING) == KING)
				{
					int kingX = p->rect->x / 60, kingY = p->rect->y / 60;
					// move up
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, kingX * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move up-left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move up-right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, kingX * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down-left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down-right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// castle kingside
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 2) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// castle queenside
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 2) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
				}
			}
		}
		#endif
	}
	else // for black
	{
		#ifdef USING_BITS
		{
			// first, look for checks
			// get the king position
			unsigned long kingPos;
			if (!_BitScanForward64(&kingPos, pieceBoards[BK_INDEX])) std::cout << "BLACK KING NOT FOUND!!\n";
			// then, look for pieces in the king's POV
			int attackers = 0;
			// white pawn
			uint64_t whiteAttackingPawns = 0;
			whiteAttackingPawns |= blackPawnAttacks[kingPos] & pieceBoards[WP_INDEX];

			// white knight
			uint64_t whiteAttackingKnights = 0;
			whiteAttackingKnights |= knightMoves[kingPos] & pieceBoards[WN_INDEX];

			// diagonal rays (white bishop/queen)
			uint64_t blockers = blackNoKing | whitePieces;
			uint64_t diagonalAttacks = 0;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 0) continue; // only diagonal rays allowed
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else bitmoves = rays[i][kingPos]; // no blockers

				diagonalAttacks |= bitmoves;
			}
			diagonalAttacks &= (pieceBoards[WB_INDEX] | pieceBoards[WQ_INDEX]);

			// adjacent rays (white rook/queen)
			blockers = blackNoKing | whitePieces;
			uint64_t adjAttacks = 0;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 != 0) continue; // only adjacent rays allowed
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					bitmoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else bitmoves = rays[i][kingPos]; // no blockers

				adjAttacks |= bitmoves;
			}
			adjAttacks &= (pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX]);


			// Next, check for pins
			uint64_t pinMask = 0;
			// first, calculate all sliding piece moves from king square
			blockers = blackNoKing | whitePieces;
			// iterate over all rays
			for (int i = 0; i < 8; i++)
			{
				uint64_t kingSlides = 0;
				uint64_t maskedBlockers = rays[i][kingPos] & blockers;
				uint64_t rayMoves = 0;
				unsigned long i2;
				unsigned char c;
				if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
				else c = _BitScanForward64(&i2, maskedBlockers);

				if (c)
				{
					rayMoves = rays[i][kingPos] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
				}
				else rayMoves = rays[i][kingPos]; // no blockers

				kingSlides |= rayMoves;


				// next, check for all enemy sliding piece moves
				uint64_t enemySlidingAttacks = 0;
				// for white bishops
				bitmoves = 0;
				uint64_t holder = pieceBoards[WB_INDEX];
				//if (!holder) std::cout << "No white bishops detected!\n";
				// loop over each bishop
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						if (oppIndex % 2 == 0) break; // only diagonal rays allowed
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;
					}
					else std::cout << "PROBLEM WITH WHITE BISHOPS!\n";

					// clear board of that bit
					holder &= holder - 1;
				}

				// for white rooks
				bitmoves = 0;
				holder = pieceBoards[WR_INDEX];
				//if (!holder) std::cout << "No white rooks detected!\n";
				// loop over each rook
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						if (oppIndex % 2 == 1) break; // only adjacent rays allowed
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;

					}
					else std::cout << "PROBLEM WITH WHITE ROOKS!\n";

					// clear board of that bit
					holder &= holder - 1;
				}

				// for white queens
				bitmoves = 0;
				holder = pieceBoards[WQ_INDEX];
				//if (!holder) std::cout << "No white queens detected!\n";
				// loop over each queen
				while (holder)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, holder);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						int oppIndex = (i + 4) % 8;
						uint64_t maskedBlockers = rays[oppIndex][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (oppIndex < 7 && oppIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[oppIndex][index] & ~rays[oppIndex][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[oppIndex][index]; // no blockers

						enemySlidingAttacks |= bitmoves;

					}
					else std::cout << "PROBLEM WITH WHITE QUEENS!\n";

					// clear board of that bit
					holder &= holder - 1;
				}

				// add the overlap of king attack rays, opposite enemy moves, and the black pieces to the pin mask
				pinMask |= enemySlidingAttacks & kingSlides & blackPieces;
			}

			// Now, calculate the pseudo-legal move rays for the pinned pieces
			uint64_t pinnedPieceMoves[64];
			for (int i = 0; i < 64; i++) pinnedPieceMoves[i] = ULLONG_MAX; // for pieces that aren't pinned, allow all squares
			uint64_t holder = pinMask;
			while (holder)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, holder);
				if (code)
				{
					// first, figure out the ray along which the piece is pinned
					int rayIndex;
					for (int i = 0; i < 8; i++)
					{
						if (rays[i][kingPos] & (1ULL << index))
						{
							rayIndex = i;
							break;
						}
					}
					// then, find the ray from the king to the pinner
					uint64_t pinRay = 0;
					uint64_t maskedBlockers = rays[rayIndex][kingPos] & whitePieces;
					unsigned long i2;
					unsigned char c;
					if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
					else c = _BitScanForward64(&i2, maskedBlockers);

					if (c)
					{
						pinRay = rays[rayIndex][kingPos] & ~rays[rayIndex][i2]; // mask our regular ray with our pinner's ray
					}
					else std::cout << "ERROR! False pin detected!\n";

					// check what piece this is
					if ((1ULL << index) & pieceBoards[BP_INDEX])
					{
						// it's a pawn! Check for possible captures
						uint64_t possiblePawnCaptures = blackPawnAttacks[index] & whitePieces;
						// verify the captures and pushes against the pin ray
						uint64_t pawnMoves = (possiblePawnCaptures | blackPawnPushes[index]) & pinRay;
						pinnedPieceMoves[index] = pawnMoves;
					}
					else if ((1ULL << index) & pieceBoards[BN_INDEX])
					{
						// it's a knight! ...it can't move
						pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[BB_INDEX])
					{
						// it's a bishop! Check if the pin ray is diagonal
						if (rayIndex % 2 == 1) pinnedPieceMoves[index] = pinRay;
						else pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[BR_INDEX])
					{
						// it's a rook! Check if the pin ray is adjacent
						if (rayIndex % 2 == 0) pinnedPieceMoves[index] = pinRay;
						else pinnedPieceMoves[index] = 0;
					}
					else if ((1ULL << index) & pieceBoards[BQ_INDEX])
					{
						// it's a queen! It can only move along the ray
						pinnedPieceMoves[index] = pinRay;
					}
					else std::cout << "ERROR! Either pinned piece is a king or pinned piece not found.\n";
				}

				// clear board of that bit
				holder &= holder - 1;
			}



			// Next, calculate danger squares
			// this is done by calculating all possible moves for white w/o king on the board
			
			// for white pawns
			bitmoves = 0;
			uint64_t whiteAttack = 0;

			uint64_t tmp = pieceBoards[WP_INDEX];
			//if (!tmp) std::cout << "No white pawns detected!\n";
			// loop over each pawn
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = (whitePawnAttacks[index]);
					whiteAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH WHITE PAWNS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for white knights
			bitmoves = 0;
			tmp = pieceBoards[WN_INDEX];
			//if (!tmp) std::cout << "No white knights detected!\n";
			// loop over each knight
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = knightMoves[index];
					whiteAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH WHITE KNIGHTS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for white bishops
			bitmoves = 0;
			tmp = pieceBoards[WB_INDEX];
			//if (!tmp) std::cout << "No white bishops detected!\n";
			// loop over each bishop
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = blackNoKing | whitePieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						if (i % 2 == 0) continue; // only diagonal rays allowed
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						whiteAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH WHITE BISHOPS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for white rooks
			bitmoves = 0;
			tmp = pieceBoards[WR_INDEX];
			//if (!tmp) std::cout << "No white rooks detected!\n";
			// loop over each rook
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = blackNoKing | whitePieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						if (i % 2 == 1) continue; // only adjacent rays allowed
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						whiteAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH WHITE ROOKS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for white queens
			bitmoves = 0;
			tmp = pieceBoards[WQ_INDEX];
			//if (!tmp) std::cout << "No white queens detected!\n";
			// loop over each queen
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					uint64_t blockers = blackNoKing | whitePieces;
					// iterate over all rays
					for (int i = 0; i < 8; i++)
					{
						uint64_t maskedBlockers = rays[i][index] & blockers;
						unsigned long i2;
						unsigned char c;
						if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
						else c = _BitScanForward64(&i2, maskedBlockers);

						if (c)
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
						}
						else bitmoves = rays[i][index]; // no blockers

						whiteAttack |= bitmoves;
					}
				}
				else std::cout << "PROBLEM WITH WHITE QUEENS!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// for white king
			bitmoves = 0;
			tmp = pieceBoards[WK_INDEX];
			if (!tmp) std::cout << "ERROR! White king not detected!\n";
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = kingMoves[index];
					whiteAttack |= bitmoves;
				}
				else std::cout << "PROBLEM WITH WHITE KING!\n";

				// clear board of that bit
				tmp &= tmp - 1;
			}

			// count number of attackers
			uint64_t attackBoard = whiteAttackingKnights | whiteAttackingPawns | diagonalAttacks | adjAttacks;
			attackers += std::bitset<64>(attackBoard).count();

			// Next, check for number of attackers (0, 1, 2)
			uint64_t legalKingMoves;
			uint64_t checkRay;
			int attackerSquare;
			switch (attackers)
			{
				// double check
			case 2:
				//std::cout << "Double check!\n";
				// only king moves are allowed; mask them with the danger squares
				legalKingMoves = kingMoves[kingPos] & ~whiteAttack & ~blackPieces;
				bitsToMoves(legalKingMoves, kingPos, BLACK_KING, moves);
				break;

				// single check
			case 1:
				//std::cout << "Single check!\n";
				// Step 1: calculate legal king moves
				legalKingMoves = kingMoves[kingPos] & ~whiteAttack & ~blackPieces;
				bitsToMoves(legalKingMoves, kingPos, BLACK_KING, moves);

				// Step 2: calculate ways to capture or block the attacker
				attackerSquare = bitToIndex(attackBoard);
				checkRay = 0;

				// if checker is a pawn or knight, the check ray should only be the attacker square
				if (attackBoard & (pieceBoards[WP_INDEX] | pieceBoards[WN_INDEX])) checkRay = attackBoard;
				else
				{
					// first, figure out the ray along which the king is in check
					int rayIndex;
					for (int i = 0; i < 8; i++)
					{
						if (rays[i][kingPos] & attackBoard)
						{
							rayIndex = i;
							break;
						}
					}
					// then, calculate the ray from the king to the checker
					checkRay = rays[rayIndex][kingPos] & ~rays[rayIndex][attackerSquare]; // mask our regular ray with the checker's ray
				}

				// can any pawn move?
				bitmoves = 0;
				tmp = pieceBoards[BP_INDEX];
				//if (!tmp) std::cout << "No black pawns detected!\n";
				// loop over each pawn
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						// check for possible en passant move
						uint64_t passant = 0;
						if (enPassantInfo[0] != -1 && (1ULL << enPassantInfo[1]) & pieceBoards[WP_INDEX])
						{
							passant = (1ULL << enPassantInfo[0]);
							// check for rare en passant pin
							if (blackPawnAttacks[index] & passant)
							{
								// check if the left or right ray is in line with the pawn
								int rayIndex;
								if (rays[2][kingPos] & (1ULL << index)) rayIndex = 2;
								else if (rays[6][kingPos] & (1ULL << index)) rayIndex = 6;
								else rayIndex = -1;
								if (rayIndex != -1)
								{
									uint64_t whiteWithoutPassant = whitePieces & ~(1ULL << enPassantInfo[1]);;
									uint64_t blackWithoutPawn = blackPieces & ~(1ULL << index);
									uint64_t blockersEP = blackWithoutPawn | whiteWithoutPassant;
									uint64_t maskedBlockersEP = rays[rayIndex][kingPos] & blockersEP;
									unsigned long i2;
									unsigned char c;
									if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockersEP); // for negative rays
									else c = _BitScanForward64(&i2, maskedBlockersEP);

									if (c)
									{
										// check if the nearest "blocker" is a rook or queen
										if ((1ULL << i2) & (pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX])) passant = 0; // pin found!!
									}
									else; // do nothing; no pin

								}
							}
						}
						uint64_t pawnPushes = blackPawnPushes[index];
						// for first-row pawns
						if (index >= 48 && index < 56)
						{
							// if something blocks the pawn
							if ((1ULL << (index - 8)) & (whitePieces | blackPieces)) pawnPushes = 0;
						}
						// mask pushes with blockers, mask captures with white pieces
						bitmoves = (pawnPushes & ~(whitePieces | blackPieces)) |
							(blackPawnAttacks[index] & (whitePieces | passant));
						// make sure the pawn doesn't disobey a pin or disregard check
						bitmoves &= pinnedPieceMoves[index];
						bitmoves &= checkRay;
						// finally, check for en passant capture check evasion
						if ((attackBoard & pieceBoards[WP_INDEX]) && bitToIndex(attackBoard) == enPassantInfo[1]) bitmoves |= blackPawnAttacks[index] & passant;

						bitsToMoves(bitmoves, index, BLACK_PAWN, moves);
					}
					else std::cout << "PROBLEM WITH BLACK PAWNS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any knight move?
				bitmoves = 0;
				tmp = pieceBoards[BN_INDEX];
				//if (!tmp) std::cout << "No black knights detected!\n";
				// loop over each knight
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						// all-in-one check
						bitmoves = (knightMoves[index] & ~blackPieces & pinnedPieceMoves[index]) & checkRay;
						
						bitsToMoves(bitmoves, index, BLACK_KNIGHT, moves);
					}
					else std::cout << "PROBLEM WITH BLACK KNIGHTS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any bishop move?
				bitmoves = 0;
				tmp = pieceBoards[BB_INDEX];
				//if (!tmp) std::cout << "No black bishops detected!\n";
				// loop over each bishop
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							if (i % 2 == 0) continue; // only diagonal rays allowed
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							// now check the moves against the check ray
							bitmoves &= checkRay;

							bitsToMoves(bitmoves, index, BLACK_BISHOP, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK BISHOPS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any rook move?
				bitmoves = 0;
				tmp = pieceBoards[BR_INDEX];
				//if (!tmp) std::cout << "No black rooks detected!\n";
				// loop over each rook
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							if (i % 2 == 1) continue; // only adjacent rays allowed
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							// now check the moves against the check ray
							bitmoves &= checkRay;

							bitsToMoves(bitmoves, index, BLACK_ROOK, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK ROOKS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any queen move?
				bitmoves = 0;
				tmp = pieceBoards[BQ_INDEX];
				//if (!tmp) std::cout << "No black queens detected!\n";
				// loop over each queen
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							// now check the moves against the check ray
							bitmoves &= checkRay;

							bitsToMoves(bitmoves, index, BLACK_QUEEN, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK QUEENS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				break;


				// no checks
			case 0:
				//std::cout << "No check!\n";
				// Debugging info:
				//std::cout << "KING POSITION: " << kingPos << '\n';
				//std::cout << "PAWN BITBOARD: " << pieceBoards[BP_INDEX] << '\n';
				//std::cout << "KNIGHT BITBOARD: " << pieceBoards[BN_INDEX] << '\n';
				//std::cout << "BISHOP BITBOARD: " << pieceBoards[BB_INDEX] << '\n';
				//std::cout << "ROOK BITBOARD: " << pieceBoards[BR_INDEX] << '\n';
				//std::cout << "QUEEN BITBOARD: " << pieceBoards[BQ_INDEX] << '\n';
				//std::cout << "KING BITBOARD: " << pieceBoards[BK_INDEX] << '\n';
				// Step 1: calculate legal king moves
				legalKingMoves = 0;
				// we are now allowed to castle, so check for that
				if (blackKingCanCastle) // king hasn't moved
				{
					if (blackKingsideRookCanCastle) // kingside rook hasn't moved
					{
						uint64_t blackKingsideCastle = ((1ULL << (kingPos + 1)) | (1ULL << (kingPos + 2)));
						// this ternary ensures that the king isn't moving through pieces or check in the castle
						legalKingMoves |= (blackKingsideCastle & ~(whitePieces | blackPieces) & ~whiteAttack) == blackKingsideCastle ? (1ULL << (kingPos + 2)) : 0;
					}
					if (blackQueensideRookCanCastle)
					{
						uint64_t blackQueensideCastle = ((1ULL << (kingPos - 1)) | (1ULL << (kingPos - 2)));
						// this ternary ensures that the king isn't moving through pieces or check in the castle
						legalKingMoves |= (blackQueensideCastle & ~(whitePieces | blackPieces) & ~whiteAttack) == blackQueensideCastle ? (1ULL << (kingPos - 2)) : 0;
					}
				}
				// tack on any other legal king moves
				legalKingMoves |= kingMoves[kingPos] & ~whiteAttack & ~blackPieces;
				bitsToMoves(legalKingMoves, kingPos, BLACK_KING, moves);

				// Step 2: calculate all other pieces' moves

				// can any pawn move?
				bitmoves = 0;
				tmp = pieceBoards[BP_INDEX];
				//if (!tmp) std::cout << "No black pawns detected!\n";
				// loop over each pawn
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						// check for possible en passant move
						uint64_t passant = 0;
						if (enPassantInfo[0] != -1 && (1ULL << enPassantInfo[1]) & pieceBoards[WP_INDEX])
						{
							passant = (1ULL << enPassantInfo[0]);
							// check for rare en passant pin
							if (blackPawnAttacks[index] & passant)
							{
								// check if the left or right ray is in line with the pawn
								int rayIndex;
								if (rays[2][kingPos] & (1ULL << index)) rayIndex = 2;
								else if (rays[6][kingPos] & (1ULL << index)) rayIndex = 6;
								else rayIndex = -1;
								if (rayIndex != -1)
								{
									uint64_t whiteWithoutPassant = whitePieces & ~(1ULL << enPassantInfo[1]);;
									uint64_t blackWithoutPawn = blackPieces & ~(1ULL << index);
									uint64_t blockersEP = blackWithoutPawn | whiteWithoutPassant;
									uint64_t maskedBlockersEP = rays[rayIndex][kingPos] & blockersEP;
									unsigned long i2;
									unsigned char c;
									if (rayIndex < 7 && rayIndex > 2) c = _BitScanReverse64(&i2, maskedBlockersEP); // for negative rays
									else c = _BitScanForward64(&i2, maskedBlockersEP);

									if (c)
									{
										// check if the nearest "blocker" is a rook or queen
										if ((1ULL << i2) & (pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX])) passant = 0; // pin found!!
									}
									else; // do nothing; no pin

								}
							}
						}
						uint64_t pawnPushes = blackPawnPushes[index];
						// for first-row pawns
						if (index >= 48 && index < 56)
						{
							// if something blocks the pawn
							if ((1ULL << (index - 8)) & (whitePieces | blackPieces)) pawnPushes = 0;
						}
						// mask pushes with blockers, mask captures with white pieces
						bitmoves = (pawnPushes & ~(whitePieces | blackPieces)) |
							(blackPawnAttacks[index] & (whitePieces | passant));
						// make sure the pawn doesn't disobey a pin
						bitmoves &= pinnedPieceMoves[index];

						bitsToMoves(bitmoves, index, BLACK_PAWN, moves);
					}
					else std::cout << "PROBLEM WITH BLACK PAWNS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any knight move?
				bitmoves = 0;
				tmp = pieceBoards[BN_INDEX];
				//if (!tmp) std::cout << "No black knights detected!\n";
				// loop over each knight
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						// all-in-one check
						bitmoves = (knightMoves[index] & ~blackPieces & pinnedPieceMoves[index]);
						
						bitsToMoves(bitmoves, index, BLACK_KNIGHT, moves);
					}
					else std::cout << "PROBLEM WITH BLACK KNIGHTS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any bishop move?
				bitmoves = 0;
				tmp = pieceBoards[BB_INDEX];
				//if (!tmp) std::cout << "No black bishops detected!\n";
				// loop over each bishop
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							if (i % 2 == 0) continue; // only diagonal rays allowed
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							bitsToMoves(bitmoves, index, BLACK_BISHOP, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK BISHOPS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any rook move?
				bitmoves = 0;
				tmp = pieceBoards[BR_INDEX];
				//if (!tmp) std::cout << "No black rooks detected!\n";
				// loop over each rook
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							if (i % 2 == 1) continue; // only adjacent rays allowed
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							bitsToMoves(bitmoves, index, BLACK_ROOK, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK ROOKS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}

				// can any queen move?
				bitmoves = 0;
				tmp = pieceBoards[BQ_INDEX];
				//if (!tmp) std::cout << "No black queens detected!\n";
				// loop over each queen
				while (tmp)
				{
					unsigned long index;
					unsigned char code = _BitScanForward64(&index, tmp);
					if (code)
					{
						uint64_t blockers = whitePieces | blackPieces;
						// iterate over all rays
						for (int i = 0; i < 8; i++)
						{
							uint64_t maskedBlockers = rays[i][index] & blockers;
							unsigned long i2;
							unsigned char c;
							if (i < 7 && i > 2) c = _BitScanReverse64(&i2, maskedBlockers); // for negative rays
							else c = _BitScanForward64(&i2, maskedBlockers);

							if (c)
							{
								bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
								bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
							}
							else bitmoves = rays[i][index]; // no blockers

							// ensure we follow pin rules
							bitmoves &= pinnedPieceMoves[index];

							bitsToMoves(bitmoves, index, BLACK_QUEEN, moves);
						}
					}
					else std::cout << "PROBLEM WITH BLACK QUEENS!\n";

					// clear board of that bit
					tmp &= tmp - 1;
				}
				break;

			default:
				std::cout << "ERROR: Triple check (or higher) detected\n";
				std::cout << "Attackers detected: " << attackers << '\n';
			}
		}

		#else
		for (Piece* p : piecesOnBoard)
		{
			Move* potentialMove;
			if ((p->info & BLACK) == BLACK)
			{
				// go thru all pieces
				/* PAWN MOVES */
				if ((p->info & PAWN) == PAWN)
				{
					// one space up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
					// two spaces up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// capture left
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
					// capture right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						// check promotion
						if (potentialMove->isPromoting)
						{
							// make knight promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_KNIGHT;
							moves.push_back(potentialMove);
							// make bishop promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_BISHOP;
							moves.push_back(potentialMove);
							// make rook promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_ROOK;
							moves.push_back(potentialMove);
							// make queen promotion
							potentialMove = potentialMove->cloneMove();
							potentialMove->promoPiece = BLACK_QUEEN;
							moves.push_back(potentialMove);
						}
						else moves.push_back(potentialMove);
					}
				}

				/* KNIGHT MOVES */
				else if ((p->info & KNIGHT) == KNIGHT)
				{
					// up-left
					if (potentialMove =validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// up-right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// down-left
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// down-right
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// left-up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// left-down
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// right-up
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// right-down
					if (potentialMove = validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
				}

				/* BISHOP MOVES */
				else if ((p->info & BISHOP) == BISHOP)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check up-right diagonal
					while (tmpX <= 7 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check down-left diagonal
					while (tmpX >= 0 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check down-right diagonal
					while (tmpX <= 7 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY++;
					}
				}

				/* ROOK MOVES */
				else if ((p->info & ROOK) == ROOK)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check right
					while (tmpX <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
					}

					tmpX = initX, tmpY = initY;
					// check down
					while (tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check left
					while (tmpX >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
					}
				}

				/* QUEEN MOVES */
				else if ((p->info & QUEEN) == QUEEN) // fix
				{
					// CHECK DIAGONALS

					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check up-right diagonal
					while (tmpX <= 7 && tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check down-left diagonal
					while (tmpX >= 0 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check down-right diagonal
					while (tmpX <= 7 && tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
						tmpY++;
					}

					// CHECK ADJACENCIES

					tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY--;
					}

					tmpX = initX, tmpY = initY;
					// check right
					while (tmpX <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX++;
					}

					tmpX = initX, tmpY = initY;
					// check down
					while (tmpY <= 7)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpY++;
					}

					tmpX = initX, tmpY = initY;
					// check left
					while (tmpX >= 0)
					{
						if (potentialMove = validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							moves.push_back(potentialMove);
						}
						tmpX--;
					}
				}

				/* KING MOVES */
				else if ((p->info & KING) == KING)
				{
					int kingX = p->rect->x / 60, kingY = p->rect->y / 60;
					// move up
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, kingX * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move up-left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move up-right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY - 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, kingX * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down-left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move down-right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY + 1) * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move left
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 1) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// move right
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 1) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// castle kingside
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX + 2) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
					// castle queenside
					if (potentialMove = validMove(p, kingX * 60, kingY * 60, (kingX - 2) * 60, kingY * 60, true))
					{
						moves.push_back(potentialMove);
					}
				}
			}
		}
		#endif
	}

	return (ull) moves.size();
}

/*
	Returns a code representing the end of a game:
	-1 represents continuation
	0 represents stalemate
	1 represents checkmate
*/
int Game::isCheckmate(int turn)
{
	int kingX, kingY, kingIndex;
	Piece* king = nullptr;
	if (turn % 2 == 0) // for white
	{
		// get king index
		kingIndex = bitToIndex(pieceBoards[WK_INDEX]);

		/*
		// get king pointer
		for (Piece* p : piecesOnBoard)
		{
			if ((p->info & WHITE_KING) == WHITE_KING)
			{
				king = p;
				break;
			}
		}
		if (!king)
		{
			std::cout << "Couldn't find white king in isCheckmate!! Oh no!!\n";
			return 0;
		}
		// get king coordinates
		for (int i = 0; i < 8; i++)
		{
			bool breakout = false;
			for (int j = 0; j < 8; j++)
			{
				if ((board[i][j] & WHITE_KING) == WHITE_KING)
				{
					kingX = j;
					kingY = i;
					breakout = true;
					break;
				}
			}
			if (breakout) break;
		}
		*/
	}
	else
	{
		// get king index
		kingIndex = bitToIndex(pieceBoards[BK_INDEX]);
		/*
		// get king pointer
		for (Piece* p : piecesOnBoard)
		{
			if ((p->info & BLACK_KING) == BLACK_KING)
			{
				king = p;
				break;
			}
		}
		if (!king)
		{
			std::cout << "Couldn't find black king in isCheckmate!! Oh no!!\n";
			return 0;
		}
		// get king coordinates
		for (int i = 0; i < 8; i++)
		{
			bool breakout = false;
			for (int j = 0; j < 8; j++)
			{
				if ((board[i][j] & BLACK_KING) == BLACK_KING)
				{
					kingX = j;
					kingY = i;
					breakout = true;
					break;
				}
			}
			if (breakout) break;
		}
		*/
	}
	kingX = kingIndex % 8;
	kingY = 7 - kingIndex / 8;
	// get king pointer
	king = board[kingY][kingX];
	std::vector<Move*> moves;
	generateLegalMoves(moves);
	if (moves.size() == 0)
	{
		// now the king is trapped. is it stalemate though?
		if (isInCheck(turn % 2)) return 1; // checkmate, liberals
		else return 0; // stalemate, liberals
	}
	else // check for other types for draws
	{
		// check repetition
		for (auto str : positions)
		{
			if (str.second == 3)
			{
				return DRAW_BY_REPETITION;
			}
		}

		// next, check for 50 move rule
		if (halfmoves == 100)
		{
			return DRAW_BY_FIFTY;
		}

		// check for insufficient material
		if (insufficientMaterial())
		{
			return DRAW_BY_MATERIAL;
		}
		
		// nothing found, continue game

		// delete moves
		for (auto p : moves) delete p;
		return -1;
	}	
}

bool Game::insufficientMaterial()
{
	// check white
	bool darkWhiteBishop = false, lightWhiteBishop = false;
	int whiteKnights = 0;
	uint64_t tmp;


	#ifdef USING_BITS

	// first, check for pawn, rook, or queen
	if (pieceBoards[WP_INDEX] || pieceBoards[WR_INDEX] || pieceBoards[WQ_INDEX]) return false;
	
	// count knights
	whiteKnights = std::bitset<64>(pieceBoards[WN_INDEX]).count();

	// loop through bishops
	tmp = pieceBoards[WB_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			unsigned long file = index % 8 + 1;
			unsigned long rank = index / 8 + 1;
			// if both rank and file have the same parity, the square is dark
			if (file % 2 == rank % 2) darkWhiteBishop = true;
			else lightWhiteBishop = true;
		}
		else std::cout << "PROBLEM!!\n";

		// remove bishops
		tmp &= tmp - 1;

	}

	#else
	for (Piece* p : piecesOnBoard)
	{
		// common king + piece checkmates
		if ((p->info & WHITE_PAWN) == WHITE_PAWN || (p->info & WHITE_ROOK) == WHITE_ROOK || (p->info & WHITE_QUEEN) == WHITE_QUEEN) return false;
		else if ((p->info & WHITE_KNIGHT) == WHITE_KNIGHT) whiteKnights++;
		else if ((p->info & WHITE_BISHOP) == WHITE_BISHOP)
		{
			// light square; x+y sum is odd
			if ((p->rect->x / 60 + p->rect->y / 60) % 2) lightWhiteBishop = true;
			else darkWhiteBishop = true;
		}
	}
	
	#endif // USING_BITS
	
	// check all possible knight/bishop mates
	if (whiteKnights > 1 || (darkWhiteBishop && lightWhiteBishop) || (whiteKnights == 1 && (darkWhiteBishop || lightWhiteBishop))) return false;

	// check black
	bool darkBlackBishop = false, lightBlackBishop = false;
	int blackKnights = 0;

	#ifdef USING_BITS

	// first, check for pawn, rook, or queen
	if (pieceBoards[BP_INDEX] || pieceBoards[BR_INDEX] || pieceBoards[BQ_INDEX]) return false;
	
	// count knights
	whiteKnights = std::bitset<64>(pieceBoards[BN_INDEX]).count();

	// loop through bishops
	tmp = pieceBoards[BB_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			unsigned long file = index % 8 + 1;
			unsigned long rank = index / 8 + 1;
			// if both rank and file have the same parity, the square is dark
			if (file % 2 == rank % 2) darkWhiteBishop = true;
			else lightWhiteBishop = true;
		}
		else std::cout << "PROBLEM!!\n";

		// remove bishops
		tmp &= tmp - 1;

	}

	#else
	
	for (Piece* p : piecesOnBoard)
	{
		// common king + piece checkmates
		if ((p->info & BLACK_PAWN) == BLACK_PAWN || (p->info & BLACK_ROOK) == BLACK_ROOK || (p->info & BLACK_QUEEN) == BLACK_QUEEN) return false;
		else if ((p->info & BLACK_KNIGHT) == BLACK_KNIGHT) blackKnights++;
		else if ((p->info & BLACK_BISHOP) == BLACK_BISHOP)
		{
			// light square; x+y sum is odd
			if ((p->rect->x / 60 + p->rect->y / 60) % 2) lightBlackBishop = true;
			else darkBlackBishop = true;
		}
	}

	#endif // USING_BITS

	// check all possible knight/bishop mates
	if (blackKnights > 1 || (darkBlackBishop && lightBlackBishop) || (blackKnights == 1 && (darkBlackBishop || lightBlackBishop))) return false;

	// now, check for opposite colored bishops
	if ((darkWhiteBishop && lightBlackBishop) || (darkBlackBishop && lightWhiteBishop)) return false;
	

	// anything else is insufficient
	return true;

}

std::string Game::getFEN()
{
	std::string fenStr = "";
	// First, determine piece arrangement on board
	for (int i = 0; i < 8; i++)
	{
		int blanks = 0;
		for (int j = 0; j < 8; j++)
		{
			if (!(board[i][j]))
			{
				blanks++; // i got a blank space baby, and i'll add one to the count
				continue;
			}
			switch (board[i][j]->info)
			{
				case WHITE_PAWN:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "P";
					break;
				case WHITE_KNIGHT:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "N";
					break;
				case WHITE_BISHOP:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "B";
					break;
				case WHITE_ROOK:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "R";
					break;
				case WHITE_QUEEN:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "Q";
					break;
				case WHITE_KING:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "K";
					break;
				case BLACK_PAWN:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "p";
					break;
				case BLACK_KNIGHT:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "n";
					break;
				case BLACK_BISHOP:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "b";
					break;
				case BLACK_ROOK:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "r";
					break;
				case BLACK_QUEEN:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "q";
					break;
				case BLACK_KING:
					if (blanks > 0)
					{
						fenStr += std::to_string(blanks);
						blanks = 0;
					}
					fenStr += "k";
					break;
			}
		}
		if (blanks > 0) fenStr += std::to_string(blanks);
		if (i != 7) fenStr += "/"; // indicate next line
	}

	// next, whose turn is it?
	if (turn % 2 == 0) fenStr += " w ";
	else fenStr += " b ";

	// determine castling rights
	std::string castlingStr = "";
	
	// check white kingside castle
	if (whiteKingCanCastle && whiteKingsideRookCanCastle) castlingStr += "K";
	// check white queenside castle
	if (whiteKingCanCastle && whiteQueensideRookCanCastle) castlingStr += "Q";
	// check black kingside castle
	if (blackKingCanCastle && blackKingsideRookCanCastle) castlingStr += "k";
	// check black queenside castle
	if (blackKingCanCastle && blackQueensideRookCanCastle) castlingStr += "q";
	// if neither side can castle
	if (castlingStr == "") castlingStr = "-";
	castlingStr += " ";
	fenStr += castlingStr;

	// get en passant square
	bool noPassant = true;
	int passantCount = 0;
	std::string passantStr = "";
	for (Piece* p : piecesOnBoard)
	{
		if (p->enPassantable)
		{
			if (noPassant)
			{
				noPassant = false;
				// get chess board coordinates
				int xCoord = p->rect->x / 60, yCoord = 8 - (p->rect->y / 60);
				// adjust yCoord for passant sqaure based on color
				if ((p->info & WHITE) == WHITE) yCoord--;
				else yCoord++;
				char fileLetter = xCoord + 97;
				passantStr = fileLetter + std::to_string(yCoord) + " ";
				break;
			}
		}
	}
	if (noPassant) passantStr = "- ";
	fenStr += passantStr;

	// fifty-move-rule counter (halfmove clock)
	fenStr += std::to_string(halfmoves);

	// move counter
	fenStr += " " + std::to_string(turn / 2 + 1);

	return fenStr;
}

// Builds position from FEN string, ex: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
void Game::buildFromFEN(std::string fen)
{
	std::stringstream ss(fen);
	std::string layout;
	std::string turn;
	std::string castling;
	std::string enPassant;
	std::string halfs;
	std::string moves;
	ss >> layout >> turn >> castling >> enPassant >> halfs >> moves;

	// build layout of board
	layout += "/"; // for formatting purposes
	int pos = 0;
	for (int i = 0; i < 8; i++)
	{
		int end = layout.find("/", pos);
		std::string line = layout.substr(pos, end - pos);
		pos = end;
		pos++; // already set up for next line
		int x = 0;
		for (int j = 0; j < line.length(); j++)
		{
			switch (line[j])
			{
				case 'r':
					makePiece(x, i, BLACK_ROOK);
					x++;
					break;
				case 'b':
					makePiece(x, i, BLACK_BISHOP);
					x++;
					break;
				case 'n':
					makePiece(x, i, BLACK_KNIGHT);
					x++;
					break;
				case 'q':
					makePiece(x, i, BLACK_QUEEN);
					x++;
					break;
				case 'k':
					makePiece(x, i, BLACK_KING);
					x++;
					break;
				case 'p':
					makePiece(x, i, BLACK_PAWN);
					x++;
					break;
				case 'R':
					makePiece(x, i, WHITE_ROOK);
					x++;
					break;
				case 'B':
					makePiece(x, i, WHITE_BISHOP);
					x++;
					break;
				case 'N':
					makePiece(x, i, WHITE_KNIGHT);
					x++;
					break;
				case 'Q':
					makePiece(x, i, WHITE_QUEEN);
					x++;
					break;
				case 'K':
					makePiece(x, i, WHITE_KING);
					x++;
					break;
				case 'P':
					makePiece(x, i, WHITE_PAWN);
					x++;
					break;
				default:
					if (line[j] == '/') // grave mistake
					{
						std::cout << "Your position readings are incorrect!\n";
					}
					// add number of blank spaces
					x += line[j] - 48; // convert char to int
			}
		}
	}

	// determine castling rights

	// initially set all to false
	whiteKingCanCastle = false;
	whiteKingsideRookCanCastle = false;
	whiteQueensideRookCanCastle = false;
	blackKingCanCastle = false;
	blackKingsideRookCanCastle = false;
	blackQueensideRookCanCastle = false;
	for (int i = 0; i < castling.length(); i++)
	{
		switch (castling[i])
		{
			case '-':
				// no castling allowed!
				break;
			case 'K':
				whiteKingCanCastle = true;
				whiteKingsideRookCanCastle = true;
			case 'Q':
				whiteKingCanCastle = true;
				whiteQueensideRookCanCastle = true;
			case 'k':
				blackKingCanCastle = true;
				blackKingsideRookCanCastle = true;
			case 'q':
				blackKingCanCastle = true;
				blackQueensideRookCanCastle = true;
		}
	}

	// determine en passant piece
	if (enPassant != "-")
	{
		// get coordinates from rank-file notation
		int x = enPassant[0] - 97; // from file
		int y = 7 - (enPassant[1] - 49); // from rank
		// update passant square info
		enPassantInfo[0] = 8 * (7 - y) + x;

		// note: FEN stores the SQUARE that can be attacked, NOT the PIECE
		if (turn == "w") y++;
		else y--;
		// update passant piece square info
		enPassantInfo[1] = 8 * (7 - y) + x;

		for (Piece* p : piecesOnBoard)
		{
			if (p->rect->x == x * 60 && p->rect->y == y * 60)
			{
				p->enPassantable = true;
				break;
			}
		}
	}
	else
	{
		enPassantInfo[0] = -1;
		enPassantInfo[1] = -1;
	}

	// set halfmoves
	halfmoves = std::stoi(halfs);

	// set turn based on white/black to move
	turn = std::stoi(moves) * 2 + ((turn == "w") ? 0 : 1);

	// FEN and position initialization
	fens.push_back(fen);
	std::string currPos = fen.substr(0, fen.find(" "));
	if (positions.find(currPos) == positions.end()) positions[currPos] = 1;
	else positions[currPos]++;
	std::cout << "FEN: " << fen << "\n";
}

ull Game::perft(int depth /* assuming >= 1 */)
{
	std::vector<Move*> move_list;
	ull n_moves, i;
	ull nodes = 0;
	//std::string startingFen = getFEN();
	std::string currFen;

	n_moves = generateLegalMoves(move_list);

	if (depth == 1)
	{
		for (auto p : move_list) delete p;
		return (ull) n_moves;
	}

	for (i = 0; i < n_moves; i++)
	{
		//currFen = getFEN();
		//if (startingFen != currFen)
		//{
		//	std::cout << "Move was not undone correctly!\n";
		//	std::cout << "Good FEN: " << startingFen << '\n';
		//	std::cout << "Bad (current) FEN: " << currFen << '\n';
		//}
		makeMove(move_list[i]);
		nodes += perft(depth - 1);
		unmakeMove(move_list[i]);
	}
	for (auto p : move_list) delete p;
	return nodes;
}

ull Game::perftCaps(int depth, bool hasCapture /* assuming >= 1 */)
{
	if (depth == 0)
	{
		if (hasCapture) return (ull) 1;
		else return (ull) 0;
	}

	std::vector<Move*> move_list;
	ull n_moves, i;
	ull nodes = 0;

	n_moves = generateLegalMoves(move_list);


	for (i = 0; i < n_moves; i++) {
		hasCapture = move_list[i]->isCapture || hasCapture;
		makeMove(move_list[i]);
		nodes += perftCaps(depth - 1, hasCapture);
		unmakeMove(move_list[i]);
	}
	return nodes;
}

inline void Game::printBoard() const
{
	for (int i = 0; i < 8; i++)
	{
		std::string outStr = "[ ";
		for (int j = 0; j < 8; j++)
		{
			outStr += board[i][j] ? std::bitset< 8 >(board[i][j]->info).to_string() + " " : std::bitset< 8 >(0).to_string() + " ";
		}
		outStr += "]\n";
		std::cout << outStr;
	}
	std::cout << '\n'; // extra newline for readability
}

Game::~Game()
{
	for (auto p : piecesOnBoard) delete p;
}