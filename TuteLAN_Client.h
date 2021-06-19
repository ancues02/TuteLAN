#pragma once
#include <string>
#include <vector>

#include "EntityManager.h"
#include "SDLGame.h"
#include "Resources.h"
#include "HandComponent.h"
#include "Entity.h"
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
	void handleInput();
	void playCard(InputHandler* ih);
	SDLGame *game_;
	Texture* texture;
	EntityManager* entityManager_;
	
	std::string input;
	bool exit_;

	uint8_t client_ID;		// podemos deducir el equipo de aqui
	std::string nick;		//nombre del jugador
	std::vector<Card> hand; //las cartas que tiene
	Entity* player; 			//yo
	int players[3]; 	//cartas de cada jugador

	uint8_t turn;		//el turno 
	Socket socket;		//mi socket

	// TO DO: pasar el tamaño en la constructora
	const static int _WINDOW_WIDTH_ = 700;
	const static int _WINDOW_HEIGHT_ = 700;

	const static int CARD_YSCALE = 70;
	const static int CARD_XSCALE = 40;
	const static int CARD_WIDTH = 67;
	const static int CARD_HEIGHT = 102;
	const static int CARD_OFFSET = 20;

};