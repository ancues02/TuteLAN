#pragma once

class Card;
bool operator== (const Card &s1, const Card &s2);

struct Card {
	uint8_t suit;
	uint8_t number;

	friend bool operator== (const Card &s1, const Card &s2){
		s1.suit == s2.suit && s1.number == s2.number;
	}

};