#include "TuteLAN_Server.h"

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
	std::cout << "Servidor iniciado y esperando " << MAX_CLIENTS << " clientes\n";
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
        
        
		client=new Socket(cliente_socket,&cliente,longCliente);
		clients.push_back(std::move(std::unique_ptr<Socket>(client)));
    	TuteMSG msg1;
		if(client->recv(msg1) < 0){
			std::cout << "SERVER: Error recibiendo mensaje\n";
			delete client;
			clients.pop_back();
		}
		else {
			player_nicks[players] = msg1.getNick();
			
			//msg = TuteMSG(player_nicks[players],TuteType::LOGIN, players, 0);
			//socket.send( msg, *clients[0].get());
			for(int i=0; i < 4; ++i){
				TuteMSG msg = TuteMSG(player_nicks[players],TuteType::LOGIN, players, 0);
				client->send(msg);
			}
			//TuteMSG msg = TuteMSG(player_nicks[players],TuteType::TURN, players, 0);
			//socket.send( msg, *client);

			players++;
		}
    }

	turn = 0;
	createDesk();
    update_game();


}

void TuteLAN_Server::update_game() {
	/*
	 *	Actualiza el estado del juego, barajeando y repartiendo
	 * 	las cartas al inicio de la partida, y gestionando el turno
	 *	de cada jugador en la partida
	 */
	mano = 0;
	turn = 0;
	team1 = Team();
	team2 = Team();
	// Partida
	while(team1_points <= POINTS_TO_WIN && team2_points <= POINTS_TO_WIN){
		// Comienzo de juego
		distributeCards();

		//sleep(100000);
		mano %= MAX_CLIENTS;
		turn = (mano + 1) % MAX_CLIENTS;

		TuteMSG msg_send;	//mensaje que enviamos
		Socket* client;
		
		int roundCount = 0;
		//Gestion de rondas (10)
		while(roundCount < 10){
			int turnCount = 0;
			// Gestion de turnos (4)
			while(turnCount < 4){
				TuteMSG received;	//mensaje que recibimos
				// Broadcast del turno 
				msg_send = TuteMSG(player_nicks[turn], TuteType::TURN, turn, 0);		
				for(int i = 0; i < clients.size(); i++){	
					clients[i].get()->send(msg_send);                    
				}
				
				if(clients[turn]->recv(received) < 0){
					std::cout << "SERVER: Error recibiendo mensaje\n";
					continue;
				}
				switch (received.getType())
				{
				case TuteType::CARD:
				{
					Card card = { received.getInfo_1(), received.getInfo_2()};
					if(legalCard(card)){
						// Quitamos la carta del jugador que la puso
						int i = 0;
						while(i < handClients[turn].size()){ 
							if(handClients[turn][i] == card){	
								handClients[turn][i] = handClients[turn][handClients[turn].size() - 1];
								handClients[turn].pop_back();
								break;
							}
							i++; 
						}
						// Poner la carta en 'round_cards'
						round_cards.push_back({ card, turn });

						// Mandar a todos la carta
						msg_send = TuteMSG(player_nicks[turn], TuteType::CARD, card.number, card.suit);
						for(int i = 0; i < clients.size(); i++){
							clients[i].get()->send( msg_send);
						}

						// Aumentamos turno
						turnCount++;
						// Actualizamos el siguiente jugador que le toca 
						turn = (turn + 1) % MAX_CLIENTS;
						
					}
					// Si la carta no es legal tiene que volver a elegir
					else{
						msg_send = TuteMSG(player_nicks[turn], TuteType::ILEGAL_MOVE, 0,0);
						clients[turn].get()->send(msg_send);
					}
					break;
				}

				case TuteType::CANTE:
				{
					int points = 20;
					// el contenido en estos mensajes es el ID del jugador
					if(legalCante(msg_send)){
						// Comprobamos si canta 40
						if(msg_send.getInfo_2() == pinta)
							points = 40;
						if(turn % 2 == 0){
							team1.gamePoints += points;
							team1.cantes[msg_send.getInfo_2()] = true;		
						}				
						else{
							team2.gamePoints += points;		
							team2.cantes[msg_send.getInfo_2()] = true;		
						}					
					}
					break;
				}

				case TuteType::CANTE_TUTE:
				{
					if(legalCanteTute(msg_send)){
						if(turn % 2 == 0){}
							// team1 wins
						else{}
							// team2 wins
					}
					break;
				}
				}
			}
			// Decidir que equipo gana la ronda
			turn = roundWinner();
			// Mandar mensaje a todos los jugadores
			roundCount++;
		}
		gameWinner();
		mano++;
	}
	// Mandar mensaje a todos los jugadores con el equipo ganador
	// Esperar y cerrar conexiones
}

//crea la baraja
void TuteLAN_Server::createDesk(){

	int suit=-1;
	for (int i = 0; i < 40 ; ++i){
		if(i%10==0)
			suit++;
		desk.push_back( { i%10, suit });
	}

	for(uint8_t i=0; i< MAX_CLIENTS; ++i){
		handClients.push_back(std::vector<Card>());
	}
}

// Barajar y repartir cartas
void TuteLAN_Server::distributeCards()
{
	//std::random_shuffle(desk.begin(), desk.end());

	
	int player=mano;

	for (int i = 0; i < 10 * MAX_CLIENTS ; ++i){
		handClients[player%MAX_CLIENTS].push_back(desk[i]);
		player++;		
	}	
	pinta = desk[39].suit;
/*
std::cout<<"----------------------------------------------------------------------\n";
	handClients[0].to_bin();
	handClients[0].from_bin(handClients[0].data());
std::cout<<"----------------------------------------------------------------------\n";*/
	TuteMSG msg;
	sleep(1);
	for(int i = 0; i<clients.size(); ++i){
		for(int j = 0; j < 10; j++){		
			msg = TuteMSG(player_nicks[i], TuteType::HAND, handClients[i][j].number,  handClients[i][j].suit);
			clients[i].get()->send(msg);
			sleep(1);
		}		
	}
	msg = TuteMSG(player_nicks[0],TuteType::LOGIN, 0, 0);

	//msg = TuteMSG(player_nicks[0], TuteType::HAND, handClients[0][2].number,  handClients[0][2].suit);

	//socket.send(msg, *clients[0].get());
	

	/*for(int i = 0; i<10; ++i){
		handClients[0].getHand()[i].to_bin();
		handClients[0].getHand()[i].from_bin(handClients[0].getHand()[i].data());		
	}*/
}

// Comprueba si el cliente puede poner la carta
bool TuteLAN_Server::legalCard(const Card& card)
{
	if(round_cards.empty()) {
		roundSuit = card.suit;
		return true;
	}
	if(card.suit == roundSuit)
		return true;
	
	if(card.suit == pinta){
		for(int i = 0; i < handClients[turn].size(); i++){
			if(handClients[turn][i].suit == roundSuit)
				return false;
		}
		return true;
	}
	else {
		for(int i = 0; i < handClients[turn].size(); i++){
			if(handClients[turn][i].suit == roundSuit || handClients[turn][i].suit == pinta)
				return false;
		}
	}
	return true;
}

bool TuteLAN_Server::legalCante(const TuteMSG& _cante)
{
	Team t;
	if(_cante.getInfo_1() % 2 == 0)
		t = team1;
	else
		t = team2;

	if(round_cards.empty() || turn % 2 != _cante.getInfo_1() % 2 || t.cantes[_cante.getInfo_2()]) 
		return false;
	
	auto playerHand = handClients[_cante.getInfo_1()];
	int puedeCantar = 0;
	int i = 0;
	while( i < playerHand.size() && puedeCantar < 2){
		if(_cante.getInfo_2() == playerHand[i].suit && playerHand[i].number > 7)
			puedeCantar++;
		i++;
	}
	
	return puedeCantar == 2;
}
//
// En el mensaje de cante de tute, el contenido es el ID del jugador
bool TuteLAN_Server::legalCanteTute(const TuteMSG& _cante)
{
	if(round_cards.empty() || turn % 2 != _cante.getInfo_1() % 2) {
		return false;
	}
	auto playerHand = handClients[_cante.getInfo_1()];
	int nReyes = 0,
		nCaballos = 0;
	int i = 0;
	while( i < playerHand.size() && (nReyes < 4 || nCaballos < 4)){
		if(playerHand[i].number == 8)
			nCaballos++;
		else if(playerHand[i].number == 9)
			nReyes++;
		i++;
	}
	
	return (nReyes == 4 || nCaballos == 4);
}

uint8_t TuteLAN_Server::roundWinner()
{
	Card winCard = round_cards[0].first;
	uint8_t winner = round_cards[0].second;
	int count = CARD_VALUES.at(round_cards[0].first.number).second;
	for(int i = 1; i < round_cards.size(); i++){
		if( winCard.suit == round_cards[i].first.suit) {
			if(CARD_VALUES.at(winCard.number).first < CARD_VALUES.at(round_cards[i].first.number).first){
				winCard = round_cards[i].first;
				winner = round_cards[i].second;
			}
		}
		else if(round_cards[i].first.suit == pinta){
			winCard = round_cards[i].first;
			winner = round_cards[i].second;
		}
		count += CARD_VALUES.at(round_cards[i].first.number).second;
	}
	if(winner % 2 == 0)
		team1.gamePoints += count;
	else
		team2.gamePoints += count;

	round_cards.clear();
	return winner;
}

void TuteLAN_Server::gameWinner()
{
	uint8_t addedPoints = 1;
	if(team1.gamePoints  > 100 || team2.gamePoints > 100)
		addedPoints = 2;
	if(team1.gamePoints > team2.gamePoints ){
		team1_points += addedPoints;
	}
	else if (team1.gamePoints  < team2.gamePoints ){
		team2_points += addedPoints;
	}
	else{
		if(mano%2 == 0)
			team1_points += addedPoints;
		else
			team2_points += addedPoints;
	}
}



