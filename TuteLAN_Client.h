#pragma once
#include <string>
#include <vector>

#include "SDLGame.h"
#include "Resources.h"
#include "Texture.h"

#include "TL_Socket.h"
#include "TuteSerializable.h"
#include "Card.h"

class InputHandler;
class TuteLAN_Client {

public:
	TuteLAN_Client(const char * s, const char * p, const char * n);
	virtual ~TuteLAN_Client();

    int connectToServer(const char * addr, const char * port);
	void start();
    void recv_thread();
	void login();
private:
	void initGame();
	void closeGame();
	void renderGame();

	void render();
	void renderPoints();
	void renderTempTxt();

	void handleInput();
	void playCard(InputHandler* ih);
	
	SDLGame *game_;
	Texture* texture;
	Texture* turnTexture;

	std::string tmpTxt;
	int tmp_limit;

	std::string input;
	bool exit_;
	bool in_game_;

	uint8_t client_ID;		// podemos deducir el equipo de aqui
	std::string nick;		//nombre del jugador
	std::vector<Card> hand; //las cartas que tiene
	std::vector<Card> roundCards;
	int players[3]; 	//cartas de cada jugador

	uint8_t pinta_suit; // el palo dominante del juego, si es 4, no hay pinta todavía
	uint8_t pinta_num;	// numero de la pinta para renderizarla
	uint8_t turn;		//el turno 
	Socket socket;		//mi socket

	uint8_t myTeamPoints,
			otherTeamPoints;

	// TO DO: pasar el tamaño en la constructora
	const static int _WINDOW_WIDTH_ = 700;
	const static int _WINDOW_HEIGHT_ = 700;

	const static int CARD_YSCALE = 70;
	const static int CARD_XSCALE = 40;
	const static int CARD_WIDTH = 67;
	const static int CARD_HEIGHT = 102;
	const static int CARD_OFFSET = 20;

};