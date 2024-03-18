#include "FangEngine.h"
#include <algorithm>
#define INF INT_MAX

FangEngine::FangEngine()
{
	playing = true;
}

FangEngine::FangEngine(Game* g)
{
	playing = true;
	game = g;
}

FangEngine::~FangEngine()
{
	;
}

// calls Minimax algorithm
Move* FangEngine::search(int depth)
{
	Move* move = new Move;
	double evaluation = minimax(depth, depth, (game->turn % 2 == 0), -INF, INF, &move);
	std::cout << "Evaluation: " << evaluation << '\n';
	return move;
}

// uses alpha-beta pruning
double FangEngine::minimax(int depth, int trueDepth, bool maxer, double alpha, double beta, Move** move)
{
	std::vector<Move*> moves;
	game->generateLegalMoves(moves);
	std::sort(moves.begin(), moves.end());
	if (depth == 0 || moves.size() == 0)
	{
		for (auto p : moves)
		{
			if (p != *move) delete p;
		}
		int end = game->isCheckmate(game->turn);
		if (end == -1) return eval();
		else if (end == 0 || end == DRAW_BY_FIFTY || end == DRAW_BY_MATERIAL || end == DRAW_BY_REPETITION) return 0;
		else
		{
			if (maxer) return -10000 + (trueDepth - depth); // white is checkmated, return based on mate quickness
			else return 10000 - (trueDepth - depth); // black is checkmated, return based on mate quickness
		}
	}
	if (maxer) // white maximizes
	{
		double maxEval, eval;
		maxEval = -INF; // worst possible eval for white
		for (auto m : moves)
		{
			game->makeMove(m);
			eval = minimax(depth - 1, false, false, alpha, beta, move);
			if (eval > maxEval)
			{
				maxEval = eval;
				if (depth == trueDepth) *move = m;
			}
			alpha = std::max(alpha, eval); // change lower bound if necessary
			game->unmakeMove(m);
			if (beta <= alpha) break; // black will never go down this branch; has better option
		}
		for (auto p : moves)
		{
			if (p != *move) delete p;
		}
		return maxEval;
	}
	else // black minimizes
	{
		double minEval, eval;
		minEval = INF; // worst possible eval for black
		for (auto m : moves)
		{
			game->makeMove(m);
			eval = minimax(depth - 1, false, true, alpha, beta, move);
			if (eval < minEval)
			{
				minEval = eval;
				if (depth == trueDepth) *move = m;
			}
			beta = std::min(beta, eval); // change upper bound if necessary
			game->unmakeMove(m);
			if (beta <= alpha) break; // black will never go down this branch; has better option
		}
		for (auto p : moves)
		{
			if (p != *move) delete p;
		}
		return minEval;
	}
}

double FangEngine::eval()
{
	int whiteKingX, whiteKingY, blackKingX, blackKingY;
	bool whiteHasQueen = false, blackHasQueen = false;
	int whiteMinors = 0, blackMinors = 0;
	double eval = 0; // evaluation in centipawns
	uint64_t tmp;
	
	#ifdef USING_BITS

	// loop over white pawns
	tmp = game->pieceBoards[WP_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += PAWN_VALUE + whitePawnPlacement[7 - (index / 8)][index % 8];
		}
		else std::cout << "Something wrong with white pawns in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over white knights
	tmp = game->pieceBoards[WN_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += KNIGHT_VALUE + whiteKnightPlacement[7 - (index / 8)][index % 8];
			whiteMinors += KNIGHT_VALUE;
		}
		else std::cout << "Something wrong with white knights in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over white bishop
	tmp = game->pieceBoards[WB_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += BISHOP_VALUE + whiteBishopPlacement[7 - (index / 8)][index % 8];
			whiteMinors += BISHOP_VALUE;
		}
		else std::cout << "Something wrong with white bishops in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over white rooks
	tmp = game->pieceBoards[WR_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += ROOK_VALUE + whiteRookPlacement[7 - (index / 8)][index % 8];
		}
		else std::cout << "Something wrong with white rooks in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over white queens
	tmp = game->pieceBoards[WQ_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += QUEEN_VALUE + whiteQueenPlacement[7 - (index / 8)][index % 8];
			whiteHasQueen = true;
		}
		else std::cout << "Something wrong with white queens in eval()!\n";

		tmp &= tmp - 1;
	}

	// get white king index
	tmp = game->pieceBoards[WK_INDEX];
	if (!tmp) std::cout << "CANNOT FIND WHITE KING IN eval()!\n";
	int whiteKingIndex = game->bitToIndex(tmp);
	whiteKingX = whiteKingIndex & 8;
	whiteKingY = 7 - (whiteKingIndex / 8);


	// loop over black pawns
	tmp = game->pieceBoards[BP_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += -PAWN_VALUE + blackPawnPlacement[7 - (index / 8)][index % 8];
		}
		else std::cout << "Something wrong with black pawns in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over black knights
	tmp = game->pieceBoards[BN_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += -KNIGHT_VALUE + blackKnightPlacement[7 - (index / 8)][index % 8];
			blackMinors += KNIGHT_VALUE;
		}
		else std::cout << "Something wrong with black knights in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over black bishop
	tmp = game->pieceBoards[BB_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += -BISHOP_VALUE + blackBishopPlacement[7 - (index / 8)][index % 8];
			blackMinors += BISHOP_VALUE;
		}
		else std::cout << "Something wrong with black bishops in eval()!\n";

		tmp &= tmp - 1;
	}

	// loop over black rooks
	tmp = game->pieceBoards[BR_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += -ROOK_VALUE + blackRookPlacement[7 - (index / 8)][index % 8];
		}
		else std::cout << "Something wrong with black rooks in eval()!\n";

		tmp &= tmp - 1;
}

	// loop over black queens
	tmp = game->pieceBoards[BQ_INDEX];
	while (tmp)
	{
		unsigned long index;
		char ch = _BitScanForward64(&index, tmp);
		if (ch)
		{
			eval += -QUEEN_VALUE + blackQueenPlacement[7 - (index / 8)][index % 8];
			blackHasQueen = true;
		}
		else std::cout << "Something wrong with black queens in eval()!\n";

		tmp &= tmp - 1;
	}

	// get black king index
	tmp = game->pieceBoards[BK_INDEX];
	if (!tmp) std::cout << "CANNOT FIND BLACK KING IN eval()!\n";
	int blackKingIndex = game->bitToIndex(tmp);
	blackKingX = blackKingIndex & 8;
	blackKingY = 7 - (blackKingIndex / 8);


	#else
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			switch (game->board[y][x])
			{
				case WHITE_PAWN:
					eval += PAWN_VALUE + whitePawnPlacement[y][x];
					break;
				case BLACK_PAWN:
					eval += -PAWN_VALUE + blackPawnPlacement[y][x];
					break;
				case WHITE_KNIGHT:
					eval += KNIGHT_VALUE + whiteKnightPlacement[y][x];
					whiteMinors += KNIGHT_VALUE;
					break;
				case BLACK_KNIGHT:
					eval += -KNIGHT_VALUE + blackKnightPlacement[y][x];
					blackMinors += KNIGHT_VALUE;
					break;
				case WHITE_BISHOP:
					eval += BISHOP_VALUE + whiteBishopPlacement[y][x];
					whiteMinors += BISHOP_VALUE;
					break;
				case BLACK_BISHOP:
					eval += -BISHOP_VALUE + blackBishopPlacement[y][x];
					blackMinors += BISHOP_VALUE;
					break;
				case WHITE_ROOK:
					eval += ROOK_VALUE + whiteRookPlacement[y][x];
					break;
				case BLACK_ROOK:
					eval += -ROOK_VALUE + blackRookPlacement[y][x];
					break;
				case WHITE_QUEEN:
					whiteHasQueen = true;
					eval += QUEEN_VALUE + whiteQueenPlacement[y][x];
					break;
				case BLACK_QUEEN:
					blackHasQueen = true;
					eval += -QUEEN_VALUE + blackQueenPlacement[y][x];
					break;
				case WHITE_KING:
					whiteKingX = x; whiteKingY = y;
					break;
				case BLACK_KING:
					blackKingX = x; blackKingY = y;
					break;
			}
		}
	}

	#endif

	// check for endgame
	if (!(whiteHasQueen || blackHasQueen) || 
		(whiteHasQueen && !blackHasQueen && whiteMinors < 400) || 
		(blackHasQueen && !whiteHasQueen && blackMinors < 400) ||
		(whiteHasQueen && blackHasQueen && whiteMinors < 400 && blackMinors < 400))
	{
		eval += KING_VALUE + whiteKingPlacementE[whiteKingY][whiteKingX];
		eval += -KING_VALUE + blackKingPlacementE[blackKingY][blackKingX];
	}
	// kings are in middlegame
	else
	{
		eval += KING_VALUE + whiteKingPlacementM[whiteKingY][whiteKingX];
		eval += -KING_VALUE + blackKingPlacementM[blackKingY][blackKingX];
	}

	return eval / 100.0; // return eval in pawns
}