#pragma once
/* 
* TO DO:
* El metodo de cantar y comprobar cante, puede cantar el jugador o su compañero
* Ajustar el fin de las partida (echar clientes o algo)
* Hacer el cliente
* Cambiar el nombre de Card.h 
*/

#include <vector>
#include <utility>
#include <map>
#include <string>

#include "SDLGame.h"

#include "Serializable.h"
#include "TL_Socket.h"
#include "Card.h"

class TuteLAN_Server {

	enum State : uint8_t { ROUND_START, RUNNING, ROUND_OVER };

public:
	TuteLAN_Server(const char * s, const char * p);
	virtual ~TuteLAN_Server();

	void init_game();
	void update_game();

private:
	const uint8_t MAX_CLIENTS = 4;
	const uint8_t POINTS_TO_WIN = 5;
	const std::map<uint8_t, std::pair<uint8_t, int>> CARD_VALUES = {
		{ 1,{10, 11}},
		{ 2,{1, 0}},
		{ 3,{9, 10}},
		{ 4,{2, 0}},
		{ 5,{3, 0}},
		{ 6,{4, 0}},
		{ 7,{5, 0}},
		{ 10,{6, 2}},
		{ 11,{7, 3}},
		{ 12,{8, 4}}
	};

	void createDesk();
	void distributeCards();
	bool legalCard(const Card& card);
	bool legalCante();
	uint8_t roundWinner();
	void gameWinner();


	uint8_t turn;		// 4 turnos cada ronda
	uint8_t mano;		// el que reparte al inicio del juego
	uint8_t pinta; 		// el palo dominante del juego
	uint8_t roundSuit; 	// el palo al que hay que ir en la ronda

	std::vector<Card> desk;	//mazo para repartir al principio del juego
	
	std::vector<std::pair<Card, uint8_t>> round_cards;	// cartas y jugadores de una ronda
	std::vector<Card> team1_cards;	// juego
	std::vector<Card> team2_cards;	// juego

	std::vector<Hand> handClients;//el servidor sabe de la mano de cada cliente
	int t1_gamePoints = 0, t2_gamePoints = 0;
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
