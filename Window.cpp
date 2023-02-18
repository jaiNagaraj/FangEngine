#include "Window.h"
#include <iostream>
#include <string>
#include <fstream>
#include <SDL.h>
#include <Windows.h>

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
        480, 480,
        0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        //return -1;
    }

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        //return -1;
    }

    SDL_Surface* boardImage = SDL_LoadBMP("board.bmp");
    SDL_Surface* whitePawn = SDL_LoadBMP("PieceImages/Chess_plt60.bmp");
    SDL_Surface* whiteKnight = SDL_LoadBMP("PieceImages/Chess_nlt60.bmp");
    SDL_Surface* whiteBishop = SDL_LoadBMP("PieceImages/Chess_blt60.bmp");
    SDL_Surface* whiteRook = SDL_LoadBMP("PieceImages/Chess_rlt60.bmp");
    SDL_Surface* whiteQueen = SDL_LoadBMP("PieceImages/Chess_qlt60.bmp");
    SDL_Surface* whiteKing = SDL_LoadBMP("PieceImages/Chess_klt60.bmp");
    SDL_Surface* blackPawn = SDL_LoadBMP("PieceImages/Chess_pdt60.bmp");
    SDL_Surface* blackKnight = SDL_LoadBMP("PieceImages/Chess_ndt60.bmp");
    SDL_Surface* blackBishop = SDL_LoadBMP("PieceImages/Chess_bdt60.bmp");
    SDL_Surface* blackRook = SDL_LoadBMP("PieceImages/Chess_rdt60.bmp");
    SDL_Surface* blackQueen = SDL_LoadBMP("PieceImages/Chess_qdt60.bmp");
    SDL_Surface* blackKing = SDL_LoadBMP("PieceImages/Chess_kdt60.bmp");

    if (!boardImage)
    {
        std::cout << "Failed to load image\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        //return -1;
    }

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
                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "da mouse is downnnnn\n";
                    break;
                case SDL_MOUSEBUTTONUP:
                    std::cout << "da mouse is back up fool\n";
                    break;
            }
            SDL_GetMouseState(&xMouse, &yMouse); // 479 is max for both coordinates

            //debugging

            std::string outStr((std::string("(") + std::to_string(xMouse) + std::string(", ") + std::to_string(yMouse) + std::string(")\n")));
            std::cout << outStr;

            std::string ltr = std::string(1, (char)(xMouse / 60 + 97));
            std::string num = std::string(1, (char)(56 - yMouse / 60));
            std::string sq = "I'm on " + ltr + num + "!\n";
            std::cout << sq;

            SDL_BlitSurface(boardImage, NULL, window_surface, NULL);
            // Place pawns
            for (int i = 0; i < 8; i++) placePiece(&whitePawn, &window_surface, i, 1);
            for (int i = 0; i < 8; i++) placePiece(&blackPawn, &window_surface, i, 6);
            // Place rest of white
            placePiece(&whiteRook, &window_surface, 0, 0);
            placePiece(&whiteKnight, &window_surface, 1, 0);
            placePiece(&whiteBishop, &window_surface, 2, 0);
            placePiece(&whiteQueen, &window_surface, 3, 0);
            placePiece(&whiteKing, &window_surface, 4, 0);
            placePiece(&whiteBishop, &window_surface, 5, 0);
            placePiece(&whiteKnight, &window_surface, 6, 0);
            placePiece(&whiteRook, &window_surface, 7, 0);
            // Place rest of black
            placePiece(&blackRook, &window_surface, 0, 7);
            placePiece(&blackKnight, &window_surface, 1, 7);
            placePiece(&blackBishop, &window_surface, 2, 7);
            placePiece(&blackQueen, &window_surface, 3, 7);
            placePiece(&blackKing, &window_surface, 4, 7);
            placePiece(&blackBishop, &window_surface, 5, 7);
            placePiece(&blackKnight, &window_surface, 6, 7);
            placePiece(&blackRook, &window_surface, 7, 7);

            SDL_UpdateWindowSurface(window);
        }
    }
}

void Window::placePiece(SDL_Surface** piece, SDL_Surface** source, int x, int y)
{
    SDL_Rect dstRect;
    dstRect.x = x*60;
    dstRect.y = (7-y)*60;
    dstRect.w = (*piece)->w;
    dstRect.h = (*piece)->h;
    SDL_BlitSurface(*piece, NULL, *source, &dstRect);
}
