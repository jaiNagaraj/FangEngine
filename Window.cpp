#include "Window.h"
#include "Piece.h"
#include <iostream>
#include <string>
#include <fstream>
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


    // Make pawns
    for (int i = 0; i < 8; i++) makePiece(i, 1, WHITE_PAWN);
    for (int i = 0; i < 8; i++) makePiece(i, 6, BLACK_PAWN);
    // Make rest of white
    makePiece(0, 0, WHITE_ROOK, 0);
    makePiece(1, 0, WHITE_KNIGHT);
    makePiece(2, 0, WHITE_BISHOP);
    makePiece(3, 0, WHITE_QUEEN);
    makePiece(4, 0, WHITE_KING);
    makePiece(5, 0, WHITE_BISHOP);
    makePiece(6, 0, WHITE_KNIGHT);
    makePiece(7, 0, WHITE_ROOK, 1);
    // Make rest of black
    makePiece(0, 7, BLACK_ROOK, 0);
    makePiece(1, 7, BLACK_KNIGHT);
    makePiece(2, 7, BLACK_BISHOP);
    makePiece(3, 7, BLACK_QUEEN);
    makePiece(4, 7, BLACK_KING);
    makePiece(5, 7, BLACK_BISHOP);
    makePiece(6, 7, BLACK_KNIGHT);
    makePiece(7, 7, BLACK_ROOK, 1);
    std::string fen = game.getFEN();
    game.fens.push_back(fen);
    std::string pos = fen.substr(0, fen.find(" "));
    if (game.positions.find(pos) == game.positions.end()) game.positions[pos] = 1;
    else game.positions[pos]++;
    std::cout << "FEN: " << fen << "\n";

    SDL_UpdateWindowSurface(window);

    bool keep_window_open = true;
    bool endLock = false;
    int endCode = -1; // Used to make sure we don't use isCheckmate over and over
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        // return -1;
    }
    TTF_Font* font = TTF_OpenFont("Fonts/chess_font.ttf",30);
    while (keep_window_open)
    {
        int xMouse, yMouse;
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0)
        {
            ;
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
                    if (endLock) break;
                    if (e.button.button == SDL_BUTTON_LEFT) dragPiece();
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (endLock) break;
                    if (draggedPiece && e.button.button == SDL_BUTTON_LEFT) endCode = dropPiece();
                    break;
            }
            // check for checkmate!
            if (endCode == 1)
            {
                endLock = true;
                std::string text;
                SDL_Color color = { 0, 255, 0, 255 };

                if (game.turn % 2 == 0) text = "Black wins!";
                else text = "White wins!";
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
            else // check for other types for draws
            {
                // check repetition
                bool rep = false;
                for (auto str : game.positions)
                {
                    if (str.second == 3)
                    {
                        rep = true;
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
                        break;
                    }
                }
                if (!rep)
                {
                    // check for 50 move rule
                    if (game.halfmoves == 100)
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
                    else refresh(); // nothing found
                }
            }
            SDL_UpdateWindowSurface(window);

            //debugging

            /*SDL_GetMouseState(&xMouse, &yMouse); // 479 is max for both coordinates
            std::string outStr((std::string("(") + std::to_string(xMouse) + std::string(", ") + std::to_string(yMouse) + std::string(")\n")));
            std::cout << outStr;

            std::string ltr = std::string(1, (char)(xMouse / 60 + 97));
            std::string num = std::string(1, (char)(56 - yMouse / 60));
            std::string sq = "I'm on " + ltr + num + "!\n";
            std::cout << sq;*/
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

Piece* Window::makePiece(int x, int y, uint8_t info, int side)
{
    // ad to game board
    game.board[7-y][x] = info;
    // set up graphic rect
    SDL_Rect* dstRect = new SDL_Rect;
    dstRect->x = x*60;
    dstRect->y = (7-y)*60;
    dstRect->w = PIECE_WIDTH;
    dstRect->h = PIECE_HEIGHT;
    std::cout << "Piece stats: " << dstRect->x << " " << dstRect->y << " " << dstRect->w << " " << dstRect->h << '\n';
    // make piece, add it to board!
    Piece* tmp = new Piece(dstRect, info, side);
    game.piecesOnBoard.push_back(tmp);
    return tmp;
}

void Window::dragPiece()
{
    int xMouse, yMouse;
    SDL_GetMouseState(&xMouse, &yMouse);
    std::string outStr((std::string("(") + std::to_string(xMouse) + std::string(", ") + std::to_string(yMouse) + std::string(")")));
    std::cout << outStr << "  <-- where you clicked\n";
    for (Piece* piece : game.piecesOnBoard)
    {
        if ((xMouse >= piece->rect->x && xMouse < piece->rect->x + piece->rect->w) && (yMouse >= piece->rect->y && yMouse < piece->rect->y + piece->rect->h))
        {
            draggedPiece = piece;
            dragDX = xMouse - draggedPiece->rect->x;
            dragDY = yMouse - draggedPiece->rect->y;
            initX = draggedPiece->rect->x;
            initY = draggedPiece->rect->y;
            std::cout << piece->rect->x << " " << piece->rect->y << " " << piece->rect->w << " " << piece->rect->h << '\n';
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

    if (game.validMove(draggedPiece, initX, initY, newX, newY))
    {
        // set the x and y coordinates to be multiples of 60 (this makes the piece snap to the grid)
        draggedPiece->rect->x = newX;
        draggedPiece->rect->y = newY;
        game.board[newY / 60][newX / 60] = game.board[initY / 60][initX / 60];
        game.board[initY / 60][initX / 60] = 0;
        game.turn++;
        // check for pawn promotion
        if (game.isPromoting)
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
                    uint8_t infoChange = draggedPiece->info; // set the same for now, changes if we chose an option
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
                    draggedPiece->info = infoChange;

                    if (clicked) // we chose a piece!
                    {
                        // update the game and board to reflect this
                        game.board[newY / 60][newX / 60] = draggedPiece->info;
                        game.isPromoting = false;
                        loop = false;
                    }
                }
            }

        }
        game.printBoard();
        draggedPiece = nullptr; // drop the bass- i mean piece
        // update fen
        std::string fen = game.getFEN();
        game.fens.push_back(fen);
        std::string pos = fen.substr(0, fen.find(" "));
        if (game.positions.find(pos) == game.positions.end()) game.positions[pos] = 1;
        else game.positions[pos]++;
        std::cout << "FEN: " << fen << "\n";
        return game.isCheckmate(game.turn); // check for checkmate!
    }
    else
    {
        draggedPiece->rect->x = initX;
        draggedPiece->rect->y = initY;
        draggedPiece = nullptr; // drop the bass- i mean piece
        return -1;
    }
}
