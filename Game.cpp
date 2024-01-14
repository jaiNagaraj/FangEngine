#include "Game.h"
#include "Piece.h"
#include "Move.h"
#include <bitset>
#include <string>
typedef unsigned long long ull;

bool Game::isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY)
{
	// White's turn, check white king
	if (turn == 0)
	{
		// check for pawn checks!
		if (kingY > 0 && ((kingX > 0 && (gameBoard[kingY - 1][kingX - 1] & BLACK_PAWN) == BLACK_PAWN)
			|| (kingX < 7 && (gameBoard[kingY - 1][kingX + 1] & BLACK_PAWN) == BLACK_PAWN))) return true;

		// check for knight checks!
		// check up2left1
		if (kingY > 1 && kingX > 0 && (gameBoard[kingY - 2][kingX - 1] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check up2right1
		if (kingY > 1 && kingX < 7 && (gameBoard[kingY - 2][kingX + 1] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check right2up1
		if (kingY > 0 && kingX < 6 && (gameBoard[kingY - 1][kingX + 2] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check right2down1
		if (kingY < 7 && kingX < 6 && (gameBoard[kingY + 1][kingX + 2] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check down2left1
		if (kingY < 6 && kingX > 0 && (gameBoard[kingY + 2][kingX - 1] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check down2right1
		if (kingY < 6 && kingX < 7 && (gameBoard[kingY + 2][kingX + 1] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check left2up1
		if (kingY > 0 && kingX > 1 && (gameBoard[kingY - 1][kingX - 2] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;
		// check left2down1
		if (kingY < 7 && kingX > 1 && (gameBoard[kingY + 1][kingX - 2] & BLACK_KNIGHT) == BLACK_KNIGHT) return true;

		int posX, posY; // declare temp position vars

		// check for diagonal checks!
		// check up-left:
		posX = kingX - 1; posY = kingY - 1;
		while (posX >= 0 && posY >= 0)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX--; posY--;
		}
		// check up-right:
		posX = kingX + 1; posY = kingY - 1;
		while (posX < 8 && posY >= 0)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX++; posY--;
		}
		// check down-right:
		posX = kingX + 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX++; posY++;
		}
		// check down-left:
		posX = kingX - 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX--; posY++;
		}

		// check for rank/file checks!
		// check up:
		posX = kingX; posY = kingY - 1;
		while (posY >= 0)
		{
			if ((gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posY--;
		}
		// check right:
		posX = kingX + 1; posY = kingY;
		while (posX < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX++;
		}
		// check down:
		posX = kingX; posY = kingY + 1;
		while (posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posY++;
		}
		// check left:
		posX = kingX - 1; posY = kingY;
		while (posX < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_ROOK) == BLACK_ROOK || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & WHITE) == WHITE ||
				(gameBoard[posY][posX] & BLACK_PAWN) == BLACK_PAWN ||
				(gameBoard[posY][posX] & BLACK_KNIGHT) == BLACK_KNIGHT ||
				(gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP ||
				(gameBoard[posY][posX] & BLACK_KING) == BLACK_KING) break;
			posX--;
		}

		// check for king checks!
		// start at the top and cycle clockwise
		if ((kingY > 0 && (gameBoard[kingY - 1][kingX] & BLACK_KING) == BLACK_KING) ||
			(kingY > 0 && kingX < 7 && (gameBoard[kingY - 1][kingX + 1] & BLACK_KING) == BLACK_KING) ||
			(kingX < 7 && (gameBoard[kingY][kingX + 1] & BLACK_KING) == BLACK_KING) ||
			(kingY < 7 && kingX < 7 && (gameBoard[kingY + 1][kingX + 1] & BLACK_KING) == BLACK_KING) ||
			(kingY < 7 && (gameBoard[kingY + 1][kingX] & BLACK_KING) == BLACK_KING) ||
			(kingY < 7 && kingX > 0 && (gameBoard[kingY + 1][kingX - 1] & BLACK_KING) == BLACK_KING) ||
			(kingX > 0 && (gameBoard[kingY][kingX - 1] & BLACK_KING) == BLACK_KING) ||
			(kingY > 0 && kingX > 0 && (gameBoard[kingY - 1][kingX - 1] & BLACK_KING) == BLACK_KING)) return true;

		// passed the gauntlet
		return false;
	}
	// black's turn, check black king
	else
	{
		// check for pawn checks!
		if (kingY < 7 && ((kingX > 0 && (gameBoard[kingY + 1][kingX - 1] & WHITE_PAWN) == WHITE_PAWN)
			|| (kingX < 7 && (gameBoard[kingY + 1][kingX + 1] & WHITE_PAWN) == WHITE_PAWN))) return true;

		// check for knight checks!
		// check up2left1
		if (kingY > 1 && kingX > 0 && (gameBoard[kingY - 2][kingX - 1] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check up2right1
		if (kingY > 1 && kingX < 7 && (gameBoard[kingY - 2][kingX + 1] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check right2up1
		if (kingY > 0 && kingX < 6 && (gameBoard[kingY - 1][kingX + 2] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check right2down1
		if (kingY < 7 && kingX < 6 && (gameBoard[kingY + 1][kingX + 2] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check down2left1
		if (kingY < 6 && kingX > 0 && (gameBoard[kingY + 2][kingX - 1] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check down2right1
		if (kingY < 6 && kingX < 7 && (gameBoard[kingY + 2][kingX + 1] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check left2up1
		if (kingY > 0 && kingX > 1 && (gameBoard[kingY - 1][kingX - 2] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;
		// check left2down1
		if (kingY < 7 && kingX > 1 && (gameBoard[kingY + 1][kingX - 2] & WHITE_KNIGHT) == WHITE_KNIGHT) return true;

		int posX, posY; // declare temp position vars

		// check for diagonal checks!
		// check up-left:
		posX = kingX - 1; posY = kingY - 1;
		while (posX >= 0 && posY >= 0)
		{
			if ((gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX--; posY--;
		}
		// check up-right:
		posX = kingX + 1; posY = kingY - 1;
		while (posX < 8 && posY >= 0)
		{
			if ((gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX++; posY--;
		}
		// check down-right:
		posX = kingX + 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX++; posY++;
		}
		// check down-left:
		posX = kingX - 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX--; posY++;
		}

		// check for rank/file checks!
		// check up:
		posX = kingX; posY = kingY - 1;
		while (posY >= 0)
		{
			if ((gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posY--;
		}
		// check right:
		posX = kingX + 1; posY = kingY;
		while (posX < 8)
		{
			if ((gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX++;
		}
		// check down:
		posX = kingX; posY = kingY + 1;
		while (posY < 8)
		{
			if ((gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posY++;
		}
		// check left:
		posX = kingX - 1; posY = kingY;
		while (posX >= 0)
		{
			if ((gameBoard[posY][posX] & WHITE_ROOK) == WHITE_ROOK || (gameBoard[posY][posX] & WHITE_QUEEN) == WHITE_QUEEN) return true;
			// if another piece blocks a check
			if ((gameBoard[posY][posX] & BLACK) == BLACK ||
				(gameBoard[posY][posX] & WHITE_PAWN) == WHITE_PAWN ||
				(gameBoard[posY][posX] & WHITE_KNIGHT) == WHITE_KNIGHT ||
				(gameBoard[posY][posX] & WHITE_BISHOP) == WHITE_BISHOP ||
				(gameBoard[posY][posX] & WHITE_KING) == WHITE_KING) break;
			posX--;
		}

		// check for king checks!
		// start at the top and cycle clockwise
		if ((kingY > 0 && (gameBoard[kingY - 1][kingX] & WHITE_KING) == WHITE_KING) ||
			(kingY > 0 && kingX < 7 && (gameBoard[kingY - 1][kingX + 1] & WHITE_KING) == WHITE_KING) ||
			(kingX < 7 && (gameBoard[kingY][kingX + 1] & WHITE_KING) == WHITE_KING) ||
			(kingY < 7 && kingX < 7 && (gameBoard[kingY + 1][kingX + 1] & WHITE_KING) == WHITE_KING) ||
			(kingY < 7 && (gameBoard[kingY + 1][kingX] & WHITE_KING) == WHITE_KING) ||
			(kingY < 7 && kingX > 0 && (gameBoard[kingY + 1][kingX - 1] & WHITE_KING) == WHITE_KING) ||
			(kingX > 0 && (gameBoard[kingY][kingX - 1] & WHITE_KING) == WHITE_KING) ||
			(kingY > 0 && kingX > 0 && (gameBoard[kingY - 1][kingX - 1] & WHITE_KING) == WHITE_KING)) return true;

		// passed the gauntlet
		return false;
	}
}

bool Game::validCastle(Piece* piece, int initX, int initY, int kingX, int kingY)
{
	// if king can't castle
	if (!(piece->canCastle)) return false;

	// if king isn't on back rank
	if (((piece->info & WHITE) == WHITE && initY != 7) || ((piece->info & BLACK) == BLACK && initY != 0)) return false;

	// remaining checks for white
	if ((piece->info & WHITE) == WHITE)
	{
		// check queenside castle
		if (initX > kingX)
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[7][0] & WHITE_ROOK) == WHITE_ROOK && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0)) return false;
			else
			{
				bool valid = false; // for checking if the rook is all good (safety precaution)
				for (Piece* p : piecesOnBoard)
				{
					// found left white rook at (0,420)
					if (p->rect->x == 0 && p->rect->y == 420)
					{
						if (!(p->canCastle)) return false;
						else
						{
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					std::cout << "Uh oh spaghetti O's";
					return false; // for some reason rook isn't there; problem!!
				}

				// make sure king won't be in check throughout!
				// open up parallel universe where king is moving thru castle
				uint8_t altBoard[8][8];
				std::copy(&board[0][0], &board[0][0] + 8 * 8, &altBoard[0][0]);
				if (isInCheck(altBoard, 0, 4, 7)) return false;

				altBoard[7][3] = altBoard[7][4];
				altBoard[7][4] = 0;
				if (isInCheck(altBoard, 0, 3, 7)) return false;

				altBoard[7][2] = altBoard[7][3];
				altBoard[7][3] = 0;
				if (isInCheck(altBoard, 0, 2, 7)) return false;

				// looks good!
				return true;
			}
		}
		// check kingside castle
		else
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[7][7] & WHITE_ROOK) == WHITE_ROOK && board[7][6] == 0 && board[7][5] == 0)) return false;
			else
			{
				bool valid = false; // for checking if the rook is all good (safety precaution)
				for (Piece* p : piecesOnBoard)
				{
					// found right white rook at (0,420)
					if (p->rect->x == 420 && p->rect->y == 420)
					{
						if (!(p->canCastle)) return false;
						else
						{
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					std::cout << "Uh oh spaghetti O's";
					return false; // for some reason rook isn't there; problem!!
				}

				// make sure king won't be in check throughout!
				// open up parallel universe where king is moving thru castle
				uint8_t altBoard[8][8];
				std::copy(&board[0][0], &board[0][0] + 8 * 8, &altBoard[0][0]);
				if (isInCheck(altBoard, 0, 4, 7)) return false;

				altBoard[7][5] = altBoard[7][4];
				altBoard[7][4] = 0;
				if (isInCheck(altBoard, 0, 5, 7)) return false;

				altBoard[7][6] = altBoard[7][5];
				altBoard[7][5] = 0;
				if (isInCheck(altBoard, 0, 6, 7)) return false;


				// looks good!
				return true;
			}
		}
	}
	// remaining checks for black
	else
	{
		// check queenside castle
		if (initX > kingX)
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[0][0] & BLACK_ROOK) == BLACK_ROOK && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0)) return false;
			else
			{
				bool valid = false; // for checking if the rook is all good (safety precaution)
				for (Piece* p : piecesOnBoard)
				{
					// found left black rook at (0,0)
					if (p->rect->x == 0 && p->rect->y == 0)
					{
						if (!(p->canCastle)) return false;
						else
						{
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					std::cout << "Uh oh spaghetti O's";
					return false; // for some reason rook isn't there; problem!!
				}

				// make sure king won't be in check throughout!
				// open up parallel universe where king is moving thru castle
				uint8_t altBoard[8][8];
				std::copy(&board[0][0], &board[0][0] + 8 * 8, &altBoard[0][0]);
				if (isInCheck(altBoard, 1, 4, 0)) return false;
				altBoard[0][3] = altBoard[0][4];
				altBoard[0][4] = 0;
				if (isInCheck(altBoard, 1, 3, 0)) return false;
				altBoard[0][2] = altBoard[0][3];
				altBoard[0][3] = 0;
				if (isInCheck(altBoard, 1, 2, 0)) return false;

				// looks good!
				return true;
			}
		}
		// check kingside castle
		else
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[0][7] & BLACK_ROOK) == BLACK_ROOK && board[0][6] == 0 && board[0][5] == 0)) return false;
			else
			{
				bool valid = false; // for checking if the rook is all good (safety precaution)
				for (Piece* p : piecesOnBoard)
				{
					// found right black rook at (420,0)
					if (p->rect->x == 420 && p->rect->y == 0)
					{
						if (!(p->canCastle)) return false;
						else
						{
							valid = true;
							break;
						}
					}
				}
				if (!valid)
				{
					std::cout << "Uh oh spaghetti O's";
					return false; // for some reason rook isn't there; problem!!
				}

				// make sure king won't be in check throughout!
				// open up parallel universe where king is moving thru castle
				uint8_t altBoard[8][8];
				std::copy(&board[0][0], &board[0][0] + 8 * 8, &altBoard[0][0]);
				if (isInCheck(altBoard, 1, 4, 0)) return false;
				altBoard[0][5] = altBoard[0][4];
				altBoard[0][4] = 0;
				if (isInCheck(altBoard, 1, 5, 0)) return false;
				altBoard[0][6] = altBoard[0][5];
				altBoard[0][5] = 0;
				if (isInCheck(altBoard, 1, 6, 0)) return false;

				// looks good!
				return true;
			}
		}
	}
}

/*
	Checks if a move is valid. If so, returns Move object, else returns null.
*/
Move* Game::validMove(Piece* piece, int oldX, int oldY, int newX, int newY, bool test)
{
	int oldXCoord = oldX / 60, oldYCoord = oldY / 60, newXCoord = newX / 60, newYCoord = newY / 60;
	//std::cout << oldX << " " << oldY << " " << newX << " " << newY << '\n';
	//std::cout << oldXCoord << " " << oldYCoord << " " << newXCoord << " " << newYCoord << '\n';
	//std::cout << "TURN: " << (turn % 2 == 0 ? "White" : "Black") << '\n';

	bool isCapturing = false;
	bool isEP = false;
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
						if (p->rect->x == newX && p->rect->y == oldYCoord * 60)
						{
							// if that piece was enPassantable
							if (p->enPassantable)
							{
								isEP = true;
								passantPiece = p;
								// board[oldYCoord][newXCoord] = 0; // gotta update bitboard too!
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
						if (p->rect->x == newX && p->rect->y == oldYCoord * 60)
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
			// passes checks; make it vulnerable to en passant
			else piece->enPassantable = true;
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
			if (validCastle(piece, oldXCoord, oldYCoord, newXCoord, newYCoord))
			{
				// preemptively move rook to correct spot
				if ((piece->info & WHITE) == WHITE) // for white
				{
					if (newXCoord < oldXCoord) // queenside castle
					{
						for (Piece* p : piecesOnBoard)
						{
							// if we found the left rook
							if ((p->info & WHITE_ROOK) == WHITE_ROOK && p->rect->x == 0)
							{
								p->rect->x = (newXCoord + 1) * 60; // move to the right of the king
								board[newYCoord][newXCoord + 1] = board[7][0];
								board[7][0] = 0;
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
								p->rect->x = (newXCoord - 1) * 60; // move to the left of the king
								board[newYCoord][newXCoord - 1] = board[7][7];
								board[7][7] = 0;
							}
						}
					}
				}
				else // for black
				{
					if (newXCoord < oldXCoord) // queenside castle
					{
						for (Piece* p : piecesOnBoard)
						{
							// if we found the left rook
							if ((p->info & BLACK_ROOK) == BLACK_ROOK && p->rect->x == 0)
							{
								p->rect->x = (newXCoord + 1) * 60; // move to the right of the king
								board[newYCoord][newXCoord + 1] = board[0][0];
								board[0][0] = 0;
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
								p->rect->x = (newXCoord - 1) * 60; // move to the left of the king
								board[newYCoord][newXCoord - 1] = board[0][7];
								board[0][7] = 0;
							}
						}
					}
				}
			}
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
	// open up parallel universe where move has been made
	uint8_t altBoard[8][8];
	std::copy(&board[0][0], &board[0][0] + 8 * 8, &altBoard[0][0]);
	altBoard[newYCoord][newXCoord] = altBoard[oldYCoord][oldXCoord];
	altBoard[oldYCoord][oldXCoord] = 0;

	int kingX, kingY;
	// for white
	if (turn % 2 == 0)
	{
		for (int i = 0; i < 8; i++)
		{
			bool breakout = false;
			for (int j = 0; j < 8; j++)
			{
				if ((altBoard[i][j] & WHITE_KING) == WHITE_KING)
				{
					kingX = j;
					kingY = i;
					breakout = true;
					break;
				}
			}
			if (breakout) break;
		}
		if (isInCheck(altBoard, 0, kingX, kingY)) return nullptr;
	}
	// for black
	else
	{
		for (int i = 0; i < 8; i++)
		{
			bool breakout = false;
			for (int j = 0; j < 8; j++)
			{
				if ((altBoard[i][j] & BLACK_KING) == BLACK_KING)
				{
					kingX = j;
					kingY = i;
					breakout = true;
					break;
				}
			}
			if (breakout) break;
		}
		if (isInCheck(altBoard, 1, kingX, kingY)) return nullptr;
	}


	// checks valid; create Move object
	Move* move = new Move;
	move->isCapture = isCapturing;
	move->isEP = isEP;
	move->piece = piece;
	move->newX = newXCoord;
	move->newY = newYCoord;
	move->oldX = oldXCoord;
	move->oldY = oldYCoord;
	if (move->isCapture)
	{
		if (move->isEP) move->captured = passantPiece;
		else
		{
			for (Piece* p : piecesOnBoard)
			{
				if (p->rect->x == newX && p->rect->y == newY && p != piece) move->captured = p;
			}
		}
	}
	else move->captured = nullptr;

	// passed the gauntlet!
	return move;
}

/*
	Function to execute a Move object and update game data
*/
void Game::makeMove(Move* move)
{
	int distMovedX = move->newX - move->oldX;
	int distMovedY = move->newY - move->oldY;
	// if pawn passes checks for two spaces
	if ((move->piece->info & PAWN) == PAWN && abs(distMovedY) == 2) move->piece->enPassantable = true;
	if (move->isCapture)
	{
		// use the combination of erase and remove to capture piece
		piecesOnBoard.erase(std::remove(piecesOnBoard.begin(), piecesOnBoard.end(), move->captured), piecesOnBoard.end());
		//std::cout << "Piece removed!\n";
	}

	// reset all previously enPassantable pieces
	for (Piece* piece : piecesOnBoard)
	{
		// if it is white's turn
		if (turn % 2 == 0 && (piece->info & BLACK) == BLACK)
		{
			piece->enPassantable = false;
		}
		// if it is black's turn
		if (turn % 2 == 1 && (piece->info & WHITE) == WHITE)
		{
			piece->enPassantable = false;
		}
	}

	// make sure rooks/kings can't castle after movement
	move->piece->canCastle = false;

	// if pawn move or capture, reset halfmove counter
	if (move->isCapture || (move->piece->info & PAWN) == PAWN) halfmoves = 0;
	else halfmoves++;

	// check for end-rank promotion in white
	if ((move->piece->info & WHITE_PAWN) == WHITE_PAWN && move->newY == 0)
	{
		isPromoting = true;
		promotingPiece = move->piece;
	}
	// check for end-rank promotion in white
	else if ((move->piece->info & BLACK_PAWN) == BLACK_PAWN && move->newY == 7)
	{
		isPromoting = true;
		promotingPiece = move->piece;
	}

	// if en passant, update board to reflect this
	if (move->isEP) board[move->oldY][move->newX] = 0;

	// update board and turn
	board[move->newY][move->newX] = board[move->oldY][move->oldX];
	board[move->oldY][move->oldX] = 0;
	turn++;

	// finaly, update fen and position data
	std::string fen = getFEN();
	fens.push_back(fen);
	std::string pos = fen.substr(0, fen.find(" "));
	if (positions.find(pos) == positions.end()) positions[pos] = 1;
	else positions[pos]++;
	std::cout << "FEN: " << fen << "\n";
}

/*
	Returns a code representing the end of a game:
	-1 represents continuation
	0 represents stalemate
	1 represents checkmate
*/
int Game::isCheckmate(int turn)
{
	int kingX, kingY;
	Piece* king = nullptr;
	// for white
	if (turn % 2 == 0)
	{
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
		// First check if king is trapped; there are no legal moves.
		bool trapped = true;
		// KING MOVES
		if (kingY > 0)
		{
			// move up
			if (validMove(king, kingX * 60, kingY * 60, kingX * 60, (kingY - 1) * 60, true)) trapped = false;
			if (kingX > 0)
			{
				// move up-left
				if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY - 1) * 60, true)) trapped = false;
			}
			if (kingX < 7)
			{
				// move up-right
				if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY - 1) * 60, true)) trapped = false;
			}
		}
		if (kingY < 7)
		{
			// move down
			if (validMove(king, kingX * 60, kingY * 60, kingX * 60, (kingY + 1) * 60, true)) trapped = false;
			if (kingX > 0)
			{
				// move down-left
				if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY + 1) * 60, true)) trapped = false;
			}
			if (kingX < 7)
			{
				// move down-right
				if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY + 1) * 60, true)) trapped = false;
			}
		}
		if (kingX > 0)
		{
			// move left
			if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, kingY * 60, true)) trapped = false;
		}
		if (kingX < 7)
		{
			// move right
			if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, kingY * 60, true)) trapped = false;
		}

		if (!trapped) return -1;

		// the king can't move; cycle thru possible moves
		bool reallyTrapped = true;
		for (Piece* p : piecesOnBoard)
		{
			if ((p->info & WHITE) == WHITE)
			{
				// go thru all pieces
				/* PAWN MOVES */
				if ((p->info & PAWN) == PAWN)
				{
					// one space up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// two spaces up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// capture left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// capture right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}

					if (!reallyTrapped) return -1;
				}

				/* KNIGHT MOVES */
				else if ((p->info & KNIGHT) == KNIGHT)
				{
					// up-left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// up-right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// down-left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// down-right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// left-up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// left-down
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// right-up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// right-down
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					if (!reallyTrapped) return -1;
				}

				/* BISHOP MOVES */
				else if ((p->info & BISHOP) == BISHOP)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpX--;
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check up-right diagonal
						while (tmpX <= 7 && tmpY >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY--;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-left diagonal
						while (tmpX >= 0 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-right diagonal
						while (tmpX <= 7 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY++;
						}
					}

					if (!reallyTrapped) return -1;
				}

				/* ROOK MOVES */
				else if ((p->info & ROOK) == ROOK)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check right
						while (tmpX <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down
						while (tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check left
						while (tmpX >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
						}
					}

					if (!reallyTrapped) return -1;
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
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpX--;
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check up-right diagonal
						while (tmpX <= 7 && tmpY >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY--;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-left diagonal
						while (tmpX >= 0 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-right diagonal
						while (tmpX <= 7 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY++;
						}
					}

					// CHECK ADJACENCIES

					tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check right
						while (tmpX <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down
						while (tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check left
						while (tmpX >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
						}
					}

					if (!reallyTrapped) return -1;
				}

			}
		}
		// ok the king really is trapped, but just check to make sure
		if (!reallyTrapped) return -1;

		// now the king is trapped. is it stalemate though?
		if (isInCheck(board, 0, kingX, kingY)) return 1; // checkmate, liberals
		else return 0; // stalemate, liberals
	}
	// for black
	else // fix
	{
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
		// First check if king is trapped; there are no legal moves.
		bool trapped = true;
		// KING MOVES
		if (kingY > 0)
		{
			// move up
			if (validMove(king, kingX * 60, kingY * 60, kingX * 60, (kingY - 1) * 60, true)) trapped = false;
			if (kingX > 0)
			{
				// move up-left
				if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY - 1) * 60, true)) trapped = false;
			}
			if (kingX < 7)
			{
				// move up-right
				if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY - 1) * 60, true)) trapped = false;
			}
		}
		if (kingY < 7)
		{
			// move down
			if (validMove(king, kingX * 60, kingY * 60, kingX * 60, (kingY + 1) * 60, true)) trapped = false;
			if (kingX > 0)
			{
				// move down-left
				if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, (kingY + 1) * 60, true)) trapped = false;
			}
			if (kingX < 7)
			{
				// move down-right
				if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, (kingY + 1) * 60, true)) trapped = false;
			}
		}
		if (kingX > 0)
		{
			// move left
			if (validMove(king, kingX * 60, kingY * 60, (kingX - 1) * 60, kingY * 60, true)) trapped = false;
		}
		if (kingX < 7)
		{
			// move right
			if (validMove(king, kingX * 60, kingY * 60, (kingX + 1) * 60, kingY * 60, true)) trapped = false;
		}

		if (!trapped) return -1;

		// the king can't move; cycle thru possible moves
		bool reallyTrapped = true;
		for (Piece* p : piecesOnBoard)
		{
			if ((p->info & BLACK) == BLACK)
			{
				// go thru all pieces
				/* PAWN MOVES */
				if ((p->info & PAWN) == PAWN)
				{
					// one space up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// two spaces up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60)) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// capture left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// capture right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}

					if (!reallyTrapped) return -1;
				}

				/* KNIGHT MOVES */
				else if ((p->info & KNIGHT) == KNIGHT)
				{
					// up-left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// up-right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) - 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// down-left
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// down-right
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 1) * 60, ((p->rect->y / 60) + 2) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// left-up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// left-down
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) - 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// right-up
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) - 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					// right-down
					if (validMove(p, p->rect->x, p->rect->y, ((p->rect->x / 60) + 2) * 60, ((p->rect->y / 60) + 1) * 60, true))
					{
						reallyTrapped = false;
						break;
					}
					if (!reallyTrapped) return -1;
				}

				/* BISHOP MOVES */
				else if ((p->info & BISHOP) == BISHOP)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up-left diagonal
					while (tmpX >= 0 && tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpX--;
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check up-right diagonal
						while (tmpX <= 7 && tmpY >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY--;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-left diagonal
						while (tmpX >= 0 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-right diagonal
						while (tmpX <= 7 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY++;
						}
					}

					if (!reallyTrapped) return -1;
				}

				/* ROOK MOVES */
				else if ((p->info & ROOK) == ROOK)
				{
					int initX = p->rect->x / 60, initY = p->rect->y / 60;
					int tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check right
						while (tmpX <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down
						while (tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check left
						while (tmpX >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
						}
					}

					if (!reallyTrapped) return -1;
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
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpX--;
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check up-right diagonal
						while (tmpX <= 7 && tmpY >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY--;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-left diagonal
						while (tmpX >= 0 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down-right diagonal
						while (tmpX <= 7 && tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
							tmpY++;
						}
					}

					// CHECK ADJACENCIES

					tmpX = initX, tmpY = initY;
					// check up
					while (tmpY >= 0)
					{
						if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
						{
							reallyTrapped = false;
							break;
						}
						tmpY--;
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check right
						while (tmpX <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check down
						while (tmpY <= 7)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpY++;
						}
					}
					if (reallyTrapped) // make sure we don't do unnecessary checks
					{
						tmpX = initX, tmpY = initY;
						// check left
						while (tmpX >= 0)
						{
							if (validMove(p, initX * 60, initY * 60, tmpX * 60, tmpY * 60, true))
							{
								reallyTrapped = false;
								break;
							}
							tmpX--;
						}
					}

					if (!reallyTrapped) return -1;
				}

			}
		}
		// ok the king really is trapped, but just check to make sure
		if (!reallyTrapped) return -1;

		// now the king is trapped. is it stalemate though?
		if (isInCheck(board, 1, kingX, kingY)) return 1; // checkmate, liberals
		else return 0; // stalemate, liberals 
	}
}

bool Game::insufficientMaterial()
{
	// check white
	bool darkWhiteBishop = false, lightWhiteBishop = false;
	int whiteKnights = 0;
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
	// check all possible knight/bishop mates
	if (whiteKnights > 1 || (darkWhiteBishop && lightWhiteBishop)) return false;

	// check black
	bool darkBlackBishop = false, lightBlackBishop = false;
	int blackKnights = 0;
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
	// check all possible knight/bishop mates
	if (blackKnights > 1 || (darkBlackBishop && lightBlackBishop)) return false;

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
			switch (board[i][j])
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
				default:
					blanks++; // i got a blank space baby, and i'll add one to the count
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
	Piece* queenRookWhite = nullptr;
	Piece* kingRookWhite = nullptr;
	Piece* kingWhite = nullptr;
	Piece* queenRookBlack = nullptr;
	Piece* kingRookBlack = nullptr;
	Piece* kingBlack = nullptr;
	// get pieces
	for (Piece* p : piecesOnBoard)
	{
		switch (p->info)
		{
			case WHITE_KING:
				kingWhite = p;
				break;
			case BLACK_KING:
				kingBlack = p;
				break;
			case WHITE_ROOK:
				if (p->rookSide == 0) queenRookWhite = p;
				else kingRookWhite = p;
				break;
			case BLACK_ROOK:
				if (p->rookSide == 0) queenRookBlack = p;
				else kingRookBlack = p;
				break;
		}
	}
	// check white kingside castle
	if (kingRookWhite && kingRookWhite->canCastle && kingWhite->canCastle) castlingStr += "K";
	// check white queenside castle
	if (queenRookWhite && queenRookWhite->canCastle && kingWhite->canCastle) castlingStr += "Q";
	// check black kingside castle
	if (kingRookBlack && kingRookBlack->canCastle && kingBlack->canCastle) castlingStr += "k";
	// check black queenside castle
	if (queenRookBlack && queenRookBlack->canCastle && kingBlack->canCastle) castlingStr += "q";
	// if neither side can castle
	if (castlingStr == "") castlingStr = "-";
	castlingStr += " ";
	fenStr += castlingStr;

	// get en passant square
	bool noPassant = true;
	std::string passantStr = "";
	for (Piece* p : piecesOnBoard)
	{
		if (p->enPassantable)
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
	if (noPassant) passantStr = "- ";
	fenStr += passantStr;

	// fifty-move-rule counter (halfmove clock)
	fenStr += std::to_string(halfmoves);

	// move counter
	fenStr += " " + std::to_string(turn / 2 + 1);

	return fenStr;
}

void Game::printBoard()
{
	for (int i = 0; i < 8; i++)
	{
		std::string outStr = "[ ";
		for (int j = 0; j < 8; j++)
		{
			outStr += std::bitset< 8 >(board[i][j]).to_string() + " ";
		}
		outStr += "]\n";
		std::cout << outStr;
	}
	std::cout << '\n'; // extra newline for readability
}