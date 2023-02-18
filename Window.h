#pragma once
#include <SDL.h>
class Window
{
	public:
		void init();
		void placePiece(SDL_Surface** piece, SDL_Surface** source, int x, int y);
	private:
		SDL_Window* window;
};

