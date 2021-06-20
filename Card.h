#pragma once
#include <unistd.h>
#include <string.h>
#include <memory>
class Card;
bool operator== (const Card &s1, const Card &s2);

struct Card {
	uint8_t number;
	uint8_t suit;

	friend bool operator== (const Card &s1, const Card &s2){
		return  s1.number == s2.number &&s1.suit == s2.suit ;
	}

};