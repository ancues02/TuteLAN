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
        
        //ThreadTCP* miThread=new ThreadTCP(cliente_socket);
        //std::thread([miThread](){ miThread->do_message(); delete miThread;}).detach();
		client=new Socket(cliente_socket,&cliente,longCliente);
		clients.push_back(std::move(std::unique_ptr<Socket>(client)));   
		players++;     
		std::cout << "Conexion desde " << host << " " << serv << "\n"; 
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

		// Mandar cada mano a su jugador
		for(int i=0; i< MAX_CLIENTS; ++i){
			socket.send(handClients[i], *clients[i].get());
		}

		mano %= MAX_CLIENTS;
		turn = (mano + 1) % MAX_CLIENTS;

		TuteBase received;
		TuteMSG msg;
		Socket* client;
		
		int roundCount = 0;
		//Gestion de rondas (10)
		while(roundCount < 10){
			int turnCount = 0;
			// Gestion de turnos (4)
			while(turnCount < 4){
				// Broadcast del turno  | notificamos el turno al que le toque	
				msg = TuteMSG(TuteType::TURN, turn);	
				socket.send(msg, *clients[turn].get());
				// currTurn(turn);
				// for(auto it = clients.begin(); it != clients.end() ; ++it){			                    
				// }
				
				if(socket.recv(received, client) < 0){
					std::cout << "SERVER: Error recibiendo mensaje\n";
					continue;
				}
				switch (received.getType())
				{
				case TuteType::CARD:
				{
					Card& card = static_cast<Card&>(received);
					if(legalCard(card)){
						// Quitamos la carta del jugador que la puso
						int i = 0;
						while(i < handClients[turn].getHand().size()){ 
							if(handClients[turn].getHand()[i] == card){	
								handClients[turn].getHand()[i] = handClients[turn].getHand()[handClients[turn].getHand().size() - 1];
								handClients[turn].getHand().pop_back();
								break;
							}
							i++; 
						}
						// Poner la carta en 'round_cards'
						round_cards.push_back({ card, turn });

						// Mandar a todos la carta
						for(auto it = clients.begin(); it != clients.end() ; ++it){
							socket.send(card, *it->get());
						}

						// Aumentamos turno
						turnCount++;
						// Actualizamos el siguiente jugador que le toca 
						turn = (turn + 1) % MAX_CLIENTS;
						
					}
					// Si la carta no es legal tiene que volver a elegir
					else{
						msg = TuteMSG(TuteType::ILEGAL_MOVE, -1);	
						socket.send(msg, *clients[turn].get());
					}
					break;
				}

				case TuteType::CANTE:
				{
					TuteCante& cante = static_cast<TuteCante&>(received);
					int points = 20;
					// el contenido en estos mensajes es el ID del jugador
					if(legalCante(cante)){
						// Comprobamos si canta 40
						if(cante.getSuit() == pinta)
							points = 40;
						if(turn % 2 == 0){
							team1.gamePoints += points;
							team1.cantes[cante.getSuit()] = true;		
						}				
						else{
							team2.gamePoints += points;		
							team2.cantes[cante.getSuit()] = true;		
						}					
					}
					break;
				}

				case TuteType::CANTE_TUTE:
				{
					TuteMSG& cante = static_cast<TuteMSG&>(received);
					if(legalCanteTute(cante)){
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

	for (int i = 0; i < 40 ; ++i){
		desk.push_back(Card(i, i%10));
	}
	
	//TODO que el nick sea el correcto xd
	for(int i=0; i< MAX_CLIENTS; ++i){
		handClients.push_back(Hand(std::vector<Card>(), i, "Player"+i));
	}
}

// Barajar y repartir cartas
void TuteLAN_Server::distributeCards()
{
	std::random_shuffle(desk.begin(), desk.end());


	int player=turn;

	for (int i = 0; i < 40 ; ++i){
		handClients[player%MAX_CLIENTS].getHand().push_back(desk[i]);
		player++;		
	}	
	pinta = desk[39].getSuit();

	for(int i = 0; i<clients.size(); ++i){		
		socket.send(handClients[i], *clients[i].get());		
	}
	
}

// Comprueba si el cliente puede poner la carta
bool TuteLAN_Server::legalCard(const Card& card)
{
	if(round_cards.empty()) {
		roundSuit = card.getSuit();
		return true;
	}
	if(card.getSuit() == roundSuit)
		return true;
	
	if(card.getSuit() == pinta){
		for(int i = 0; i < handClients[turn].getHand().size(); i++){
			if(handClients[turn].getHand()[i].getSuit() == roundSuit)
				return false;
		}
		return true;
	}
	else {
		for(int i = 0; i < handClients[turn].getHand().size(); i++){
			if(handClients[turn].getHand()[i].getSuit() == roundSuit || handClients[turn].getHand()[i].getSuit() == pinta)
				return false;
		}
	}
	return true;
}

bool TuteLAN_Server::legalCante(const TuteCante& _cante)
{
	Team t;
	if(_cante.getPlayer() % 2 == 0)
		t = team1;
	else
		t = team2;

	if(round_cards.empty() || turn % 2 != _cante.getPlayer() % 2 || t.cantes[_cante.getSuit()]) 
		return false;
	
	auto playerHand = handClients[_cante.getPlayer()].getHand();
	int puedeCantar = 0;
	int i = 0;
	while( i < playerHand.size() && puedeCantar < 2){
		if(_cante.getSuit() == playerHand[i].getSuit() && playerHand[i].getNumber() > 10)
			puedeCantar++;
		i++;
	}
	
	return puedeCantar == 2;
}
//
// En el mensaje de cante de tute, el contenido es el ID del jugador
bool TuteLAN_Server::legalCanteTute(const TuteMSG& _cante)
{
	if(round_cards.empty() || turn % 2 != _cante.getContent() % 2) {
		return false;
	}
	auto playerHand = handClients[_cante.getContent()].getHand();
	int nReyes = 0,
		nCaballos = 0;
	int i = 0;
	while( i < playerHand.size() && (nReyes < 4 || nCaballos < 4)){
		if(playerHand[i].getNumber() == 11)
			nCaballos++;
		else if(playerHand[i].getNumber() == 12)
			nReyes++;
		i++;
	}
	
	return (nReyes == 4 || nCaballos == 4);
}

uint8_t TuteLAN_Server::roundWinner()
{
	Card winCard = round_cards[0].first;
	uint8_t winner = round_cards[0].second;
	int count = CARD_VALUES.at(round_cards[0].first.getNumber()).second;
	for(int i = 1; i < round_cards.size(); i++){
		if( winCard.getSuit() == round_cards[i].first.getSuit()){
			if(CARD_VALUES.at(winCard.getNumber()).first < CARD_VALUES.at(round_cards[i].first.getNumber()).first){
				winCard = round_cards[i].first;
				winner = round_cards[i].second;
			}
		}
		else if(round_cards[i].first.getSuit() == pinta){
			winCard = round_cards[i].first;
			winner = round_cards[i].second;
		}
		count += CARD_VALUES.at(round_cards[i].first.getNumber()).second;
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



