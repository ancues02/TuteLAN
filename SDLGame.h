#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <memory>
#include <assert.h>

using namespace std;

class SDLGame {
public:
	virtual ~SDLGame();

	SDLGame(SDLGame&) = delete;
	SDLGame& operator=(SDLGame&) = delete;

	inline static SDLGame* init(string windowTitle, int width, int height) {
		assert(instance_ == nullptr);
		instance_ = new SDLGame(windowTitle, width, height);
		return instance_;
	}

	inline static SDLGame* instance() {
		assert(instance_ != nullptr);
		return instance_;
	}

	inline SDL_Window* getWindow() const {
		return window_;
	}

	inline SDL_Renderer* getRenderer() const {
		return renderer_;
	}

	inline int getWindowWidth() const {
		return width_;
	}

	inline int getWindowHeight() const {
		return height_;
	}

	inline void toggleFullScreen() {
		int flags = SDL_GetWindowFlags(window_);
		if (flags & SDL_WINDOW_FULLSCREEN) {
			SDL_SetWindowFullscreen(window_, 0);
		} else {
			SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);
		}

	}
	inline Uint32 getTime() {
		return SDL_GetTicks();
	}

private:
	SDLGame(string windowTitle_, int width, int height);

	void initSDL(); // initialize SDL
	void closeSDL(); // close SDL
	//void initResources(); // initialize the SDLResources object with the data at the top of this file
	//void closeResources(); // close the SDLResources object (frees all memory)

protected:

	SDL_Window *window_; // the window
	SDL_Renderer *renderer_;  // the renderer

	string windowTitle_; // window title
	int width_; // window width
	int height_; // window height

	static SDLGame* instance_;

};

