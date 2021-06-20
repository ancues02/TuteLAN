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

    texture = game_->getTextureMngr()->getTexture(Resources::Deck);
    turnTexture = game_->getTextureMngr()->getTexture(Resources::Turn);
	pinta_suit = 4;
	pinta_num = 1;
	turn=0;
	myTeamPoints = otherTeamPoints = 0;
	tmpTxt = "Hola que tal";
	tmp_limit = game_->getTime() + 4000;
}

void TuteLAN_Client::closeGame() {
    delete game_;
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

	in_game_ = false;
	exit_ = false;
	while (!exit_) {
		handleInput();
		render();
	}
}

void TuteLAN_Client::renderGame(){

	if(!in_game_){
		Texture score (	game_->getRenderer(), 
        				"Waiting for players...",
            			game_->getFontMngr()->getFont(Resources::ARIAL24),
            			{ COLOR(0x111122ff) });
    	score.render( _WINDOW_WIDTH_ / 2 - score.getWidth() / 2, _WINDOW_HEIGHT_ / 2 - score.getHeight());
	}
	else {
		// Player cards render
		int iniCardPos = 200;
		int despl = 300;
		SDL_Rect rect, clip;

		//Render cartas de cada jugador
		for(int i=0; i < hand.size(); ++i){
			// Jugador
			rect = RECT(iniCardPos - 70 + (40 * i), _WINDOW_HEIGHT_/ 2 - CARD_HEIGHT / 2 + despl, CARD_WIDTH, CARD_HEIGHT);
			clip = RECT(CARD_WIDTH * hand[i].number, CARD_HEIGHT * hand[i].suit, 67, 102 );
			texture->render(rect,0, clip);
			// Jugador arriba
			rect = RECT(iniCardPos + CARD_OFFSET * i,_WINDOW_HEIGHT_/ 2 - CARD_HEIGHT / 2 - despl, CARD_WIDTH, CARD_HEIGHT);
			clip = RECT(CARD_WIDTH, CARD_HEIGHT * 4, 67, 102 );
			texture->render(rect,180, clip);
			// Jugador derecha
			rect = RECT( _WINDOW_WIDTH_ / 2 - CARD_WIDTH / 2 + despl, iniCardPos + CARD_OFFSET * i, CARD_WIDTH, CARD_HEIGHT);
			texture->render(rect,90, clip);
			// Jugador izquierda
			rect = RECT( _WINDOW_WIDTH_ / 2 - CARD_WIDTH / 2 - despl, iniCardPos + CARD_OFFSET * i, CARD_WIDTH, CARD_HEIGHT);
			texture->render(rect,270, clip);
		}

		//Render cartas en la mesa
		int center_offset = 80;
		for(int i = 0; i < roundCards.size(); i++){
			rect = RECT((_WINDOW_WIDTH_ / 2 - CARD_WIDTH / 2) - center_offset + i * CARD_WIDTH, _WINDOW_HEIGHT_/ 2 - CARD_HEIGHT / 2, CARD_WIDTH, CARD_HEIGHT);
			clip = RECT(CARD_WIDTH * roundCards[i].number, CARD_HEIGHT * roundCards[i].suit, 67, 102 );
			texture->render(rect,0, clip);
		}		

		// Render pinta
		rect = RECT(_WINDOW_WIDTH_ - CARD_WIDTH - CARD_WIDTH / 4, CARD_HEIGHT / 2, CARD_WIDTH, CARD_HEIGHT);
		clip = RECT(CARD_WIDTH * (int)pinta_num, CARD_HEIGHT * (int)pinta_suit, 67, 102 );
		texture->render(rect,0, clip);

		//Render turno
		int turnPos = (turn - client_ID + 4) % 4;
		//std::cout << "Seno: " <<  sin(turnPos  * 90) << "  Coseno: " <<  cos(turnPos  * 90) << "\n";
		rect = RECT((_WINDOW_WIDTH_ / 2 - turnTexture->getWidth() / 2) + (180 * sin(turnPos * M_PI / 180 * 90)),
					(_WINDOW_HEIGHT_ / 2  - turnTexture->getHeight() / 2)+ (180 * cos(turnPos * M_PI / 180 * 90)), 50,50);
		turnTexture->render(rect);
	
	}
	
}
void TuteLAN_Client::renderPoints(){
	// Render de puntuacion
	Texture score (	game_->getRenderer(), 
					"MyTeam: " + to_string(myTeamPoints),
					game_->getFontMngr()->getFont(Resources::ARIAL16),
					{ COLOR(0xffffffff) });
	score.render( _WINDOW_WIDTH_ / 12 - score.getWidth() / 2, score.getHeight());
	Texture oscore (	game_->getRenderer(), 
					"OtherTeam: " + to_string(otherTeamPoints),
					game_->getFontMngr()->getFont(Resources::ARIAL16),
					{ COLOR(0x999999ff) });

	oscore.render( _WINDOW_WIDTH_ / 12 - score.getWidth() / 2, 2 * score.getHeight());
}

void TuteLAN_Client::renderTempTxt(){
	int remTime = tmp_limit - game_->getTime();
	if(remTime > 0){
		Texture score (	game_->getRenderer(), 
						tmpTxt,
						game_->getFontMngr()->getFont(Resources::ARIAL16),
						{ COLOR(0x992211ff) });
		score.render( _WINDOW_WIDTH_ / 2 - score.getWidth() / 2,_WINDOW_HEIGHT_ / 2 + score.getHeight());
	}
}


void TuteLAN_Client::render() {
	//recv el estado del juego del servidor
	//si es tu turno, actualizas input y todo
	Uint32 startTime = game_->getTime();
	SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x006600FF));
	SDL_RenderClear(game_->getRenderer());		

	renderGame();
	renderPoints();
	renderTempTxt();

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
			TuteMSG msg(nick, TuteType::DISCONNECT, client_ID, 0);
			socket.send(msg);	
			std::cout << "ME voy\n";	
		}
		
		else if(!in_game_) return;

		//aqui mandar los mensajes
		else if(ih->isKeyDown(SDLK_c) && input == "\0"){
			input = "c";
			std::cout << "Si pulsas o, c, e, b cantas con ese palo\n";
		}
		else if(input == "c"){//cantes
			if(ih->isKeyDown(SDLK_o) ){//rey y caballo de oros
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 0);
    			socket.send(msg);
				input="\0";
				std::cout << "Cantar en oros\n";
			}
			else if(ih->isKeyDown(SDLK_c) ){//rey y caballo de copas
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 1);
    			socket.send(msg);
				input="\0";
				std::cout << "Can39tar en copas\n";
			}
			else if(ih->isKeyDown(SDLK_e) ){//rey y caballo de espadas
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 2);
    			socket.send(msg);
				input="\0";
				std::cout << "Cantar en espadas\n";
			}
			else if(ih->isKeyDown(SDLK_b) ){//rey y caballo de bastos  
				TuteMSG msg(nick, TuteType::CANTE, client_ID, 3);
    			socket.send(msg);
				input="\0";
				std::cout << "Cantar en bastos\n";
			}
			else if(ih->isKeyDown(SDLK_t) ){//tute, 4 reyes o 4 caballos
				TuteMSG msg(nick, TuteType::CANTE_TUTE, client_ID, 0);
    			socket.send(msg);
				input="\0";
				std::cout << "Cantar en tute\n";
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

	if(ih->isKeyDown(SDLK_t)){//jugar la carta en la posicion 0
		if(hand.size() <= 0){
			std::cout<<"No tienes cartas suficientes, prueba a pulsar un numero menor\n";
		}
		else{
			TuteMSG msg(nick, TuteType::CARD, hand[0].number, hand[0].suit);
			std::cout<<"Has usado la carta " << (int)hand[0].number<< " de "<< (int)hand[0].suit << "\n";
			socket.send(msg);	
		}	
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
		case TuteType::LOGIN:{
			client_ID = received.getInfo_1();
			std::cout << "LOGIN: el id es: " << (int)client_ID << "\n";
			break;
		}
		case TuteType::TURN:{
			turn = received.getInfo_1();
			std::cout << "TURN: Es el turno de: "<< (int)turn << "\n";
			if(roundCards.size()>=4)
				roundCards.clear();
			break;
		}
		case TuteType::HAND:
		{
			// To Do: hacer mensaje
			in_game_ = true;
			hand.push_back({ received.getInfo_1(), received.getInfo_2()});
			std::cout << "HAND: Recibo carta: " << (int)received.getInfo_1() << " " << (int)received.getInfo_2() << " para la mano, nº cartas" << hand.size() << "\n";
				
			break;
		}
		case TuteType::ILEGAL_MOVE:
		{
			std::cout << "Eres un tramposo! Movimiento ilegal\n";
			break;
		}
		case TuteType::PINTA:
		{
			pinta_num = received.getInfo_1();
			pinta_suit = received.getInfo_2();
			std::cout << "La pinta es: " << (int)pinta_suit << " "<< (int)pinta_num << "\n";
			break;
		}
		case TuteType::CARD:
		{
			Card card ={ received.getInfo_1(), received.getInfo_2() };
			if(turn == client_ID)//si es mi turno ha sido la carta que he usado
			{
				std::cout << "Quitar la carta usada: "<< (int)card.number<<" "<<(int)card.suit <<"\n";
				int i=0;
				while(i<hand.size()){
					if(hand[i] == card){
						hand[i]=hand[hand.size()-1];
						hand.pop_back();
						break;
					}
					++i;
				}
			}			
			roundCards.push_back(card);
			
			break;
		}
		case TuteType::CANTE:
		{
			//info1 es cliente e info 2 es palo
			int points=20;
			if(received.getInfo_2() == pinta_suit)
				points=40;
			//TO DO poner mensaje de texto en pantalla
			std::cout << "El jugador " << received.getInfo_1() << "ha cantado "<< points << "en " << received.getInfo_2() <<"\n"; 
			break;
		}

		case TuteType::CANTE_TUTE:
		{
			//info1 es el jugador
			std::cout << "El jugador " << received.getInfo_1() << "ha cantado tute, ";
			if(received.getInfo_1() % 2 == client_ID % 2) std::cout << "tu equipo ha ganado\n";
			else std::cout << "su equipo ha ganado\n";
			//TO DO poner mensaje de texto en pantalla
			break;
		}
		case TuteType::TUTE_WINNER:
		{
			exit_=true;
			break;
		}
		case TuteType::WAIT:
		{
			in_game_ = false;
			hand.clear();
			roundCards.clear();
			for(int i = 0; i < 3; i++){
				players[i] = 0;
			}
			break;
		}
		default:
			std::cout << "MENSAJE RECIBIDO NO ESPERADO\n";
			break;
		}
    }
}