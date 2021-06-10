//Texture.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <vector>

#include "Serializable.h"

class Card: public  Serializable{
public:  
    static const size_t CARD_SIZE = 2 * sizeof(uint8_t);

    Card(  uint8_t number, uint8_t suit);
    virtual ~Card(){}

    void to_bin();

    int from_bin(char * bobj);
    
    uint8_t getSuit(){ return suit;}
    uint8_t getNumber(){ return number;}
    

private:
    uint8_t suit;//0 oros, 1 copas, 2 espadas, 3 bastos
    uint8_t number; 
    
};


class Hand: public  Serializable{
public:  
    static const size_t HAND_SIZE = 10 * (Card::CARD_SIZE);

    Hand(const std::vector<Card> &hand_, uint8_t client_ID_,const std::string &nick_);
    virtual ~Hand(){}

    void to_bin();

    int from_bin(char * bobj);  

    std::vector<Card> &getHand(){ return hand;}      
    uint8_t getClient_ID(){ return client_ID;}
    const std::string &getNick(){ return nick;}

private:
    std::vector<Card> hand;
    uint8_t client_ID;
    std::string nick;
};