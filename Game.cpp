#include "Game.h"
#include "Piece.h"
#include <bitset>
#include <string>

bool isInCheck(uint8_t gameBoard[][8], int turn, int kingX, int kingY)
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
			posX--; posY--;
		}
		// check up-right:
		posX = kingX + 1; posY = kingY - 1;
		while (posX < 8 && posY >= 0)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posX++; posY--;
		}
		// check down-right:
		posX = kingX + 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posX++; posY++;
		}
		// check down-left:
		posX = kingX - 1; posY = kingY + 1;
		while (posX < 8 && posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posX--; posY++;
		}

		// check for rank/file checks!
		// check up:
		posX = kingX; posY = kingY - 1;
		while (posY >= 0)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posY--;
		}
		// check right:
		posX = kingX + 1; posY = kingY;
		while (posX < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posX++;
		}
		// check down:
		posX = kingX; posY = kingY + 1;
		while (posY < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
			posY++;
		}
		// check left:
		posX = kingX - 1; posY = kingY;
		while (posX < 8)
		{
			if ((gameBoard[posY][posX] & BLACK_BISHOP) == BLACK_BISHOP || (gameBoard[posY][posX] & BLACK_QUEEN) == BLACK_QUEEN) return true;
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
}

bool Game::validMove(Piece* piece, int oldX, int oldY, int newX, int newY)
{
	int oldXCoord = oldX / 60, oldYCoord = oldY / 60, newXCoord = newX / 60, newYCoord = newY / 60;
	std::cout << oldX << " " << oldY << " " << newX << " " << newY << '\n';
	std::cout << oldXCoord << " " << oldYCoord << " " << newXCoord << " " << newYCoord << '\n';
	std::cout << "TURN: " << (turn % 2 == 0 ? "White" : "Black") << '\n';

	bool isCapturing = false;

	// if it's white's turn but black played
	if (turn % 2 == 0 && !(piece->info & WHITE)) return false;
	// if it's black's turn but white played
	if (turn % 2 != 0 && !(piece->info & BLACK)) return false;

	int distMovedX = newXCoord - oldXCoord;
	int distMovedY = newYCoord - oldYCoord;
	//std::cout << distMovedX << " " << distMovedY << '\n';

	/*
			********PAWN MOVES********
	*/

	if ((piece->info & WHITE_PAWN) == WHITE_PAWN)
	{
		// if the pawn moved impossibly
		if ((abs(distMovedX) > 1) || (distMovedY > -1) || (distMovedY < -2)) return false;

		//if the pawn wants to capture
		if (abs(distMovedX) == 1)
		{
			// if it didn't move diagonally by one
			if (!(distMovedY == -1)) return false;
			// if the capturing square does not hold a black piece
			if ((board[newYCoord][newXCoord] & BLACK) != BLACK) return false;
			isCapturing = true;
		}

		// if the pawn was on the starting row
		else if (oldYCoord == 6)
		{
			// if it didn't move 1 or 2 spaces forwards
			if (!((distMovedY == -1) || (distMovedY == -2))) return false;
			// if something is blocking the pawn
			if (!(board[newYCoord][newXCoord] == 0b00000000) || (distMovedY == -2 && !(board[newYCoord + 1][newXCoord] == 0b00000000))) return false;
		}
		// if something is blocking the pawn
		else if (!(distMovedY == -1) || !(board[newYCoord][newXCoord] == 0b00000000)) return false;
	}
	else if ((piece->info & BLACK_PAWN) == BLACK_PAWN)
	{

		// if the pawn moved impossibly
		if ((abs(distMovedX) > 1) || (distMovedY < 1) || (distMovedY > 2)) return false;

		//if the pawn wants to capture
		if (abs(distMovedX) == 1)
		{
			// if it didn't move diagonally by one
			if (!(distMovedY == 1)) return false;
			// if the capturing square does not hold a white piece
			if ((board[newYCoord][newXCoord] & WHITE) != WHITE) return false;
			isCapturing = true;
		}

		// if the pawn was on the starting row
		else if (oldYCoord == 1)
		{
			// if it didn't move 1 or 2 spaces forwards
			if (!((distMovedY == 1) || (distMovedY == 2))) return false;
			// if something is blocking the pawn
			if (!(board[newYCoord][newXCoord] == 0b00000000) || (distMovedY == 2 && !(board[newYCoord - 1][newXCoord] == 0b00000000))) return false;
		}
		// if something is blocking the pawn
		else if (!(distMovedY == 1) || !(board[newYCoord][newXCoord] == 0b00000000)) return false;
	}

	/*
			********KNIGHT MOVES********
	*/


	else if ((piece->info & KNIGHT) == KNIGHT)
	{
		// if horsey didnt move in L shape :(
		if (!((abs(distMovedX) == 2 && abs(distMovedY) == 1) || (abs(distMovedY) == 2 && abs(distMovedX) == 1))) return false;

		//if the knight wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE) 
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return false;
			isCapturing = true;
		}
	}

	/*
			********BISHOP MOVES********
	*/


	else if ((piece->info & BISHOP) == BISHOP)
	{
		// if bishop doesn't move diagonally
		if (abs(distMovedX) != abs(distMovedY)) return false;

		//if the bishop wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return false;
			isCapturing = true;
		}

		// check for any obstructions in bishop's path
		int tmpX = oldXCoord, tmpY = oldYCoord;
		tmpX += newXCoord > oldXCoord ? 1 : -1;
		tmpY += newYCoord > oldYCoord ? 1 : -1;
		while (tmpX != newXCoord && tmpY != newYCoord)
		{
			if (board[tmpY][tmpX] != 0b00000000) return false;
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
		if (!((abs(distMovedX) > 0 && abs(distMovedY) == 0) || (abs(distMovedY) > 0 && abs(distMovedX) == 0))) return false;

		//if the rook wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return false;
			isCapturing = true;
		}

		// check for any obstructions in rook's path
		int tmpX = oldXCoord, tmpY = oldYCoord;
		if (abs(distMovedX) > 0)
		{
			tmpX += newXCoord > oldXCoord ? 1 : -1;
			while (tmpX != newXCoord)
			{
				if (board[tmpY][tmpX] != 0b00000000) return false;
				tmpX += newXCoord > oldXCoord ? 1 : -1;
			}
		}
		else
		{
			tmpY += newYCoord > oldYCoord ? 1 : -1;
			while (tmpY != newYCoord)
			{
				if (board[tmpY][tmpX] != 0b00000000) return false;
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
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)) return false;
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
					if (board[tmpY][tmpX] != 0b00000000) return false;
					tmpX += newXCoord > oldXCoord ? 1 : -1;
				}
			}
			else
			{
				tmpY += newYCoord > oldYCoord ? 1 : -1;
				while (tmpY != newYCoord)
				{
					if (board[tmpY][tmpX] != 0b00000000) return false;
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
				if (board[tmpY][tmpX] != 0b00000000) return false;
				tmpX += newXCoord > oldXCoord ? 1 : -1;
				tmpY += newYCoord > oldYCoord ? 1 : -1;
			}
		}
		// if queen moves impossibly
		else return false;
	}

	/*
			********KING MOVES********
	*/


	else if ((piece->info & KING) == KING)
	{
		//if the king wants to capture
		if (!(board[newYCoord][newXCoord] == 0b00000000))
		{
			// if the capturing square does not hold a different-color piece OR if the piece is the king
			if (((board[newYCoord][newXCoord] & BLACK) != BLACK && (piece->info & WHITE) == WHITE)
				|| ((board[newYCoord][newXCoord] & WHITE) != WHITE && (piece->info & BLACK) == BLACK)
				|| ((board[newYCoord][newXCoord] & KING) == KING)) return false;
			isCapturing = true;
		}
		// if the king moves more than a square
		if (abs(distMovedX) > 1 || abs(distMovedY) > 1) return false;
	}
	




	if (isCapturing)
	{
		// remove piece from board vector
		uint8_t capturedPiece = board[newYCoord][newXCoord];
		//std::cout << std::bitset< 8 >(capturedPiece).to_string() << '\n';
		for (Piece* piece : piecesOnBoard)
		{
			if (piece->rect->x == newX && piece->rect->y == newY)
			{
				// use the combination of erase and remove to capture piece
				piecesOnBoard.erase(std::remove(piecesOnBoard.begin(), piecesOnBoard.end(), piece),piecesOnBoard.end());
				//std::cout << "Piece removed!\n";
				break;
			}
		}
	}
	//printBoard();
	return true;
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
}