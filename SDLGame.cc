#include "SDLGame.h"

#include <time.h>
#include <iostream>

SDLGame* SDLGame::instance_ = nullptr;

SDLGame::SDLGame(string windowTitle, int width, int height) :
		windowTitle_(windowTitle), width_(width), height_(height) {
	initSDL();
	//initResources();
}

SDLGame::~SDLGame() {
	//closeResources();
	closeSDL();
}

void SDLGame::initSDL() {

	int sdlInit_ret = SDL_Init(SDL_INIT_EVERYTHING);
	assert(sdlInit_ret == 0);

	// Create window
	window_ = SDL_CreateWindow(windowTitle_.c_str(),
	SDL_WINDOWPOS_UNDEFINED,
	SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_SHOWN);
	assert(window_ != nullptr);

	// Create the renderer
	renderer_ = SDL_CreateRenderer(window_, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer_ != nullptr);

	// Clear screen (background color).
	int sdlSetDrawColor_ret = SDL_SetRenderDrawColor(renderer_, 0, 100, 100,
			255);  // Dark grey.
	assert(sdlSetDrawColor_ret != -1);
	int sdlRenderClear_ret = SDL_RenderClear(renderer_);
	assert(sdlRenderClear_ret != -1);
	SDL_RenderPresent(renderer_);

	// hide cursor by default
	//SDL_ShowCursor(0);

}

void SDLGame::closeSDL() {

	SDL_DestroyRenderer(renderer_);
	renderer_ = nullptr;

	SDL_DestroyWindow(window_);
	window_ = nullptr;

	SDL_Quit();
}

// void SDLGame::closeResources() {
// }

