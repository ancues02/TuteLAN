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
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "TL_Socket.h"
#include "TuteSerializable.h"
#include "Card.h"

// Clase para el servidor del juego
class TuteLAN_Server {
public:
	TuteLAN_Server(const char * s, const char * p);
	virtual ~TuteLAN_Server();

	// Pone a escuchar el servidor e inicializa los IDs
	void init_game();

	// Funcion que se realiza en el hilo principal del servidor
	// y controla una partida y las desconexiones
	void update_game();

	// Funcion que gestiona los mensajes que hay que mandar y recibir
	// de cada cliente (thread por cliente)	
	void handle_message(TuteMSG& received, uint8_t clientPos, bool& _exit);
private:
	static const uint8_t MAX_CLIENTS = 4;
	const uint8_t POINTS_TO_WIN = 5;

	// Mapa de carta con sus valores y puntuacion
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

	// Manda un mensaje a todos los clientes
	void broadcast_message(TuteMSG& msg);

	// Espera a que se conecten los clientes necesarios
	// y crea su correspondiente thread
	void wait_players();
	
	// Inicializa la baraja y crea las manos de cada jugador
	void createDesk();
	
	// Se hace al iniciar una partida. 
	// Barajea aletoriamente las cartas y decide la pinta de la partida
	void distributeCards();

	// Comprueba si es posible poner la carta
	bool legalCard(const Card& card);

	// Comprueba si es posible cantar del palo que se indique
	bool legalCante(const TuteMSG& cante);

	// Comprueba si es posible cantar tute 
	bool legalCanteTute(const TuteMSG& cante);

	// Decide que equipo es el ganador de la ronda
	uint8_t roundWinner();

	// Decide que equipo es el ganador del juego y 
	// actualiza sus puntos
	void gameWinner();

	// Decide que equipo gana la partida
	// y vuelve a esperar conexiones
	void endTute();

	// Pone los valores de todas las variables
	// de la partida a sus valores iniciales
	void restart();

	// Representa un equipo, su puntuación del juego
	// y si ha cantado segun que palo
	struct Team{
		int gamePoints = 0;
		bool cantes[4] = {false, false, false, false};	// oros, copas, espadas, bastos
	};

	// Representa la desconexion de algun cliente
	bool disconnection;

	// Variables del servidor

	// Nicks de cada jugador
	std::string player_nicks[MAX_CLIENTS];
	// Posicion de cada cliente en el vector de clientes
	std::array<uint8_t, MAX_CLIENTS> clientsPos;
	// Vector de sockets de cada cliente y su ID
	std::vector<std::pair<std::unique_ptr<Socket>, uint8_t>> clients;
	// Cola de IDs disponibles para nuevos clientes
	std::queue<uint8_t> available_IDs;
	// Socket del servidor
	Socket socket;
	// Condicion para (des)bloquear el thread principal
	pthread_cond_t wakeUp;
	// Mutex para controlar regiones criticas
	pthread_mutex_t m;

	// Variables de la partida

	// ID del jugador que le toca poner carta (4 turnos cada ronda)
	uint8_t turn;	
	// ID el que reparte al inicio del juego	
	uint8_t mano;	
	// El palo dominante del juego	 
	uint8_t pinta; 	
	// El palo al que hay que ir en la ronda	
	uint8_t roundSuit; 	
	// Baraja para repartir al principio del juego
	std::vector<Card> desk;	
	// Cartas y jugadores de una ronda
	std::vector<std::pair<Card, uint8_t>> round_cards;	
	// Cartas que han ganado los equipos en un juego
	std::vector<Card> team1_cards;
	std::vector<Card> team2_cards;
	// El servidor crea y actualiza la mano de cada cliente
	std::vector<std::vector<Card>> handClients;
	// Equipos con su puntuacion y cantes
	Team team1, team2;
	// Los puntos de cada equipo, por partida
	uint8_t team1_points=0, team2_points=0;

	// Contador de las rondas de un juego
	// y de los turnos de una ronda
	int roundCount, turnCount;
};