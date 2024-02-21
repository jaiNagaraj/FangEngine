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
	// add to game board
	board[y][x] = info;
	// set up graphic rect
	SDL_Rect* dstRect = new SDL_Rect;
	dstRect->x = x * 60;
	dstRect->y = y * 60;
	dstRect->w = 60;
	dstRect->h = 60;
	// make piece, add it to board!
	Piece* tmp = new Piece(dstRect, info);
	piecesOnBoard.push_back(tmp);
	return tmp;
}

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
		while (posX >= 0 && posY < 8)
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
		while (posX >= 0)
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
		while (posX >= 0 && posY < 8)
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

void Game::updateAttackBoard()
{
	// reset attack boards
	std::fill(whiteAttack[0], whiteAttack[0] + 8 * 8, 0);
	std::fill(blackAttack[0], blackAttack[0] + 8 * 8, 0);
	// iterate thru board
	for (int y = 0; y < 8; y++) // y
	{
		for (int x = 0; x < 8; x++) // x
		{
			int tmpX, tmpY;
			switch (board[y][x])
			{
				case WHITE_PAWN:
					if (x > 0) whiteAttack[y - 1][x - 1]++; // attack up-left
					if (x < 7) whiteAttack[y - 1][x + 1]++; // attack up-right
					break;
				case BLACK_PAWN:
					if (x > 0) blackAttack[y + 1][x - 1]++; // attack down-left
					if (x < 7) blackAttack[y + 1][x + 1]++; // attack down-right
					break;
				case WHITE_KNIGHT:
					// attack up2left1
					if (y > 1 && x > 0) whiteAttack[y - 2][x - 1]++;
					// attack up2right1
					if (y > 1 && x < 7) whiteAttack[y - 2][x + 1]++;
					// attack right2up1
					if (y > 0 && x < 6) whiteAttack[y - 1][x + 2]++;
					// attack right2down1
					if (y < 7 && x < 6) whiteAttack[y + 1][x + 2]++;
					// attack down2left1
					if (y < 6 && x > 0) whiteAttack[y + 2][x - 1]++;
					// attack down2right1
					if (y < 6 && x < 7) whiteAttack[y + 2][x + 1]++;
					// attack left2up1
					if (y > 0 && x > 1) whiteAttack[y - 1][x - 2]++;
					// attack left2down1
					if (y < 7 && x > 1) whiteAttack[y + 1][x - 2]++;
					break;
				case BLACK_KNIGHT:
					// attack up2left1
					if (y > 1 && x > 0) blackAttack[y - 2][x - 1]++;
					// attack up2right1
					if (y > 1 && x < 7) blackAttack[y - 2][x + 1]++;
					// attack right2up1
					if (y > 0 && x < 6) blackAttack[y - 1][x + 2]++;
					// attack right2down1
					if (y < 7 && x < 6) blackAttack[y + 1][x + 2]++;
					// attack down2left1
					if (y < 6 && x > 0) blackAttack[y + 2][x - 1]++;
					// attack down2right1
					if (y < 6 && x < 7) blackAttack[y + 2][x + 1]++;
					// attack left2up1
					if (y > 0 && x > 1) blackAttack[y - 1][x - 2]++;
					// attack left2down1
					if (y < 7 && x > 1) blackAttack[y + 1][x - 2]++;
					break;
				case WHITE_BISHOP:
					// check up-left
					tmpX = x - 1, tmpY = y - 1;
					while (tmpX >= 0 && tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY--;
					}

					// check up-right
					tmpX = x + 1, tmpY = y - 1;
					while (tmpX <= 7 && tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY--;
					}

					// check down-right
					tmpX = x + 1, tmpY = y + 1;
					while (tmpX <= 7 && tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY++;
					}

					// check down-left
					tmpX = x - 1, tmpY = y + 1;
					while (tmpX >= 0 && tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY++;
					}
					break;
				case BLACK_BISHOP:
					// check up-left
					tmpX = x - 1, tmpY = y - 1;
					while (tmpX >= 0 && tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY--;
					}

					// check up-right
					tmpX = x + 1, tmpY = y - 1;
					while (tmpX <= 7 && tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY--;
					}

					// check down-right
					tmpX = x + 1, tmpY = y + 1;
					while (tmpX <= 7 && tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY++;
					}

					// check down-left
					tmpX = x - 1, tmpY = y + 1;
					while (tmpX >= 0 && tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY++;
					}
					break;
				case WHITE_ROOK:
					// check up
					tmpX = x, tmpY = y - 1;
					while (tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY--;
					}

					// check right
					tmpX = x + 1, tmpY = y;
					while (tmpX <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++;
					}

					// check down
					tmpX = x, tmpY = y + 1;
					while (tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY++;
					}

					// check left
					tmpX = x - 1, tmpY = y;
					while (tmpX >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--;
					}
					break;
				case BLACK_ROOK:
					// check up
					tmpX = x, tmpY = y - 1;
					while (tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY--;
					}

					// check right
					tmpX = x + 1, tmpY = y;
					while (tmpX <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++;
					}

					// check down
					tmpX = x, tmpY = y + 1;
					while (tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY++;
					}

					// check left
					tmpX = x - 1, tmpY = y;
					while (tmpX >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--;
					}
					break;
				case WHITE_QUEEN:
					/* CHECK DIAGONALS*/

					// check up-left
					tmpX = x - 1, tmpY = y - 1;
					while (tmpX >= 0 && tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY--;
					}

					// check up-right
					tmpX = x + 1, tmpY = y - 1;
					while (tmpX <= 7 && tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY--;
					}

					// check down-right
					tmpX = x + 1, tmpY = y + 1;
					while (tmpX <= 7 && tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY++;
					}

					// check down-left
					tmpX = x - 1, tmpY = y + 1;
					while (tmpX >= 0 && tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY++;
					}

					/* CHECK ADJACENCIES */

					// check up
					tmpX = x, tmpY = y - 1;
					while (tmpY >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY--;
					}

					// check right
					tmpX = x + 1, tmpY = y;
					while (tmpX <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++;
					}

					// check down
					tmpX = x, tmpY = y + 1;
					while (tmpY <= 7)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY++;
					}

					// check left
					tmpX = x - 1, tmpY = y;
					while (tmpX >= 0)
					{
						whiteAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--;
					}

					break;
				case BLACK_QUEEN:
					/* CHECK DIAGONALS*/

					// check up-left
					tmpX = x - 1, tmpY = y - 1;
					while (tmpX >= 0 && tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY--;
					}

					// check up-right
					tmpX = x + 1, tmpY = y - 1;
					while (tmpX <= 7 && tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY--;
					}

					// check down-right
					tmpX = x + 1, tmpY = y + 1;
					while (tmpX <= 7 && tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++; tmpY++;
					}

					// check down-left
					tmpX = x - 1, tmpY = y + 1;
					while (tmpX >= 0 && tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--; tmpY++;
					}

					/* CHECK ADJACENCIES */

					// check up
					tmpX = x, tmpY = y - 1;
					while (tmpY >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY--;
					}

					// check right
					tmpX = x + 1, tmpY = y;
					while (tmpX <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX++;
					}

					// check down
					tmpX = x, tmpY = y + 1;
					while (tmpY <= 7)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpY++;
					}

					// check left
					tmpX = x - 1, tmpY = y;
					while (tmpX >= 0)
					{
						blackAttack[tmpY][tmpX]++;
						// if space isn't empty
						if (board[tmpY][tmpX]) break; // if space isn't empty
						tmpX--;
					}

					break;
				case WHITE_KING:
					if (y > 0) whiteAttack[y - 1][x]++;
					if (y > 0 && x < 7) whiteAttack[y - 1][x + 1]++;
					if (x < 7) whiteAttack[y][x + 1]++;
					if (y < 7 && x < 7) whiteAttack[y + 1][x + 1]++;
					if (y < 7) whiteAttack[y + 1][x]++;
					if (y < 7 && x > 0) whiteAttack[y + 1][x - 1]++;
					if (x > 0) whiteAttack[y][x - 1]++;
					if (y > 0 && x > 0) whiteAttack[y - 1][x - 1]++;
					break;
				case BLACK_KING:
					if (y > 0) blackAttack[y - 1][x]++;
					if (y > 0 && x < 7) blackAttack[y - 1][x + 1]++;
					if (x < 7) blackAttack[y][x + 1]++;
					if (y < 7 && x < 7) blackAttack[y + 1][x + 1]++;
					if (y < 7) blackAttack[y + 1][x]++;
					if (y < 7 && x > 0) blackAttack[y + 1][x - 1]++;
					if (x > 0) blackAttack[y][x - 1]++;
					if (y > 0 && x > 0) blackAttack[y - 1][x - 1]++;
					break;
			}
		}
	}
}

bool Game::validCastle(Piece* piece, int initX, int initY, int kingX, int kingY)
{
	// if king isn't on back rank
	if (((piece->info & WHITE) == WHITE && initY != 7) || ((piece->info & BLACK) == BLACK && initY != 0)) return false;

	// remaining checks for white
	if ((piece->info & WHITE) == WHITE)
	{
		// if king can't castle
		if (!whiteKingCanCastle) return false;
		//std::cout << "White king ok to castle\n";

		// check queenside castle
		if (initX > kingX)
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[7][0] & WHITE_ROOK) == WHITE_ROOK && board[7][1] == 0 && board[7][2] == 0 && board[7][3] == 0)) return false;
			//std::cout << "Nothing blocking white\n";

			// if we've moved the rook
			if (!whiteQueensideRookCanCastle) return false;
			//std::cout << "White queen rook ok to castle\n";

			//bool valid = false; // for checking if the rook is all good (safety precaution)
			//for (Piece* p : piecesOnBoard)
			//{
			//	// found left white rook at (0,420)
			//	if (p->rect->x == 0 && p->rect->y == 420)
			//	{
			//		if (!(p->canCastle)) return false;
			//		else
			//		{
			//			valid = true;
			//			break;
			//		}
			//	}
			//}
			//if (!valid)
			//{
			//	std::cout << "Uh oh spaghetti O's";
			//	return false; // for some reason rook isn't there; problem!!
			//}

			// make sure king won't be in check throughout!
			if (isInCheck(board, 0, 4, 7)) return false;

			uint8_t tmp;
			tmp = board[7][3];
			board[7][3] = board[7][4];
			board[7][4] = 0;
			if (isInCheck(board, 0, 3, 7))
			{
				// reset
				board[7][4] = board[7][3];
				board[7][3] = tmp;
				return false;
			}
			// reset
			board[7][4] = board[7][3];
			board[7][3] = tmp;

			tmp = board[7][2];
			board[7][2] = board[7][3];
			board[7][3] = 0;
			if (isInCheck(board, 0, 2, 7))
			{
				// reset
				board[7][3] = board[7][2];
				board[7][2] = tmp;
				return false;
			}
			// reset
			board[7][3] = board[7][2];
			board[7][2] = tmp;

			// looks good!
			return true;
		}
		// check kingside castle
		else
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[7][7] & WHITE_ROOK) == WHITE_ROOK && board[7][6] == 0 && board[7][5] == 0)) return false;

			//std::cout << "Nothing blocking white\n";

			// if we've moved the rook
			if (!whiteKingsideRookCanCastle) return false;
			//std::cout << "White king rook ok to castle\n";

			//bool valid = false; // for checking if the rook is all good (safety precaution)
			//for (Piece* p : piecesOnBoard)
			//{
			//	// found right white rook at (0,420)
			//	if (p->rect->x == 420 && p->rect->y == 420)
			//	{
			//		if (!(p->canCastle)) return false;
			//		else
			//		{
			//			valid = true;
			//			break;
			//		}
			//	}
			//}
			//if (!valid)
			//{
			//	std::cout << "Uh oh spaghetti O's";
			//	return false; // for some reason rook isn't there; problem!!
			//}

			// make sure king won't be in check throughout!
			if (isInCheck(board, 0, 4, 7)) return false;

			uint8_t tmp;

			tmp = board[7][5];
			board[7][5] = board[7][4];
			board[7][4] = 0;
			if (isInCheck(board, 0, 5, 7))
			{
				// reset
				board[7][4] = board[7][5];
				board[7][5] = tmp;
				return false;
			}
			// reset
			board[7][4] = board[7][5];
			board[7][5] = tmp;

			tmp = board[7][6];
			board[7][6] = board[7][5];
			board[7][5] = 0;
			if (isInCheck(board, 0, 6, 7))
			{
				// reset
				board[7][5] = board[7][6];
				board[7][6] = tmp;
				return false;
			}
			// reset
			board[7][5] = board[7][6];
			board[7][6] = tmp;


			// looks good!
			return true;
		}
	}
	// remaining checks for black
	else
	{
		// if the king has moved
		if (!blackKingCanCastle) return false;

		// check queenside castle
		if (initX > kingX)
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[0][0] & BLACK_ROOK) == BLACK_ROOK && board[0][1] == 0 && board[0][2] == 0 && board[0][3] == 0)) return false;

			// if we've moved the rook before
			if (!blackQueensideRookCanCastle) return false;

			//bool valid = false; // for checking if the rook is all good (safety precaution)
			//for (Piece* p : piecesOnBoard)
			//{
			//	// found left black rook at (0,0)
			//	if (p->rect->x == 0 && p->rect->y == 0)
			//	{
			//		if (!(p->canCastle)) return false;
			//		else
			//		{
			//			valid = true;
			//			break;
			//		}
			//	}
			//}
			//if (!valid)
			//{
			//	std::cout << "Uh oh spaghetti O's";
			//	return false; // for some reason rook isn't there; problem!!
			//}

			// make sure king won't be in check throughout!
			if (isInCheck(board, 1, 4, 0)) return false;

			uint8_t tmp;

			tmp = board[0][3];
			board[0][3] = board[0][4];
			board[0][4] = 0;
			if (isInCheck(board, 1, 3, 0))
			{
				// reset
				board[0][4] = board[0][3];
				board[0][3] = tmp;
				return false;
			}
			// reset
			board[0][4] = board[0][3];
			board[0][3] = tmp;

			tmp = board[0][2];
			board[0][2] = board[0][3];
			board[0][3] = 0;
			if (isInCheck(board, 1, 2, 0))
			{
				// reset
				board[0][3] = board[0][2];
				board[0][2] = tmp;
				return false;
			}
			// reset
			board[0][3] = board[0][2];
			board[0][2] = tmp;

			// looks good!
			return true;
		}
		// check kingside castle
		else
		{
			// if the rook isn't there and/or there are pieces blocking
			if (!((board[0][7] & BLACK_ROOK) == BLACK_ROOK && board[0][6] == 0 && board[0][5] == 0)) return false;

			// if we've moved the rook
			if (!blackKingsideRookCanCastle) return false;

			//bool valid = false; // for checking if the rook is all good (safety precaution)
			//for (Piece* p : piecesOnBoard)
			//{
			//	// found right black rook at (420,0)
			//	if (p->rect->x == 420 && p->rect->y == 0)
			//	{
			//		if (!(p->canCastle)) return false;
			//		else
			//		{
			//			valid = true;
			//			break;
			//		}
			//	}
			//}
			//if (!valid)
			//{
			//	std::cout << "Uh oh spaghetti O's";
			//	return false; // for some reason rook isn't there; problem!!
			//}

			// make sure king won't be in check throughout!
			if (isInCheck(board, 1, 4, 0)) return false;

			uint8_t tmp;

			tmp = board[0][5];
			board[0][5] = board[0][4];
			board[0][4] = 0;
			if (isInCheck(board, 1, 5, 0))
			{
				// reset
				board[0][4] = board[0][5];
				board[0][5] = tmp;
				return false;
			}
			// reset
			board[0][4] = board[0][5];
			board[0][5] = tmp;

			tmp = board[0][6];
			board[0][6] = board[0][5];
			board[0][5] = 0;
			if (isInCheck(board, 1, 6, 0))
			{
				// reset
				board[0][5] = board[0][6];
				board[0][6] = tmp;
				return false;
			}
			// reset
			board[0][5] = board[0][6];
			board[0][6] = tmp;

			// looks good!
			return true;
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
}

/*
	Function to execute a Move object and update game data
*/
void Game::makeMove(Move* move)
{
	int distMovedX = move->newX - move->oldX;
	int distMovedY = move->newY - move->oldY;
	if (move->isCapture)
	{
		// use the combination of erase and remove to capture piece
		std::vector<Piece*>::iterator it = std::find(piecesOnBoard.begin(), piecesOnBoard.end(), move->captured);
		if (it != piecesOnBoard.end()) piecesOnBoard.erase(it);
		else std::cout << "Error: Could not capture piece!!!\n";
		//std::cout << "Piece removed!\n";
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
				else if (move->oldX == 420) whiteKingsideRookCanCastle = false;
				break;
			case BLACK_ROOK:
				if (move->oldX == 0) blackQueensideRookCanCastle = false;
				else if (move->oldX == 420) blackKingsideRookCanCastle = false;
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
	
	// if pawn passes checks for two spaces
	if ((move->piece->info & PAWN) == PAWN && abs(distMovedY) == 2) move->piece->enPassantable = true;

	// if pawn move or capture, reset halfmove counter
	if (move->isCapture || (move->piece->info & PAWN) == PAWN) halfmoves = 0;
	else halfmoves++;

	// if en passant, update board to reflect this
	if (move->isEP) board[move->oldY][move->newX] = 0;

	// if promotion
	if (move->isPromoting)
	{
		// update board and piece info
		move->piece->info = move->promoPiece;
		board[move->oldY][move->oldX] = move->piece->info;
	}

	// update board and turn
	board[move->newY][move->newX] = board[move->oldY][move->oldX];
	board[move->oldY][move->oldX] = 0;
	turn++;

	// update piece graphic coordinates
	move->piece->rect->x = move->newX * 60;
	move->piece->rect->y = move->newY * 60;

	// finaly, update fen and position data
	move->fen = getFEN();
	fens.push_back(move->fen);
	std::string pos = move->fen.substr(0, move->fen.find(" "));
	if (positions.count(pos) == 0) positions[pos] = 1;
	else positions[pos]++;
	//std::cout << "FEN: " << move->fen << "\n";
}

void Game::unmakeMove(Move* move)
{
	int distMovedX = move->newX - move->oldX;
	int distMovedY = move->newY - move->oldY;
	if (move->isCapture)
	{
		// Add the captured piece back
		piecesOnBoard.push_back(move->captured);
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
						break;
					}
				}
			}
		}
	}

	// reset previously enPassantable piece
	if (move->lossOfEP)
	{
		move->lostEP->enPassantable = true;
	}

	// reset castling rights
	if (move->oldCastlingRights & WK_CASTLE) whiteKingCanCastle = true;
	if (move->oldCastlingRights & WQR_CASTLE) whiteQueensideRookCanCastle = true;
	if (move->oldCastlingRights & WKR_CASTLE) whiteKingsideRookCanCastle = true;
	if (move->oldCastlingRights & BK_CASTLE) blackKingCanCastle = true;
	if (move->oldCastlingRights & BQR_CASTLE) blackQueensideRookCanCastle = true;
	if (move->oldCastlingRights & BKR_CASTLE) blackKingsideRookCanCastle = true;

	// if the piece was pawn that moved two spaces, un-passant it
	if ((move->piece->info & PAWN) == PAWN && abs(distMovedY) == 2) move->piece->enPassantable = false;

	// reinstate halfmove counter
	halfmoves = move->oldHalfmoves;

	// if en passant, put passanted pawn back on board
	if (move->isEP) board[move->oldY][move->newX] = move->captured->info;

	// if promotion, unpromote
	if (move->isPromoting)
	{
		// update board and piece info
		if ((turn - 1) % 2 == 0) move->piece->info = WHITE_PAWN;
		else move->piece->info = BLACK_PAWN;
		board[move->newY][move->newX] = move->piece->info;
	}

	// update board and turn
	board[move->oldY][move->oldX] = board[move->newY][move->newX];
	if (move->isCapture && !(move->isEP))
	{
		board[move->newY][move->newX] = move->captured->info;
	}
	else board[move->newY][move->newX] = 0;
	turn--;

	// reset piece graphic coordinates
	move->piece->rect->x = move->oldX * 60;
	move->piece->rect->y = move->oldY * 60;

	// finaly, delete fen and position data
	fens.pop_back(); // assuming we unmake right after we make...
	std::string pos = move->fen.substr(0, move->fen.find(" "));
	if (positions.count(pos) == 0) std::cout << "Something is afoot!\n";
	else positions[pos]--; // the position should already exist, so just decrease counter
	if (positions[pos] == 0) positions.erase(pos); // prevent unnecessarily large map
}

std::vector<Move*> Game::bitsToMoves(uint64_t bitboard)
{
	std::vector<Move*> moves;
	return moves;
}

ull Game::generateLegalMoves(std::vector<Move*>& moves)
{
	uint64_t bitmoves = 0;
	uint64_t whitePieces = 0;
	uint64_t blackPieces = 0;
	whitePieces |= pieceBoards[WP_INDEX] | pieceBoards[WK_INDEX] | pieceBoards[WB_INDEX] | pieceBoards[WR_INDEX] | pieceBoards[WQ_INDEX] | pieceBoards[WK_INDEX];
	blackPieces |= pieceBoards[BP_INDEX] | pieceBoards[BK_INDEX] | pieceBoards[BB_INDEX] | pieceBoards[BR_INDEX] | pieceBoards[BQ_INDEX] | pieceBoards[BK_INDEX];
	if (turn % 2 == 0) // for white
	{
		bool usingBits = false;
		if (usingBits)
		{
			// for white pawns
			bitmoves = 0;
			uint64_t tmp = pieceBoards[WP_INDEX];
			if (!tmp) std::cout << "No white pawns detected!\n";
			// loop over each pawn
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = (whitePawnPushes[index] | whitePawnAttacks[index]) & blackPieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH WHITE PAWNS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}
		
			// for white knights
			bitmoves = 0;
			tmp = pieceBoards[WK_INDEX];
			if (!tmp) std::cout << "No white knights detected!\n";
			// loop over each knight
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = knightMoves[index] & blackPieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH WHITE KNIGHTS!\n";
		
				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for white bishops
			bitmoves = 0;
			tmp = pieceBoards[WB_INDEX];
			if (!tmp) std::cout << "No white bishops detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH WHITE BISHOPS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for white rooks
			bitmoves = 0;
			tmp = pieceBoards[WR_INDEX];
			if (!tmp) std::cout << "No white rooks detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH WHITE ROOKS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for white queens
			bitmoves = 0;
			tmp = pieceBoards[WQ_INDEX];
			if (!tmp) std::cout << "No white queens detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~whitePieces; // ensure that we aren't capturing a white blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH WHITE QUEENS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for white king
			bitmoves = 0;
			tmp = pieceBoards[WK_INDEX];
			if (!tmp) std::cout << "ERROR! White king not detected!\n";
			// loop over each king
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = kingMoves[index] & blackPieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH WHITE KING!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}
		}


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
	}
	else // for black
	{
		bool usingBits = false;
		if (usingBits)
		{
			// for black pawns
			bitmoves = 0;
			uint64_t tmp = pieceBoards[BP_INDEX];
			if (!tmp) std::cout << "No black pawns detected!\n";
			// loop over each pawn
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = (blackPawnPushes[index] | blackPawnAttacks[index]) & whitePieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH BLACK PAWNS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for black knights
			bitmoves = 0;
			tmp = pieceBoards[BK_INDEX];
			if (!tmp) std::cout << "No black knights detected!\n";
			// loop over each knight
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = knightMoves[index] & whitePieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH BLACK KNIGHTS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for black bishops
			bitmoves = 0;
			tmp = pieceBoards[BB_INDEX];
			if (!tmp) std::cout << "No black bishops detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH BLACK BISHOPS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for black rooks
			bitmoves = 0;
			tmp = pieceBoards[BR_INDEX];
			if (!tmp) std::cout << "No black rooks detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH BLACK ROOKS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for black queens
			bitmoves = 0;
			tmp = pieceBoards[WQ_INDEX];
			if (!tmp) std::cout << "No black queens detected!\n";
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
						if (_BitScanForward64(&i2, maskedBlockers))
						{
							bitmoves = rays[i][index] & ~rays[i][i2]; // mask our regular ray with our blocker's ray
							bitmoves &= ~blackPieces; // ensure that we aren't capturing a black blocker
						}
						else bitmoves = rays[i][index];

						// insert bits-to-moves function here
					}
				}
				else std::cout << "PROBLEM WITH BLACK QUEENS!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}

			// for black king
			bitmoves = 0;
			tmp = pieceBoards[WK_INDEX];
			if (!tmp) std::cout << "ERROR! Black king not detected!\n";
			while (tmp)
			{
				unsigned long index;
				unsigned char code = _BitScanForward64(&index, tmp);
				if (code)
				{
					bitmoves = kingMoves[index] & whitePieces;
					// insert bits-to-moves function here
				}
				else std::cout << "PROBLEM WITH BLACK KING!\n";

				// clear board of that bit
				tmp = (tmp >> (index + 1)) << (index + 1);
			}
		}


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
	int kingX, kingY;
	Piece* king = nullptr;
	if (turn % 2 == 0) // for white
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
	}
	else
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
	}
	std::vector<Move*> moves;
	generateLegalMoves(moves);
	if (moves.size() == 0)
	{
		// now the king is trapped. is it stalemate though?
		if (isInCheck(board, turn % 2, kingX, kingY)) return 1; // checkmate, liberals
		else return 0; // stalemate, liberals
	}
	else
	{
		// delete moves
		for (auto p : moves) delete p;
		return -1;
	}
}

int Game::oldIsCheckmate(int turn)
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
	if (whiteKnights > 1 || (darkWhiteBishop && lightWhiteBishop) || (whiteKnights == 1 && (darkWhiteBishop || lightWhiteBishop))) return false;

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

		// note: FEN stores the SQUARE that can be attacked, NOT the PIECE
		if (turn == "w") y++;
		else y--;

		for (Piece* p : piecesOnBoard)
		{
			if (p->rect->x == x * 60 && p->rect->y == y * 60)
			{
				p->enPassantable = true;
				break;
			}
		}
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

Game::~Game()
{
	for (auto p : piecesOnBoard) delete p;
}