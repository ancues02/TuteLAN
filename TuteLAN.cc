#include "TuteLAN.h"

#include <assert.h>
#include "InputHandler.h"
#include "SDL_macros.h"
#include "algorithm"

using namespace std;

/*
 * Clase para el servidor del juego
 */
TuteLAN_Server::TuteLAN_Server(const char * s, const char * p) : socket(s,p) {
	socket.bind();
}

TuteLAN_Server::~TuteLAN_Server() {
}

void TuteLAN_Server::init_game() {
	/*
	 *	
	 * 	Espera a que se connecten los 4 jugadores y entonces
	 * 	lanza el juego
	 */
	
	int players=0;
    Socket* client;
    
	if(socket.listen(MAX_CLIENTS) == -1){
		std::cerr << "[listen]: uso de listen\n";
		return ;
	}
    

    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    struct sockaddr cliente;
    socklen_t longCliente=sizeof(sockaddr);
    while(players<MAX_CLIENTS){
        int cliente_socket=socket.accept(&cliente, &longCliente);

        if(cliente_socket==-1){
            std::cerr << "[accept] Error en accept\n";
            continue;
        }   
        getnameinfo(&cliente, longCliente, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICSERV || NI_NUMERICHOST);
        std::cout << "Conexion desde " << host << " " << serv << "\n"; 
        
        //ThreadTCP* miThread=new ThreadTCP(cliente_socket);
        //std::thread([miThread](){ miThread->do_message(); delete miThread;}).detach();
		client=new Socket(cliente_socket,&cliente,longCliente);
		clients.push_back(std::move(std::unique_ptr<Socket>(client)));   
		players++;     
    }

	turn = 0;
	createDesk();
	//distribute
    update_game();


}

void TuteLAN_Server::update_game() {
	/*
	 *	Actualiza el estado del juego, barajeando y repartiendo
	 * 	las cartas al inicio de la partida, y gestionando el turno
	 *	de cada jugador en la partida
	 */
	while(team1_points <= POINTS_TO_WIN && team2_points <= POINTS_TO_WIN){
		distributeCards();



	}
	/*if(socket.recv(msg, client) != 0){
		std::cerr << "[recv]: error en recv do_message\n";
			continue;
	}

	for(auto it = clients.begin(); it != clients.end() ; ++it){
		if(!(*client == *it->get())){
			socket.send(msg,*it->get());
		}                    
	}*/


}

//crea la baraja
void TuteLAN_Server::createDesk(){

	for (int i = 0; i < 40 ; ++i){
		desk.push_back(Card(i, i%10));
	}
	
}

//repartir cartas
void TuteLAN_Server::distributeCards()
{
	std::random_shuffle(desk.begin(), desk.end());
	
	int player=turn;
	for (int i = 0; i < 40 ; ++i){
		handClients[player%MAX_CLIENTS].push_back(desk[i]);
		player++;		
	}	
	pinta = desk[39].getSuit();
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

