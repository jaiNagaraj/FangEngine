#include "Window.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <SDL.h>
#include <SDL_ttf.h>
#include <Windows.h>
#define PIECE_WIDTH 60
#define PIECE_HEIGHT 60
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

void Window::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        //return -1;
    }

    window = SDL_CreateWindow("Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        //return -1;
    }

    window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        //return -1;
    }

    // build game state
    std::string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::string buildFEN = "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1";
    std::string possibleErrFEN = "r7/B1p1kpp1/7p/4p3/4p3/2P5/5PPP/1R4K1 b - - 0 32";
    std::string possibleErrFEN2 = "rnbqk2r/pppp1ppp/8/7n/1b1NPp2/2NP4/PPP3PP/R1BQKB1R b KQkq - 2 6";
    game.buildFromFEN(startingFEN);

    // test legal moves
    //std::vector<Move*> m;
    //std::cout << game.generateLegalMoves(m);

    // Print game to start
    game.printBoard();

    refresh();
    SDL_UpdateWindowSurface(window);

    // initialize players
    p1 = FangEngine(&game);
    p2 = FangEngine(&game);

    /* PERFORMANCE TESTING */
    //for (int depth = 3; depth <= 3; depth++)
    //{
    //    auto t1 = std::chrono::high_resolution_clock::now();
    //    std::cout << "Number of possible positions at depth = " << depth << ": " << perft(depth) << '\n';
    //    auto t2 = std::chrono::high_resolution_clock::now();
    //    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    //    std::cout << "This took " << ms_int.count() << "ms\n";
    //}
    // Debugging: Check position count
    //for (auto i : game.positions)
    //{
    //    std::cout << "For position " << i.first << ": " << i.second << '\n';
    //}
    // Debugging: Check fen count
    //std::cout << "Number of fens: " << game.fens.size() << '\n';
    /* PERFORMANCE TESTING: CAPTURES */
    //for (int depth = 1; depth <= 3; depth++)
    //{
    //    std::cout << "Number of possible captures at depth = " << depth << ": " << game.perftCaps(depth, false) << '\n';
    //}

    // print out starting moves
    game.generateLegalMoves(game.legalMoveList);
    //for (auto m : game.legalMoveList) m->printMove();

    bool keep_window_open = true;
    endLock = false;
    endCode = -1; // Used to make sure we don't use isCheckmate over and over
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        // return -1;
    }
    font = TTF_OpenFont("Fonts/chess_font.ttf",30);
    while (keep_window_open)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0)
        {
            if (endLock && !(e.type == SDL_QUIT)) continue;
            switch (e.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
                case SDL_MOUSEMOTION:
                    if (endLock) continue;
                    if (draggedPiece)
                    {
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if (x < WINDOW_WIDTH && x > 0 && y < WINDOW_HEIGHT && y > 0)
                        {
                            draggedPiece->rect->x = x - dragDX;
                            draggedPiece->rect->y = y - dragDY;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (endLock) continue;
                    if (e.button.button == SDL_BUTTON_LEFT) dragPiece();
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (endLock) continue;
                    if (draggedPiece && e.button.button == SDL_BUTTON_LEFT) endCode = dropPiece();
                    break;
            }
            
            endCodeCheck();
            SDL_UpdateWindowSurface(window);

            // 1 second delay before computer move
            //std::this_thread::sleep_for(std::chrono::nanoseconds(10));
            //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(50));

            // 0 for white, 1 for black, -1 to disable
            int computerTurn1 = 1, computerTurn2 = -1;

            // make computer move
            if (game.turn % 2 == computerTurn1)
            {
                Move* m = p1.search(4);
                if (m && m->piece)
                {
                    game.makeMove(m);
                    endCode = game.isCheckmate(game.turn);
                    endCodeCheck();
                    SDL_UpdateWindowSurface(window);
                    // print out next person's moves
                    for (auto m : game.legalMoveList) delete m;
                    game.legalMoveList.clear();
                    game.generateLegalMoves(game.legalMoveList);
            
                    //// finaly, update fen and position data
                    //std::string fen = game.getFEN();
                    //game.fens.push_back(fen);
                    //std::string pos = fen.substr(0, fen.find(" "));
                    //if (game.positions.count(pos) == 0) game.positions[pos] = 1;
                    //else game.positions[pos]++;
                    //std::cout << "FEN: " << fen << "\n";
                    //std::cout << "FEN: " << move->fen << "\n";
                }
                if (m) delete m;
            }
            if (game.turn % 2 == computerTurn2)
            {
                Move* m = p2.search(4);
                if (m && m->piece)
                {
                    game.makeMove(m);
                    endCode = game.isCheckmate(game.turn);
                    endCodeCheck();
                    SDL_UpdateWindowSurface(window);
                    // print out next person's moves
                    for (auto m : game.legalMoveList) delete m;
                    game.legalMoveList.clear();
                    game.generateLegalMoves(game.legalMoveList);

                    //// finaly, update fen and position data
                    //std::string fen = game.getFEN();
                    //game.fens.push_back(fen);
                    //std::string pos = fen.substr(0, fen.find(" "));
                    //if (game.positions.count(pos) == 0) game.positions[pos] = 1;
                    //else game.positions[pos]++;
                    //std::cout << "FEN: " << fen << "\n";
                    //std::cout << "FEN: " << move->fen << "\n";
                }
                if (m) delete m;
            }
        }
    }
}

// blit the pieces onto the board
void Window::refresh()
{
    SDL_BlitSurface(boardImage, NULL, window_surface, NULL);
    for (Piece* piece : game.piecesOnBoard)
    {
        if (piece == draggedPiece) continue;
        switch (piece->info)
        {
            case WHITE_PAWN:
                SDL_BlitSurface(whitePawn, NULL, window_surface, piece->rect);
                break;
            case WHITE_KNIGHT:
                SDL_BlitSurface(whiteKnight, NULL, window_surface, piece->rect);
                break;
            case WHITE_BISHOP:
                SDL_BlitSurface(whiteBishop, NULL, window_surface, piece->rect);
                break;
            case WHITE_ROOK:
                SDL_BlitSurface(whiteRook, NULL, window_surface, piece->rect);
                break;
            case WHITE_QUEEN:
                SDL_BlitSurface(whiteQueen, NULL, window_surface, piece->rect);
                break;
            case WHITE_KING:
                SDL_BlitSurface(whiteKing, NULL, window_surface, piece->rect);
                break;
            case BLACK_PAWN:
                SDL_BlitSurface(blackPawn, NULL, window_surface, piece->rect);
                break;
            case BLACK_KNIGHT:
                SDL_BlitSurface(blackKnight, NULL, window_surface, piece->rect);
                break;
            case BLACK_BISHOP:
                SDL_BlitSurface(blackBishop, NULL, window_surface, piece->rect);
                break;
            case BLACK_ROOK:
                SDL_BlitSurface(blackRook, NULL, window_surface, piece->rect);
                break;
            case BLACK_QUEEN:
                SDL_BlitSurface(blackQueen, NULL, window_surface, piece->rect);
                break;
            case BLACK_KING:
                SDL_BlitSurface(blackKing, NULL, window_surface, piece->rect);
                break;

        }
    }
    // blit the dragged piece last so it won't go under anything
    if (draggedPiece)
    {
        switch (draggedPiece->info)
        {
            case WHITE_PAWN:
                SDL_BlitSurface(whitePawn, NULL, window_surface, draggedPiece->rect);
                break;
            case WHITE_KNIGHT:
                SDL_BlitSurface(whiteKnight, NULL, window_surface, draggedPiece->rect);
                break;
            case WHITE_BISHOP:
                SDL_BlitSurface(whiteBishop, NULL, window_surface, draggedPiece->rect);
                break;
            case WHITE_ROOK:
                SDL_BlitSurface(whiteRook, NULL, window_surface, draggedPiece->rect);
                break;
            case WHITE_QUEEN:
                SDL_BlitSurface(whiteQueen, NULL, window_surface, draggedPiece->rect);
                break;
            case WHITE_KING:
                SDL_BlitSurface(whiteKing, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_PAWN:
                SDL_BlitSurface(blackPawn, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_KNIGHT:
                SDL_BlitSurface(blackKnight, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_BISHOP:
                SDL_BlitSurface(blackBishop, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_ROOK:
                SDL_BlitSurface(blackRook, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_QUEEN:
                SDL_BlitSurface(blackQueen, NULL, window_surface, draggedPiece->rect);
                break;
            case BLACK_KING:
                SDL_BlitSurface(blackKing, NULL, window_surface, draggedPiece->rect);
                break;
        }
    }
}

void Window::dragPiece()
{
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    std::string outStr((std::string("(") + std::to_string(xMouse) + std::string(", ") + std::to_string(yMouse) + std::string(")")));
    //std::cout << outStr << "  <-- where you clicked\n";
    for (Piece* piece : game.piecesOnBoard)
    {
        if ((xMouse >= piece->rect->x && xMouse < piece->rect->x + piece->rect->w) && (yMouse >= piece->rect->y && yMouse < piece->rect->y + piece->rect->h))
        {
            draggedPiece = piece;
            dragDX = xMouse - draggedPiece->rect->x;
            dragDY = yMouse - draggedPiece->rect->y;
            initX = draggedPiece->rect->x;
            initY = draggedPiece->rect->y;
            //std::cout << piece->rect->x << " " << piece->rect->y << " " << piece->rect->w << " " << piece->rect->h << '\n';
        }
    }
}

int Window::dropPiece()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (x >= 480) x = 479;
    if (x < 0) x = 0;
    if (y >= 480) y = 479;
    if (y < 0) y = 0;
    
    int newX = x / 60 * 60, newY = y / 60 * 60;
    Move* moveToMake = game.validMove(draggedPiece, initX, initY, newX, newY);
    if (moveToMake)
    {
        std::cout << "VALID MOVE!!!\n";
        // set the x and y coordinates to be multiples of 60 (this makes the piece snap to the grid)
        draggedPiece->rect->x = newX;
        draggedPiece->rect->y = newY;

        if (!(moveToMake->piece)) moveToMake->piece = draggedPiece;
         

        // check for pawn promotion
        if (moveToMake->isPromoting)
        {
            // form graphic for promotion!
            // first, a white background
            SDL_Rect* bgRect = new SDL_Rect;
            bgRect->x = 120;
            bgRect->y = 210;
            bgRect->w = PIECE_WIDTH * 4;
            bgRect->h = PIECE_HEIGHT;

            // then, the rects (screen areas) for the piece graphics
            SDL_Rect* nRect = new SDL_Rect; // knight
            nRect->x = 120 + 60*0;
            nRect->y = 210;
            nRect->w = PIECE_WIDTH;
            nRect->h = PIECE_HEIGHT;
            SDL_Rect* bRect = new SDL_Rect; // bishop
            bRect->x = 120 + 60 * 1;
            bRect->y = 210;
            bRect->w = PIECE_WIDTH;
            bRect->h = PIECE_HEIGHT;
            SDL_Rect* rRect = new SDL_Rect; // rook
            rRect->x = 120 + 60 * 2;
            rRect->y = 210;
            rRect->w = PIECE_WIDTH;
            rRect->h = PIECE_HEIGHT;
            SDL_Rect* qRect = new SDL_Rect; // queen
            qRect->x = 120 + 60 * 3;
            qRect->y = 210;
            qRect->w = PIECE_WIDTH;
            qRect->h = PIECE_HEIGHT;

            //SDL_RenderClear(window_renderer);
            //SDL_SetRenderDrawColor(window_renderer, 255, 255, 255, 255);
            //SDL_RenderFillRect(window_renderer, bgRect);
            //SDL_RenderPresent(window_renderer);

            // next, blit the graphic
            if ((draggedPiece->info & WHITE) == WHITE)
            {
                SDL_BlitSurface(whiteKnight, NULL, window_surface, nRect);
                SDL_BlitSurface(whiteBishop, NULL, window_surface, bRect);
                SDL_BlitSurface(whiteRook, NULL, window_surface, rRect);
                SDL_BlitSurface(whiteQueen, NULL, window_surface, qRect);
            }
            else
            {
                SDL_BlitSurface(blackKnight, NULL, window_surface, nRect);
                SDL_BlitSurface(blackBishop, NULL, window_surface, bRect);
                SDL_BlitSurface(blackRook, NULL, window_surface, rRect);
                SDL_BlitSurface(blackQueen, NULL, window_surface, qRect);
            }
            SDL_UpdateWindowSurface(window);

            // finally, loop until a choice is made
            bool loop = true;
            while (loop)
            {
                SDL_Event event;
                // check if we clicked
                if (SDL_PollEvent(&event) > 0 && event.type == SDL_MOUSEBUTTONUP)
                {
                    uint8_t infoChange; // changes if we chose an option
                    SDL_Point clickPoint; // mouse location
                    SDL_GetMouseState(&clickPoint.x, &clickPoint.y); // get mouse location
                    bool clicked = true;
                    if ((draggedPiece->info & WHITE) == WHITE)
                    {
                        // check what piece we clicked on, if any
                        if (SDL_PointInRect(&clickPoint, nRect)) infoChange = WHITE_KNIGHT;
                        else if (SDL_PointInRect(&clickPoint, bRect)) infoChange = WHITE_BISHOP;
                        else if (SDL_PointInRect(&clickPoint, rRect)) infoChange = WHITE_ROOK;
                        else if (SDL_PointInRect(&clickPoint, qRect)) infoChange = WHITE_QUEEN;
                        else clicked = false; // nothing selected
                    }
                    else
                    {
                        // check what piece we clicked on, if any
                        if (SDL_PointInRect(&clickPoint, nRect)) infoChange = BLACK_KNIGHT;
                        else if (SDL_PointInRect(&clickPoint, bRect)) infoChange = BLACK_BISHOP;
                        else if (SDL_PointInRect(&clickPoint, rRect)) infoChange = BLACK_ROOK;
                        else if (SDL_PointInRect(&clickPoint, qRect)) infoChange = BLACK_QUEEN;
                        else clicked = false; // nothing selected
                    }

                    if (clicked) // we chose a piece!
                    {
                        moveToMake->promoPiece = infoChange;
                        // update the game and board to reflect this
                        //game.board[newY / 60][newX / 60] = draggedPiece->info;
                        loop = false;
                    }
                }
            }

        }

        // execute the move
        game.makeMove(moveToMake);
        game.printBoard();

        draggedPiece = nullptr; // drop the bass- i mean piece

        // print out next person's moves
        for (auto m : game.legalMoveList) delete m;
        game.legalMoveList.clear();
        game.generateLegalMoves(game.legalMoveList);
        //for (auto m : game.legalMoveList) m->printMove();

        //// finaly, update fen and position data
        //std::string fen = game.getFEN();
        //game.fens.push_back(fen);
        //std::string pos = fen.substr(0, fen.find(" "));
        //if (game.positions.count(pos) == 0) game.positions[pos] = 1;
        //else game.positions[pos]++;

        // print out game FEN
        std::cout << "FEN: " << game.getFEN() << "\n";
        return game.isCheckmate(game.turn); // check for checkmate!
    }
    else
    {
        std::cout << "\nINVALID MOVE!!!\n";
        std::cout << "Valid moves are:\n";
        for (auto m : game.legalMoveList) m->printMove();
        std::cout << "Try again!\n\n";
        draggedPiece->rect->x = initX;
        draggedPiece->rect->y = initY;
        draggedPiece = nullptr; // drop the bass- i mean piece
        return -1;
    }
}

void Window::endCodeCheck()
{
    // check for checkmate!
    if (endCode == 1)
    {
        endLock = true;
        std::string text;
        SDL_Color color = { 0, 255, 0, 255 };

        if (game.turn % 2 == 0) text = "Black wins!";
        else text = "White wins!";
        std::cout << text << '\n';
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Rect* textRect = new SDL_Rect;
        textRect->x = 60;
        textRect->y = 60;
        textRect->w = 60;
        textRect->h = 60;
        refresh();
        SDL_BlitSurface(textSurface, NULL, window_surface, textRect);

    }
    else if (endCode == 0)
    {
        endLock = true;
        std::string text = "Stalemate!";
        SDL_Color color = { 255, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Rect* textRect = new SDL_Rect;
        textRect->x = 60;
        textRect->y = 60;
        textRect->w = 60;
        textRect->h = 60;
        refresh();
        SDL_BlitSurface(textSurface, NULL, window_surface, textRect);
    }
    else if (endCode == DRAW_BY_REPETITION)
    {
        endLock = true;
        std::string text = "Draw by repetition!";
        SDL_Color color = { 255, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Rect* textRect = new SDL_Rect;
        textRect->x = 60;
        textRect->y = 60;
        textRect->w = 60;
        textRect->h = 60;
        refresh();
        SDL_BlitSurface(textSurface, NULL, window_surface, textRect);
    }
    else if (endCode == DRAW_BY_FIFTY)
    {
        endLock = true;
        std::string text = "Draw by 50 move rule!";
        SDL_Color color = { 255, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Rect* textRect = new SDL_Rect;
        textRect->x = 60;
        textRect->y = 60;
        textRect->w = 60;
        textRect->h = 60;
        refresh();
        SDL_BlitSurface(textSurface, NULL, window_surface, textRect);
    }
    else if (endCode == DRAW_BY_MATERIAL)
    {
        endLock = true;
        std::string text = "Draw by insufficient material!";
        SDL_Color color = { 255, 0, 0, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Rect* textRect = new SDL_Rect;
        textRect->x = 60;
        textRect->y = 60;
        textRect->w = 60;
        textRect->h = 60;
        refresh();
        SDL_BlitSurface(textSurface, NULL, window_surface, textRect);
    }
    else // nothing found, make computer move
    {
        refresh(); // nothing found
    }
}

ull Window::perft(int depth /* assuming >= 1 */)
{
    std::vector<Move*> move_list;
    ull n_moves, i;
    ull nodes = 0;
    //std::string startingFen = getFEN();
    std::string currFen;

    if (depth == 0)
    {
        //for (auto p : move_list) delete p;
        //return (ull) n_moves;
        return 1;
    }

    n_moves = game.generateLegalMoves(move_list);


    for (i = 0; i < n_moves; i++)
    {
        //currFen = getFEN();
        //if (startingFen != currFen)
        //{
        //	std::cout << "Move was not undone correctly!\n";
        //	std::cout << "Good FEN: " << startingFen << '\n';
        //	std::cout << "Bad (current) FEN: " << currFen << '\n';
        //}
        //move_list[i]->printMove();
        game.makeMove(move_list[i]);
        refresh();
        SDL_UpdateWindowSurface(window);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(50));
        nodes += perft(depth - 1);
        game.unmakeMove(move_list[i]);
        //std::cout << "\nhelo\n";
        refresh();
        SDL_UpdateWindowSurface(window);
    }
    for (auto p : move_list) delete p;
    return nodes;
}