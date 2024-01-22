#pragma once
#include <SDL.h>
#include "Game.h"
class Window
{
	public:
		void init();
		void refresh();
		void dragPiece();
		int dropPiece();
	private:
		SDL_Window* window;
		SDL_Renderer* window_renderer;
		Game game;
		Player p1;
		Player p2;

		// dragged piece info
		Piece* draggedPiece;
		int dragDX, dragDY, initX, initY;

		// a bunch of surfaces
		SDL_Surface* window_surface;
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
};

