#pragma once
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <string>
#include <assert.h>
#include "algorithm"
#include <thread>
#include <mutex>

#include "TL_Socket.h"
#include "TuteSerializable.h"
#include "Card.h"
class TuteLAN_Server {

	enum State : uint8_t { ROUND_START, RUNNING, ROUND_OVER };

public:
	TuteLAN_Server(const char * s, const char * p);
	virtual ~TuteLAN_Server();

	void init_game();
	void update_game();

	// Thread function
	void handle_message(TuteMSG& received, bool& _exit);
private:
	static const uint8_t MAX_CLIENTS = 4;
	const uint8_t POINTS_TO_WIN = 1;
	// TO DO: hacer array
	const std::map<uint8_t, std::pair<uint8_t, int>> CARD_VALUES = {
		{ 0,{10, 11}},
		{ 1,{1, 0}},
		{ 2,{9, 10}},
		{ 3,{2, 0}},
		{ 4,{3, 0}},
		{ 5,{4, 0}},
		{ 6,{5, 0}},
		{ 7,{6, 2}},
		{ 8,{7, 3}},
		{ 9,{8, 4}}
	};

	void broadcast_message(TuteMSG& msg);
	void disconnect_player(Socket* sock);
	void wait_players();
	// TO DO: Mover este struct
	struct Team{
		int gamePoints = 0;
		bool cantes[4] = {false, false, false, false};	// oros, copas, espadas, bastos
	};

	void createDesk();
	void distributeCards();
	bool legalCard(const Card& card);
	bool legalCante(const TuteMSG& cante);
	bool legalCanteTute(const TuteMSG& cante);
	uint8_t roundWinner();
	void gameWinner();
	void endGame();

	std::mutex m;
	bool disconnection;
	std::string player_nicks[MAX_CLIENTS];
	uint8_t turn;		// 4 turnos cada ronda
	uint8_t mano;		// el que reparte al inicio del juego
	uint8_t pinta; 		// el palo dominante del juego
	uint8_t roundSuit; 	// el palo al que hay que ir en la ronda

	std::vector<Card> desk;	//mazo para repartir al principio del juego
	
	std::vector<std::pair<Card, uint8_t>> round_cards;	// cartas y jugadores de una ronda
	std::vector<Card> team1_cards;	// juego
	std::vector<Card> team2_cards;	// juego

	std::vector<std::vector<Card>> handClients;//el servidor sabe de la mano de cada cliente
	//int t1_gamePoints = 0, t2_gamePoints = 0;	// los puntos de cada equipo por juego
	Team team1, team2;
	uint8_t team1_points=0, team2_points=0; //los puntos de cada equipo, por partida

	int roundCount, turnCount;

	std::vector<std::pair<std::unique_ptr<Socket>, uint8_t>> clients;	//socket e ID
	std::queue<uint8_t> available_IDs;
	Socket socket;
};
