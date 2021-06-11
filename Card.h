//Texture.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <vector>

#include "Serializable.h"

enum TuteType : uint8_t { 
    ILEGAL_MOVE,
    TURN,
    CANTE_VEINTE,
    CANTE_CUARENTA,
    CANTE_TUTE,
    ROUND_WINNER,
    GAME_WINNER,
    CARD,
    HAND
};
class TuteBase: public Serializable{
public: 
    TuteBase() : type(0){}
    TuteBase(uint8_t _type) : type(_type){}
    virtual ~TuteBase(){}

    void to_bin(){};
    int from_bin(char * bobj){return 0;};

    uint8_t getType() {return type;}
protected:
    uint8_t type;
};

class TuteMSG: public  TuteBase{
public:  
    static const size_t MSG_SIZE = 2 * sizeof(uint8_t);

    TuteMSG();
    TuteMSG( uint8_t _type, uint8_t _cnt);
    virtual ~TuteMSG(){}

    void to_bin();

    int from_bin(char * bobj);

private:
    uint8_t content;
};

class TuteCante: public  TuteBase{
public:  
    static const size_t CANTE_SIZE = 3 * sizeof(uint8_t);

    TuteCante();
    TuteCante( uint8_t _type, uint8_t _cnt, uint8_t _player);
    virtual ~TuteCante(){}

    void to_bin();

    int from_bin(char * bobj);

private:
    uint8_t content;
    uint8_t player;
};

class Card;
bool operator== (const Card &s1, const Card &s2);

class Card: public  TuteBase{
public:  
    static const size_t CARD_SIZE = 3 * sizeof(uint8_t);

    Card();
    Card(  uint8_t number, uint8_t suit);
    virtual ~Card(){}

    void to_bin();

    int from_bin(char * bobj);
    
    uint8_t getSuit() const { return suit;}
    uint8_t getNumber() const { return number;}

    friend bool operator== (const Card &s1, const Card &s2);

private:
    uint8_t suit;//0 oros, 1 copas, 2 espadas, 3 bastos
    uint8_t number; 
    //uint8_t client_ID;

};


class Hand: public  TuteBase{
public:  
    static const size_t HAND_SIZE = 10 * (Card::CARD_SIZE) + 2 * sizeof(uint8_t) + 8 * sizeof(char);

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