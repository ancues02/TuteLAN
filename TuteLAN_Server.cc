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
	 * 	Espera a que se connecten los 4 jugadores y entonces
	 * 	lanza el juego
	 */
	std::cout << "Servidor iniciado y esperando " << MAX_CLIENTS << " clientes\n";

	if(socket.listen(MAX_CLIENTS) == -1){
		std::cerr << "[listen]: uso de listen\n";
		return ;
	}
    for(int i = 0; i < MAX_CLIENTS; i++){
		available_IDs.push(i);
	}

	std::cout << "IDs: " << available_IDs.size() << "\n";
	wait_players();
}

void TuteLAN_Server::wait_players(){
	Socket* client;
	//int players;
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    struct sockaddr cliente;
    socklen_t longCliente=sizeof(sockaddr);
    while(clients.size() < MAX_CLIENTS){
        int cliente_socket=socket.accept(&cliente, &longCliente);

        if(cliente_socket==-1){
            std::cerr << "[accept] Error en accept\n";
            continue;
        }   
        getnameinfo(&cliente, longCliente, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICSERV || NI_NUMERICHOST);
		std::cout << "Conexion desde " << host << " " << serv << " Nplayers : " << clients.size() << "\n"; 
        
        
		client=new Socket(cliente_socket,&cliente,longCliente);
		clients.push_back({ std::move(std::unique_ptr<Socket>(client)), available_IDs.front() } );

		std::cout << "Pusheado socket, Nclients = " << clients.size() << "\n";
    	TuteMSG msg1;
		if(client->recv(msg1) < 0){
			std::cout << "SERVER: Error recibiendo mensaje\n";
			delete client;
			clients.pop_back();
		}
		else {
			player_nicks[available_IDs.front()] = msg1.getNick();			
			TuteMSG msg = TuteMSG(player_nicks[available_IDs.front()],TuteType::LOGIN, available_IDs.front(), 0);
			client->send(msg);
			//players++;
			available_IDs.pop();

			// Thread 
			TuteLAN_Server* serv = this;
			std::thread c_th = std::thread([client, serv](){
				bool _exit = false;
				while(!_exit){
					TuteMSG received;
					if(client->recv(received) < 0){
						std::cout << "SERVER: Error recibiendo mensaje\n";
						continue;
					}
					std::cout << "SERVER: Mensaje Recibido\n";
					
					serv->handle_message(received, _exit);

					std::cout << "SERVER: Mensaje Procesado\n";
				}
			});
			c_th.detach();
		}
    }

	disconnection = false;
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
	roundCount = turnCount = 0;

	while(!disconnection && team1_points <= POINTS_TO_WIN && team2_points <= POINTS_TO_WIN){	// o termina una partida ?
		
		// Principo de juego
		distributeCards();
		mano %= MAX_CLIENTS;
		turn = (mano + 1) % MAX_CLIENTS;

		TuteMSG msg_send = TuteMSG(player_nicks[turn], TuteType::TURN, turn, 0);		
		broadcast_message(msg_send);

		while(roundCount < 10 && !disconnection){
			int no = 0;
		}	// wait ( signal de algun thread )
		//m.lock();
		if(!disconnection) {
			gameWinner();
			mano++;
			roundCount = 0;
		}

		//m.unlock();
		//team1_points=6;
		
	}
	if(disconnection){
		TuteMSG msg = TuteMSG("\0", TuteType::WAIT, 0, 0);
		broadcast_message(msg);
		wait_players();
	}
	else{
		endGame();
	}
	// Mandar mensaje a todos los jugadores con el equipo ganador
	// Esperar y cerrar conexiones
}
void TuteLAN_Server::endGame(){
	int winner=0;
	if(team1_points < team2_points)
		winner=1;
	TuteMSG msg = TuteMSG("\0", TuteType::TUTE_WINNER, winner, 0);
	broadcast_message(msg);
	while(clients.size() > 0){
		//esperar :D
	}
	wait_players();

}
void TuteLAN_Server::broadcast_message(TuteMSG& msg){
	std::cout << "Broadcast de: " << (int)msg.getType() << "\n";             
	for(int i = 0; i < clients.size(); i++){	
		clients[i].first.get()->send(msg);		       
	}
	sleep(1);
}

void TuteLAN_Server::handle_message(TuteMSG& received, bool& _exit){
	switch (received.getType())
	{
	case TuteType::CARD:
	{
		Card card = { received.getInfo_1(), received.getInfo_2()};
		if(legalCard(card)){
			std::cout<<"Han jugado la carta "<< (int)card.number << " de " << (int)card.suit<<"\n";
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
			TuteMSG msg_send = TuteMSG(player_nicks[turn], TuteType::CARD, card.number, card.suit);
			broadcast_message(msg_send);
			//m.lock();
			
			// Aumentamos turno
			turnCount++;

			if(turnCount == 4){
				// Decidir que equipo gana la ronda
				turn = roundWinner();
				roundCount++;
				turnCount = 0;
			}
			else{
				// Actualizamos el siguiente jugador que le toca 
				turn = (turn + 1) % MAX_CLIENTS;
			}

			//m.unlock();

			msg_send = TuteMSG(player_nicks[turn], TuteType::TURN, turn, 0);		
			broadcast_message(msg_send);				
		}
		// Si la carta no es legal tiene que volver a elegir
		else{
			TuteMSG msg_send = TuteMSG(player_nicks[turn], TuteType::ILEGAL_MOVE, 0,0);
			clients[turn].first.get()->send(msg_send);
		}
		break;
	}

	case TuteType::CANTE:
	{
		std::cout << "intentan cantar\n";                    

		int points = 20;
		// el contenido en estos mensajes es el ID del jugador
		if(legalCante(received)){
			// Comprobamos si canta 40
			if(received.getInfo_2() == pinta)
				points = 40;
			if(turn % 2 == 0){
				team1.gamePoints += points;
				team1.cantes[received.getInfo_2()] = true;									
			}				
			else{
				team2.gamePoints += points;		
				team2.cantes[received.getInfo_2()] = true;		
			}
			//mandar mensaje a todos de quien ha cantado y en que palo
			TuteMSG msg_send = TuteMSG(player_nicks[received.getInfo_1()], TuteType::CANTE, received.getInfo_1(), received.getInfo_2());								
			broadcast_message(msg_send);								
		}
		else{
			TuteMSG msg_send = TuteMSG(player_nicks[received.getInfo_1()], TuteType::ILEGAL_MOVE, 0,0);
			clients[received.getInfo_1()].first.get()->send(msg_send);
		}
		break;
	}

	case TuteType::CANTE_TUTE:
	{
		if(legalCanteTute(received)){
			// team1 wins
			if(turn % 2 == 0){
				team1_points = POINTS_TO_WIN;
			}
			else// team2 wins
			{
				team2_points = POINTS_TO_WIN;
			}
			TuteMSG msg_send = TuteMSG(player_nicks[turn], TuteType::CANTE_TUTE, received.getInfo_1() % 2, received.getInfo_2());								
			broadcast_message(msg_send);
		}
		break;
	}

	case TuteType::DISCONNECT:
	{
		
		std::cout << "Desconexion de: " << (int)received.getInfo_1() << " Clientes " << clients.size() << " IDs " << available_IDs.size() <<"\n";

		auto it = clients.begin();
		while (it != clients.end() && it->second != received.getInfo_1())
		{
			++it;
		}

		if(it !=  clients.end()){
			clients.erase(it);

			available_IDs.push(received.getInfo_1());
			_exit = disconnection = true;

			std::cout << "Desconexion de: " << (int)received.getInfo_1() << " Clientes " << clients.size() << " IDs " << available_IDs.size() <<"\n";
		}

		break;
	}
	}		
	
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
	srand(time(NULL));
	std::random_shuffle(desk.begin(), desk.end());

	//empieza repartiendo al jugador de la derecha del que reparte
	int player=mano+1;

	for (int i = 0; i < 10 * MAX_CLIENTS ; ++i){
		handClients[player%MAX_CLIENTS].push_back(desk[i]);
		player++;		
	}
	//la pinta es la ultima	
	pinta = desk[39].suit;

	sleep(1);
	TuteMSG msg;
	for(int j = 0; j < 10; j++){		
		for(int i = 0; i<clients.size(); ++i){
			msg = TuteMSG(player_nicks[i], TuteType::HAND, handClients[i][j].number,  handClients[i][j].suit);
			clients[i].first.get()->send(msg);
		}	
		sleep(1);	
	}
	msg =TuteMSG("", TuteType::PINTA, desk[39].number, desk[39].suit);
	broadcast_message(msg);

}

// Comprueba si el cliente puede poner la carta
// primero que se usa la carta del palo de la ronda, luego la pinta y luego cualquier otra
bool TuteLAN_Server::legalCard(const Card& card)
{
	//si es la primera carta en la mesa, esa es el palo de la ronda
	if(round_cards.empty()) {
		roundSuit = card.suit;
		return true;
	}

	//si se juega del palo de la ronda bien
	if(card.suit == roundSuit)
		return true;

	//si se juega de la pinta, comprobar que el jugador no tiene cartas del palo de la ronda
	if(card.suit == pinta){
		for(int i = 0; i < handClients[turn].size(); i++){
			if(handClients[turn][i].suit == roundSuit)
				return false;
		}
		return true;
	}
	
	//sino comprobar que no tiene ni pinta ni carta del palo de la ronda	
	for(int i = 0; i < handClients[turn].size(); i++){
		if(handClients[turn][i].suit == roundSuit || handClients[turn][i].suit == pinta)
			return false;
	}
	
	return true;
}

//Analizar si es legal el cante
//el mensaje de cante el info1 es la id del cliente y el info2 el palo
bool TuteLAN_Server::legalCante(const TuteMSG& _cante)
{
	Team* t;
	if(_cante.getInfo_1() % 2 == 0)
		t = &team1;
	else
		t = &team2;

	if(!round_cards.empty() || turn % 2 != _cante.getInfo_1() % 2 || t->cantes[_cante.getInfo_2()]) 
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
// En el mensaje de cante de tute, el info1 es el ID del jugador
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

//ver quien ha ganado la ronda y sumar los puntos de juego
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
	//las 10 de ultimas
	if(turn%2 == 0)
		team1.gamePoints += 10;
	else 
		team2.gamePoints +=10;
	//comprobar quien ha ganado el juego
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
	TuteMSG msg("ganador", TuteType::GAME_WINNER,turn%2,addedPoints);
	broadcast_message(msg);

	
}



