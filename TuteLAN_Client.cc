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

    texture = game_->getTextureMngr()->getTexture(Resources::Deck);

	
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

void TuteLAN_Client::login(){
	TuteMSG msg(nick, TuteType::LOGIN, 0, 0);
    socket.send(msg);
}

void TuteLAN_Client::start() {

	// for(int i=0; i < 10; ++i){
	// 	hand.push_back({0, i});
	// }
	exit_ = false;
    //TO DO: mandar el nombre del login
	while (!exit_) {
		handleInput();
		render();
	}
}

void TuteLAN_Client::renderGame(){

	// Player cards render
	int iniCardPos = 200;
   for(int i=0; i < hand.size(); ++i){
		SDL_Rect rect, clip;
		rect = RECT(iniCardPos + CARD_OFFSET * i, _WINDOW_HEIGHT_/ 2 - CARD_HEIGHT / 2, CARD_WIDTH, CARD_HEIGHT);
		clip = RECT(CARD_WIDTH * hand[i].number, CARD_HEIGHT * hand[i].suit, 67, 102 );
		texture->render(rect,0, clip);
	}

	// int iniCardPos = 200;
    // for(int i=0; i < hand.size(); ++i){
	// 	SDL_Rect rect, clip;
	// 	rect = RECT(iniCardPos + CARD_OFFSET * i, _WINDOW_HEIGHT_/ 2 - CARD_HEIGHT / 2, CARD_WIDTH, CARD_HEIGHT);
	// 	clip = RECT(CARD_WIDTH * hand[i].number, CARD_HEIGHT * hand[i].suit, 67, 102 );
	// 	texture->render(rect,0, clip);
	// }

	// Render other players
	// double angle = 90;
	// for(int i = 0; i < 3; i++){
	// 	for(int j = 0; j < player[i]; j++){
	// 		SDL_Rect rect, clip;
	// 		rect = RECT(iniCardPos + CARD_OFFSET * i, _WINDOW_HEIGHT_- 80, CARD_WIDTH, CARD_HEIGHT);
	// 		clip = RECT(CARD_WIDTH * hand[i].number, CARD_HEIGHT * hand[i].suit, 67, 102 );
	// 		texture->render(rect,0, clip);
	// 	}
	// }
    //}
}


void TuteLAN_Client::render() {
	//recv el estado del juego del servidor
	//si es tu turno, actualizas input y todo
	Uint32 startTime = game_->getTime();
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x006600FF));
	SDL_RenderClear(game_->getRenderer());		

	// render entities
	//entityManager_->render();
	renderGame();

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
		else if(ih->isKeyDown(SDLK_c) /*&& turn%2 == client_ID*/ && input == "\0"){
			input = "c";
			std::cout << "Si pulsas o, c, e, b cantas con ese palo\n";
		}
		else if(input == "c"){//cantes
			if(ih->isKeyDown(SDLK_o) ){//rey y caballo de oros
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 0);
    			socket.send(msg);
				input="\0";
			}
			else if(ih->isKeyDown(SDLK_c) ){//rey y caballo de copas
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 1);
    			socket.send(msg);
				input="\0";
			}
			else if(ih->isKeyDown(SDLK_e) ){//rey y caballo de espadas
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 2);
    			socket.send(msg);
				input="\0";
			}
			else if(ih->isKeyDown(SDLK_b) ){//rey y caballo de bastos  
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 3);
    			socket.send(msg);
				input="\0";
			}
			else if(ih->isKeyDown(SDLK_t) ){//tute, 4 reyes o 4 caballos
				TuteMSG msg(nick, TuteType::CANTE_TUTE, client_ID, 0);
    			socket.send(msg);
				input="\0";
			}
			else if(ih->isKeyDown(SDLK_q)){//si pulsas otra tecla, deja de intentar cantar
				input="\0";
			}
		}
		playCard(ih);
	}
}

void TuteLAN_Client::playCard(InputHandler* ih){
	if(turn != client_ID) return;

	if(ih->isKeyUp(SDLK_0) && hand.size() > 0){//jugar la carta en la posicion 0
		TuteMSG msg(nick, TuteType::CARD, hand[0].number, hand[0].suit);
		std::cout<<"Has usado la carta " << (int)hand[0].number<< " de "<< (int)hand[0].suit << "\n";
		socket.send(msg);		
	}
	else if(ih->isKeyDown(SDLK_1)){//jugar la carta en la posicion 1
		if(hand.size() <= 1){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[1].number, hand[1].suit);
			std::cout<<"Has usado la carta " << (int)hand[1].number<< " de "<< (int)hand[1].suit << "\n";
			socket.send(msg);	
		}	
	}
	else if(ih->isKeyDown(SDLK_2) && hand.size() > 2){//jugar la carta en la posicion 2
		if(hand.size() <= 1){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[2].number, hand[2].suit);
			std::cout<<"Has usado la carta " << (int)hand[2].number<< " de "<< (int)hand[2].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_3) && hand.size() > 3){//jugar la carta en la posicion 3
		if(hand.size() <= 2){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[3].number, hand[3].suit);
			std::cout<<"Has usado la carta " << (int)hand[3].number<< " de "<< (int)hand[3].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_4) && hand.size() > 4){//jugar la carta en la posicion 4
		if(hand.size() <= 3){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[4].number, hand[4].suit);
			std::cout<<"Has usado la carta " << (int)hand[4].number<< " de "<< (int)hand[4].suit << "\n";
			socket.send(msg);	
		}		
	}
	else if(ih->isKeyDown(SDLK_5) && hand.size() > 5){//jugar la carta en la posicion 5
		if(hand.size() <= 4){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[5].number, hand[5].suit);
			std::cout<<"Has usado la carta " << (int)hand[5].number<< " de "<< (int)hand[5].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_6) && hand.size() > 6){//jugar la carta en la posicion 6
		if(hand.size() <= 5){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[6].number, hand[6].suit);
			std::cout<<"Has usado la carta " << (int)hand[6].number<< " de "<< (int)hand[6].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_7) && hand.size() > 7){//jugar la carta en la posicion 7
		if(hand.size() <= 6){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[7].number, hand[7].suit);
			std::cout<<"Has usado la carta " << (int)hand[7].number<< " de "<< (int)hand[7].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_8) && hand.size() > 8){//jugar la carta en la posicion 8
		if(hand.size() <= 7){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[8].number, hand[8].suit);
			std::cout<<"Has usado la carta " << (int)hand[8].number<< " de "<< (int)hand[8].suit << "\n";
			socket.send(msg);	
		}			
	}
	else if(ih->isKeyDown(SDLK_9) && hand.size() > 9){//jugar la carta en la posicion 9
		if(hand.size() <= 8){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[9].number, hand[9].suit);
			std::cout<<"Has usado la carta " << (int)hand[9].number<< " de "<< (int)hand[9].suit << "\n";
			socket.send(msg);	
		}			
	}
}

void TuteLAN_Client::recv_thread()
{	
    //TuteMSG received;
		TuteMSG received;
    while(true)
    {
        //Recibir Mensajes de red
		std::cout << "Esperando a recibir un mensaje\n";

        if(socket.recv(received) < 0){ 
			std::cout <<"ALGO MAL\n";
			continue;
		}
		std::cout <<"Mensaje recibido\n";
		switch (received.getType())
		{
		case TuteType::PRUEBA:{
			std::cout << "Esta mal\n";
			break;
		}
		case TuteType::LOGIN:{
			client_ID = received.getInfo_1();
			std::cout << "LOGIN: el id es: " << (int)client_ID << "\n";
			break;
		}
		case TuteType::TURN:{
			turn = received.getInfo_1();
			std::cout << "TURN: Es el turno de: "<< (int)turn << "\n";
			break;
		}
		case TuteType::HAND:
		{
			hand.push_back({ received.getInfo_1(), received.getInfo_2()});
			std::cout << "HAND: Recibo carta: " << (int)received.getInfo_1() << " " << (int)received.getInfo_2() << " para la mano, nº cartas" << hand.size() << "\n";

			break;
		}
		case TuteType::ILEGAL_MOVE:
		{
			std::cout << "Eres un tramposo! Movimiento ilegal\n";
			break;
		}
		case TuteType::CARD:
		{
			Card card ={ received.getInfo_1(), received.getInfo_2() };
			std::cout << "Quitar la carta usada\n";
			if(turn == client_ID)//si es mi turno ha sido la carta que he usado
			{
				int i=0;
				while(i<hand.size()){
					if(hand[i] == card){
						hand[i]=hand[hand.size()-1];
						hand.pop_back();
					}
					++i;
				}
			}// TO DO: si no, se pone en el centro
			break;
		}
		case TuteType::CANTE:
		{
			std::cout << "MENSAJE RECIBIDO NO CANTE\n";
			break;
		}

		case TuteType::CANTE_TUTE:
		{
			std::cout << "MENSAJE RECIBIDO NO CANTE_TUTE\n";
			break;
		}
		default:
		std::cout << "MENSAJE RECIBIDO NO ESPERADO\n";
		break;
		}
    }
}