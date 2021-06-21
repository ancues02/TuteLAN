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

// Clase para el cliente del juego 
class TuteLAN_Client {
public:
	TuteLAN_Client(const char * s, const char * p, const char * n);
	virtual ~TuteLAN_Client();
	
	// Conecta el jugador al servidor
    int connectToServer(const char * addr, const char * port);

	// Inicia el bucle de input y renderizado del hilo principal
	void start();

	// Gestiona los mensajes recibidos del servidor (en otro hilo)
    void recv_thread();

	// Manda un mensaje de LOGIN al servidor
	void login();
private:
	// Inicializa SDL y las variables del juego 
	void initGame();

	// Cierra SDL
	void closeGame();

	// Bucle de render de SDL
	void render();

	// Renderiza las cartas, la pinta y el turno
	void renderGame();
	
	// Renderiza los puntos de cada equipo
	void renderPoints();

	// Renderiza un texto temporalmente
	// Tambien lo usamos para terminar la partida si se
	// recibe el mensaje de fin
	void renderTempTxt();

	// Gestiona el input de la ventana de SDL
	void handleInput();

	// Manda una carta segun el input
	void playCard(InputHandler* ih);

	const static int _WINDOW_WIDTH_ = 700;
	const static int _WINDOW_HEIGHT_ = 700;

	const static int CARD_WIDTH = 67;
	const static int CARD_HEIGHT = 102;
	const static int CARD_OFFSET = 20;
	
	// Variable para cerrar el juego
	bool exit_;
	// Variable para gestionar si estamos
	// en el juego o esperando a otros jugadores
	bool in_game_;
	// Variable para terminar el juego cuando
	// termina una partida
	bool endGame=false;
	// Para gestionar los recursos y la ventana
	SDLGame* game_;
	// Textura de las cartas
	Texture* texture;
	// Textura dle turno
	Texture* turnTexture;
	// Texto temporal
	std::string tmpTxt;
	// Limite de tiempo para el texto temporal
	int tmp_limit;
	// Para recoger la tecla del jugador al cantar
	std::string input;
	
	// ID
	uint8_t client_ID;
	// Nombre del jugador
	std::string nick;
	// Cartas del jugador
	std::vector<Card> hand;
	// Cartas en la mesa
	std::vector<Card> roundCards;
	// Numero de cartas de cada jugador
	int players[4]; 	
	// El palo dominante del juego, si es 4, no hay pinta todavía
	uint8_t pinta_suit; 
	// Numero de la pinta para renderizarla
	uint8_t pinta_num;
	// El turno 
	uint8_t turn;	
	// Socket del cliente	
	Socket socket;
	// Puntos de la partida de cada equipo
	uint8_t myTeamPoints,
			otherTeamPoints;

};