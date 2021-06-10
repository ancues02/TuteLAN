#pragma once

//partida-ronda-turno
//2sota 3 caballo 4 rey tres 10 as 11 
//cantar las 20 20 puntos y cantar las 40 40 puntos
//pinta

#include <vector>
#include <string>

#include "SDLGame.h"

#include "Serializable.h"
#include "TL_Socket.h"
#include "Card.h"

class TuteLAN_Server {

public:
	TuteLAN_Server(const char * s, const char * p);
	virtual ~TuteLAN_Server();

	void init_game();
	void update_game();

private:
	const uint8_t MAX_CLIENTS = 4;
	const uint8_t POINTS_TO_WIN = 5;


	void createDesk();
	void distributeCards();

	uint8_t turn;		// 4 turnos
	uint8_t pinta; //el palo dominante de la ronda
	std::vector<Card> desk;//mazo para repartir al principio
	std::vector<std::vector<Card>> handClients;//el servidor sabe de la mano de cada cliente
	uint8_t team1_points=0, team2_points=0; //los puntos de cada equipo

	std::vector<std::unique_ptr<Socket>> clients;
	Socket socket;
};


class TuteLAN_Client {

public:
	TuteLAN_Client(const char * s, const char * p, const char * n);
	virtual ~TuteLAN_Client();

	void start();

private:
	void initGame();
	void closeGame();

	SDLGame *game_;
	bool exit_;

	uint8_t client_ID;	// podemos deducir el equipo de aqui
	std::string nick;
	std::vector<Card> hand;

	Socket socket;

	// TO DO: pasar el tamaño en la constructora
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
