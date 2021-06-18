#include "TuteSerializable.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

TuteMSG::TuteMSG(): TuteBase(0), content(-1) {
} 

TuteMSG::TuteMSG(uint8_t _type, uint8_t _cnt): TuteBase(_type), content(_cnt) {
} 

void TuteMSG::to_bin()
{
    alloc_data(MSG_SIZE);

    memset(_data, 0, MSG_SIZE);

    char* tmp=_data;

    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &content, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
}

int TuteMSG::from_bin(char * bobj)
{
    alloc_data(MSG_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MSG_SIZE);

    char* tmp = _data;

    memcpy(&type,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&content,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    return 0;
}

TuteCante::TuteCante(): TuteBase(0), content(-1) {
} 

TuteCante::TuteCante(uint8_t _type, uint8_t _cnt, uint8_t _player): TuteBase(_type), content(_cnt), player(_player) {
} 

void TuteCante::to_bin()
{
    alloc_data(CANTE_SIZE);

    memset(_data, 0, CANTE_SIZE);

    char* tmp=_data;

    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &content, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &player, sizeof(uint8_t));

}

int TuteCante::from_bin(char * bobj)
{
    alloc_data(CANTE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, CANTE_SIZE);

    char* tmp = _data;

    memcpy(&type,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&content,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&player,tmp, sizeof(uint8_t));

    return 0;
}

Card::Card(): TuteBase(TuteType::CARD), suit(-1) , number(-1) {} 

Card::Card( uint8_t number_, uint8_t suit_): TuteBase(TuteType::CARD), suit(suit_) , number(number_) {} 

void Card::to_bin()
{
    std::cout << "\nSerializar Carta\n";
    alloc_data(CARD_SIZE);

    memset(_data, 0, CARD_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp=_data;

    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    std::cout << "type: "<< (int)type << "\n";

    memcpy(tmp, &suit, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    std::cout << "suit: "<< (int)suit << "\n";

    memcpy(tmp, &number, sizeof(uint8_t));
    tmp += sizeof(uint8_t);  
    std::cout << "number: "<< (int)number << "\n";

    //memcpy(tmp, &client_ID, sizeof(uint8_t));

}

int Card::from_bin(char * bobj)
{
    std::cout << "\nDeserializar Carta\n";

    alloc_data(CARD_SIZE);

    memcpy(static_cast<void *>(_data), bobj, CARD_SIZE);

    char* tmp = _data;

    memcpy(&type,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);
    std::cout << "type: "<< (int)type << "\n";

    memcpy(&suit,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);
    std::cout << "suit: "<< (int)suit << "\n";

    memcpy(&number,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);     
    std::cout << "number: "<< (int)number << "\n";

    //memcpy(&client_ID,tmp, sizeof(uint8_t));

    return 0;
}

bool operator== (const Card &s1, const Card &s2){
    return s1.number == s2.number && s1.suit == s2.suit;
}

Hand::Hand(const std::vector<Card> &hand_, uint8_t client_ID_,const std::string &nick_): TuteBase(TuteType::HAND), hand(hand_) , client_ID(client_ID_), nick(nick_) {} 

//serializar la mano
void Hand::to_bin()
{
        std::cout << "\nSerializar Hand\n";

    alloc_data(HAND_SIZE);

    memset(_data, 0, HAND_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp=_data;

    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    std::cout << "type: "<< (int)type << "\n";
    memcpy(tmp, &client_ID, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    std::cout << "client_ID: "<< (int)client_ID << "\n";


    memcpy(tmp, nick.c_str(), 8 * sizeof(char));
    tmp += 8 * sizeof(char);
    std::cout << "nick: "<< nick << "\n";


    for(int i=0; i< hand.size();++i){
        hand[i].to_bin();
        memcpy(tmp, hand[i].data(), hand[i].size());
        tmp +=  hand[i].size();
        
    }
}

//deserializar la mano
int Hand::from_bin(char * bobj)
{
    std::cout << "\nDeserializar Hand\n";

    alloc_data(HAND_SIZE);

    memcpy(static_cast<void *>(_data), bobj, HAND_SIZE);

    char* tmp = _data;
        
    memcpy(&type,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);
    std::cout << "type: "<< (int)type << "\n";

    memcpy(&client_ID,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);
    std::cout << "client_ID: "<< (int)client_ID << "\n";

    tmp[7 * sizeof(char)] = '\0';//capamos el nombre a 8 letras
    nick=tmp;
    tmp += 8 * sizeof(char);
    std::cout << "nick: "<< nick << "\n";

    //las cartas
    for(int i=0; i< hand.size();++i){
        hand[i].from_bin(tmp);       
        tmp +=  hand[i].size();
    }

    return 0;
}
