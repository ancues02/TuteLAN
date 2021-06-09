#include "TuteLAN.h"

#include <assert.h>
#include "InputHandler.h"
#include "SDL_macros.h"

using namespace std;

TuteLAN::TuteLAN() :
		game_(nullptr), //
		exit_(false) {
	initGame();
}

TuteLAN::~TuteLAN() {
	closeGame();
}

void TuteLAN::initGame() {
	game_ = SDLGame::init("TuteLAN", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);
}

void TuteLAN::closeGame() {
    delete game_;
}

void TuteLAN::start() {
	exit_ = false;
	auto ih = InputHandler::instance();

	while (!exit_) {
		Uint32 startTime = game_->getTime();
		SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0xFF0000FF));
		SDL_RenderClear(game_->getRenderer());

		ih->update();
		if (ih->keyDownEvent()) {
			if (ih->isKeyDown(SDLK_ESCAPE)) {
				exit_ = true;
				break;
			}
		}

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

