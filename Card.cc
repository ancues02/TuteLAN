#include "Card.h"


Card::Card( uint8_t number_, uint8_t suit_):suit(suit_) , number(number_){} 

void Card::to_bin()
{
    alloc_data(CARD_SIZE);

    memset(_data, 0, CARD_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp=_data;

    memcpy(tmp, &suit, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &number, sizeof(uint8_t));
    tmp += sizeof(uint8_t);  

}

int Card::from_bin(char * bobj)
{
    alloc_data(CARD_SIZE);

    memcpy(static_cast<void *>(_data), bobj, CARD_SIZE);

    char* tmp = _data;

    memcpy(&suit,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    memcpy(&number,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);     

    return 0;
}


Hand::Hand(const std::vector<Card> &hand_, uint8_t client_ID_,const std::string &nick_):hand(hand_) , client_ID(client_ID_), nick(nick_){} 

//serializar la mano
void Hand::to_bin()
{
    alloc_data(HAND_SIZE);

    memset(_data, 0, HAND_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp=_data;

    memcpy(tmp, &client_ID, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, nick.c_str(), 8 * sizeof(char));
    tmp += 8 * sizeof(char);

    for(int i=0; i< hand.size();++i){
        hand[i].to_bin();
        memcpy(tmp, hand[i].data(), hand[i].size());
        tmp +=  hand[i].size();
    }
}

//deserializar la mano
int Hand::from_bin(char * bobj)
{
    alloc_data(HAND_SIZE);

    memcpy(static_cast<void *>(_data), bobj, HAND_SIZE);

    char* tmp = _data;

    memcpy(&client_ID,tmp, sizeof(uint8_t));
    tmp+=sizeof(uint8_t);

    tmp[7 * sizeof(char)] = '\0';//capamos el nombre a 8 letras
    nick=tmp;
    tmp += 8 * sizeof(char);

    //las cartas
    for(int i=0; i< hand.size();++i){
        hand[i].from_bin(tmp);       
        tmp +=  hand[i].size();
    }

    return 0;
}
