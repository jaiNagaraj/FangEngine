#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <Windows.h>
#include <string>
#include "Window.h"
#include "Game.h"

int main()
{
    Window window;
    window.init();
}