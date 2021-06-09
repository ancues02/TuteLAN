#include "TuteLAN.h"

#include <assert.h>
#include "InputHandler.h"
#include "SDL_macros.h"

using namespace std;

/*
 * Clase para el servidor del juego
 */
TuteLAN_Server::TuteLAN_Server(const char * s, const char * p) : socket(s,p) {
}

TuteLAN_Server::~TuteLAN_Server() {
}

void TuteLAN_Server::init_game() {
	/*
	 *	Crea el socket del servidor
	 * 	Espera a que se connecten los 4 jugadores y entonces
	 * 	lanza el juego
	 */
}

void TuteLAN_Server::update_game() {
	/*
	 *	Actualiza el estado del juego, barajeando y repartiendo
	 * 	las cartas al inicio de la partida, y gestionando el turno
	 *	de cada jugador en la partida
	 */
}


/*
 * Clase para el cliente del juego
 */
TuteLAN_Client::TuteLAN_Client(const char * s, const char * p, const char * n):
		socket(s, p), //
		nick(n) {
	initGame();
}

TuteLAN_Client::~TuteLAN_Client() {
	closeGame();
}

void TuteLAN_Client::initGame() {
	game_ = SDLGame::init("TuteLAN", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);
}

void TuteLAN_Client::closeGame() {
    delete game_;
}

void TuteLAN_Client::start() {
	exit_ = false;
	auto ih = InputHandler::instance();

	while (!exit_) {
		//recv el estado del juego del servidor
		//si es tu turno, actualizas input y todo
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

