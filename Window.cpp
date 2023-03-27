#include "Window.h"
#include "Piece.h"
#include <iostream>
#include <string>
#include <fstream>
#include <SDL.h>
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

    //if (!boardImage)
    //{
    //    std::cout << "Failed to load image\n";
    //    std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
    //    //return -1;
    //}


    // Make pawns
    for (int i = 0; i < 8; i++) makePiece(i, 1, WHITE_PAWN);
    for (int i = 0; i < 8; i++) makePiece(i, 6, BLACK_PAWN);
    // Make rest of white
    makePiece(0, 0, WHITE_ROOK);
    makePiece(1, 0, WHITE_KNIGHT);
    makePiece(2, 0, WHITE_BISHOP);
    makePiece(3, 0, WHITE_QUEEN);
    makePiece(4, 0, WHITE_KING);
    makePiece(5, 0, WHITE_BISHOP);
    makePiece(6, 0, WHITE_KNIGHT);
    makePiece(7, 0, WHITE_ROOK);
    // Make rest of black
    makePiece(0, 7, BLACK_ROOK);
    makePiece(1, 7, BLACK_KNIGHT);
    makePiece(2, 7, BLACK_BISHOP);
    makePiece(3, 7, BLACK_QUEEN);
    makePiece(4, 7, BLACK_KING);
    makePiece(5, 7, BLACK_BISHOP);
    makePiece(6, 7, BLACK_KNIGHT);
    makePiece(7, 7, BLACK_ROOK);

    SDL_UpdateWindowSurface(window);

    //draggedPiece = new Piece();

    bool keep_window_open = true;
    while (keep_window_open)
    {
        int xMouse, yMouse;
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0)
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
                case SDL_MOUSEMOTION:
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
                    std::cout << "da mouse is downnnnn\n";
                    dragPiece();
                    break;
                case SDL_MOUSEBUTTONUP:
                    std::cout << "da mouse is back up fool\n";
                    if (draggedPiece) dropPiece();
                    break;
            }
            refresh();
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
}

Piece* Window::makePiece(int x, int y, uint8_t info)
{
    game.board[7-y][x] = info;
    SDL_Rect* dstRect = new SDL_Rect;
    dstRect->x = x*60;
    dstRect->y = (7-y)*60;
    dstRect->w = PIECE_WIDTH;
    dstRect->h = PIECE_HEIGHT;
    std::cout << "Piece stats: " << dstRect->x << " " << dstRect->y << " " << dstRect->w << " " << dstRect->h << '\n';
    //SDL_BlitSurface(*piece, NULL, *source, dstRect);
    Piece* tmp = new Piece(dstRect, info);
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

void Window::dropPiece()
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
        draggedPiece->rect->x = x / 60 * 60;
        draggedPiece->rect->y = y / 60 * 60;
        game.board[newY / 60][newX / 60] = game.board[initY / 60][initX / 60];
        game.board[initY / 60][initX / 60] = 0;
        game.turn++;
    }
    else
    {
        draggedPiece->rect->x = initX;
        draggedPiece->rect->y = initY;
    }
    // drop the bass- i mean piece
    draggedPiece = nullptr;
}
