#pragma once
#include <string>
#include <vector>

#include "SDLGame.h"

#include "TL_Socket.h"
#include "TuteSerializable.h"

class TuteLAN_Client {

public:
	TuteLAN_Client(const char * s, const char * p, const char * n);
	virtual ~TuteLAN_Client();

	void start();

private:
	void initGame();
	void closeGame();

	void render();
	void handleInput();

	SDLGame *game_;
	bool exit_;

	uint8_t client_ID;	// podemos deducir el equipo de aqui
	std::string nick;
	std::vector<Card> hand;
	bool myTurn;
	Socket socket;

	// TO DO: pasar el tamaño en la constructora
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};