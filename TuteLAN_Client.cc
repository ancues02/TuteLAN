#include "TuteLAN_Client.h"

#include <assert.h>
#include "InputHandler.h"
#include "SDL_macros.h"


/*
 * Clase para el cliente del juego
 */
TuteLAN_Client::TuteLAN_Client(const char * s, const char * p, const char * n):
		socket(s, p, false), //
		nick(n) {
	initGame();
}

TuteLAN_Client::~TuteLAN_Client() {
	closeGame();
}

void TuteLAN_Client::initGame() {
	game_ = SDLGame::init("TuteLAN", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);
	entityManager_ = new EntityManager(game_);
	player = entityManager_->addEntity();
	playerLeft = entityManager_->addEntity();
	playerPartner = entityManager_->addEntity();
	playerRight = entityManager_->addEntity();


	player->addComponent<HandComponent>();
	
}

void TuteLAN_Client::closeGame() {
    delete game_;
	delete entityManager_;
}

int TuteLAN_Client::connectToServer(const char * addr, const char * port){
	addrinfo hints;
	addrinfo *serv_res;

	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	std::cout << "hola\n";
	int rc = getaddrinfo(addr, port, &hints, &serv_res);
    if( rc != 0){
        std::cerr << "[getaddrinfo]: " << gai_strerror(rc) << '\n';
        return -1;
    }
	std::cout << "adios\n";


    char buffer[80];
    int bytes;
    
    int ret = socket.connect(serv_res->ai_addr, serv_res->ai_addrlen);
    if(ret < 0){
        std::cerr << strerror(errno) << '\n';
        return -1;
    }
    
    freeaddrinfo(serv_res);
}
void TuteLAN_Client::start() {

	exit_ = false;
    //TO DO: mandar el nombre del login
	while (!exit_) {
		handleInput();
		render();
	}
}

void TuteLAN_Client::render() {
	//recv el estado del juego del servidor
	//si es tu turno, actualizas input y todo
	Uint32 startTime = game_->getTime();
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0xFF0000FF));
	SDL_RenderClear(game_->getRenderer());		

	// render entities
	entityManager_->render();

	SDL_RenderPresent(game_->getRenderer());

	Uint32 frameTime = game_->getTime() - startTime;
	if (frameTime < 10)
		SDL_Delay(10 - frameTime);
}

void TuteLAN_Client::handleInput() {
	auto ih = InputHandler::instance();
	ih->update();
	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_ESCAPE)) {
			exit_ = true;			
		}
		//aqui mandar los mensajes
	}
}

void TuteLAN_Client::recv_thread()
{
    TuteBase received;
    while(true)
    {
        //Recibir Mensajes de red
        if(socket.recv(received) < 0) continue;
		switch (received.getType())
		{
		case TuteType::HAND:
		{
			std::cout << "El cliente: "<< client_ID <<" recibe mano\n";
			Hand& handMsg = static_cast<Hand&>(received);
			hand = handMsg.getHand();
			client_ID=handMsg.getClient_ID();
			nick = handMsg.getNick();
			// auto it = player->getComponent<HandComponent>(ecs::HandComponent);//->setHand(handMsg.getHand());
			// it->setHand(hand);
			break;
		}
		case TuteType::ILEGAL_MOVE:
		{

			std::cout << "Eres un tramposo! Movimiento ilegal\n";
			break;
		}
		case TuteType::TURN:
		{
			TuteMSG& _turn = static_cast<TuteMSG&>(received);
			turn= _turn.getContent();
			std::cout << "Es el turno de: "<<turn;
		}
		case TuteType::CARD:
		{
			Card& card = static_cast<Card&>(received);
			if(turn == client_ID)//si es mi turno ha sido la carta que he usado
			{
				int i=0;
				while(i<hand.size()){
					if(hand[i] == card)
						hand[i]=hand[hand.size()-1];
						hand.pop_back();
					++i;
				}
			}
			break;
		}
		case TuteType::CANTE:
		{
			
			break;
		}

		case TuteType::CANTE_TUTE:
		{
			
			break;
		}
		}
    }
}