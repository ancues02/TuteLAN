#pragma once
#include <string>
#include <vector>

#include "EntityManager.h"
#include "SDLGame.h"
#include "Resources.h"
#include "HandComponent.h"
#include "Entity.h"


#include "TL_Socket.h"
#include "TuteSerializable.h"
#include "Card.h"


class TuteLAN_Client {

public:
	TuteLAN_Client(const char * s, const char * p, const char * n);
	virtual ~TuteLAN_Client();

    int connectToServer(const char * addr, const char * port);
	void start();
    void recv_thread();
private:
	void initGame();
	void closeGame();

	void render();
	void handleInput();

	SDLGame *game_;
	EntityManager* entityManager_;

	bool exit_;

	uint8_t client_ID;		// podemos deducir el equipo de aqui
	std::string nick;		//nombre del jugador
	std::vector<Card> hand; //las cartas que tiene
	Entity* player; 			//yo
	Entity* playerLeft; 		//el jugador de mi izquierda
	Entity* playerPartner; 	//mi compañero
	Entity* playerRight; 	//el jugador de mi derecha

	uint8_t turn;		//el turno 
	Socket socket;		//mi socket

	// TO DO: pasar el tamaño en la constructora
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};