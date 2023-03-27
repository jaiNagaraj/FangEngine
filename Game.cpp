#include "Game.h"
#include "Piece.h"
#include <bitset>
#include <string>

bool Game::validMove(Piece* piece, int oldX, int oldY, int newX, int newY)
{
	int oldXCoord = oldX / 60, oldYCoord = oldY / 60, newXCoord = newX / 60, newYCoord = newY / 60;
	std::cout << oldX << " " << oldY << " " << newX << " " << newY << '\n';
	std::cout << oldXCoord << " " << oldYCoord << " " << newXCoord << " " << newYCoord << '\n';
	std::cout << "TURN: " << turn << '\n';
	std::cout << "Piece on new square: " << (board[newYCoord][newXCoord] & BLACK) << '\n';

	bool isCapturing = false;

	// if it's white's turn but black played
	if (turn % 2 == 0 && !(piece->info & WHITE)) return false;
	// if it's black's turn but white played
	if (turn % 2 != 0 && !(piece->info & BLACK)) return false;

	if ((piece->info & WHITE_PAWN) == WHITE_PAWN)
	{
		int distMovedX = newXCoord - oldXCoord;
		int distMovedY = newYCoord - oldYCoord;
		std::cout << distMovedX << " " << distMovedY << '\n';

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
			if (!(board[newYCoord][newXCoord] == 0b00000000) || (distMovedY == -2 && !(board[newYCoord + 1][newXCoord] == 0b00000000))) return false;
		}

		else if (!(distMovedY == -1) || !(board[newYCoord][newXCoord] == 0b00000000)) return false;
	}
	else if ((piece->info & BLACK_PAWN) == BLACK_PAWN)
	{
		int distMovedX = newXCoord - oldXCoord;
		int distMovedY = newYCoord - oldYCoord;
		std::cout << distMovedX << " " << distMovedY << '\n';

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
			if (!(board[newYCoord][newXCoord] == 0b00000000) || (distMovedY == 2 && !(board[newYCoord - 1][newXCoord] == 0b00000000))) return false;
		}

		else if (!(distMovedY == 1) || !(board[newYCoord][newXCoord] == 0b00000000)) return false;
	}





	if (isCapturing)
	{
		// remove piece from board vector
		uint8_t capturedPiece = board[newYCoord][newXCoord];
		std::cout << std::bitset< 8 >(capturedPiece).to_string() << '\n';
		for (Piece* piece : piecesOnBoard)
		{
			if (piece->rect->x == newX && piece->rect->y == newY)
			{
				std::remove(piecesOnBoard.begin(), piecesOnBoard.end(), piece);
				std::cout << "Piece removed!\n";
				break;
			}
		}
	}
	printBoard();
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