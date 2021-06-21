#pragma once
#include <unistd.h>
#include <string.h>
#include <memory>
class Card;
bool operator== (const Card &s1, const Card &s2);
//una carta consta de su numero (del 0 al 9)
//y del palo, 0 = oros, 1 = copas, 2 = espadas, 3 = bastos
struct Card {
	uint8_t number;
	uint8_t suit;
	//operador de igualdad entre dos cartas, debe coincidir el numero y palo
	friend bool operator== (const Card &s1, const Card &s2){
		return  s1.number == s2.number && s1.suit == s2.suit ;
	}

};