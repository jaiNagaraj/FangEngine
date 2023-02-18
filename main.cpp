#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "Window.h"
#include "Game.h"

int main()
{
    /*
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        480, 480,
        0);

    if (!window)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    SDL_Surface* window_surface = SDL_GetWindowSurface(window);

    if (!window_surface)
    {
        std::cout << "Failed to get the surface from the window\n";
        return -1;
    }

    SDL_Surface* image = SDL_LoadBMP("board.bmp");

    if (!image)
    {
        std::cout << "Failed to load image\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        return -1;
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
            }
            SDL_GetMouseState(&xMouse, &yMouse); // 479 is max for both coordinates

            //debugging
            std::string outStr((std::string("(") + std::to_string(xMouse) + std::string(", ") + std::to_string(yMouse) + std::string(")\n")));
            std::wstring tmp = std::wstring(outStr.begin(), outStr.end());
            LPCWSTR wideString = tmp.c_str();
            OutputDebugString(wideString);
            std::string ltr = std::string(1, (char)(xMouse / 60 + 97));
            std::string num = std::string(1, (char)(56 - yMouse / 60));
            std::string sq = "I'm on " + ltr + num + "!\n";
            std::wstring tmp2 = std::wstring(sq.begin(), sq.end());
            LPCWSTR wideString2 = tmp2.c_str();
            OutputDebugString(wideString2);

            SDL_BlitSurface(image, NULL, window_surface, NULL);
            SDL_UpdateWindowSurface(window);
        }
    }
    */
    Window window;
    window.init();
}