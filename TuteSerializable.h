//Texture.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include <memory>
#include <vector>
#include <iostream>

#include "Serializable.h"

enum TuteType : uint8_t { 
    PRUEBA,
    ILEGAL_MOVE,
    TURN,
    CANTE,
    CANTE_TUTE,
    ROUND_WINNER,
    GAME_WINNER,
    CARD,
    HAND
};
// class TuteBase: public Serializable{
// public: 
//     TuteBase() : type(0){}
//     TuteBase(uint8_t _type) : type(_type){}
//     virtual ~TuteBase(){}

//     virtual void to_bin()=0;//{std::cout<<"to_bin de tuteBase\n";};
//     virtual int from_bin(char * bobj)=0;//{std::cout<<"from_bin de tuteBase\n";return 0;};

//     uint8_t getType() {return type;}
// protected:
//     uint8_t type;
// };


class TuteObject{
public:
    TuteObject(){

    }
    virtual ~TuteObject(){}
    virtual uint8_t getSize()=0;
    virtual char* serialize()=0;
    virtual int deserialize(char* bojb)=0;
};

class TuteMSG: public  Serializable{
public:  
    size_t MSG_SIZE =  sizeof(uint8_t) + object.getSize();

    TuteMSG();
    TuteMSG( uint8_t _type, uint8_t _cnt);
    virtual ~TuteMSG(){}

    void to_bin() override;

    int from_bin(char * bobj) override;
    uint8_t getType() const { return type;}

private:
    uint8_t type;
    TuteObject object;
};

class TuteCante: public  TuteObject{
public:  
    static const size_t CANTE_SIZE = 3 * sizeof(uint8_t);

    TuteCante();
    TuteCante( uint8_t _type, uint8_t _cnt, uint8_t _player);
    virtual ~TuteCante(){}

    char* serialize() override;

    int deserialize(char * bobj) override;
    uint8_t getType() override;

    uint8_t getSuit() const { return content;}
    uint8_t getPlayer() const { return player;}

private:
    uint8_t content;
    uint8_t player;
};

class Card;
bool operator== (const Card &s1, const Card &s2);

class Card: public  TuteObject{
public:  
    static const size_t CARD_SIZE = 3 * sizeof(uint8_t);

    Card();
    Card(  uint8_t number, uint8_t suit);
    virtual ~Card(){}

    char* serialize() override;

    int deserialize(char * bobj) override;
    uint8_t getType() override;

    uint8_t getSuit() const { return suit;}
    uint8_t getNumber() const { return number;}

    friend bool operator== (const Card &s1, const Card &s2);

private:
    uint8_t suit;//0 oros, 1 copas, 2 espadas, 3 bastos
    uint8_t number; 
    
};


class Hand: public  TuteObject{
public:  
    static const size_t HAND_SIZE = 10 * (Card::CARD_SIZE) + 2 * sizeof(uint8_t) + 8 * sizeof(char);

    Hand(const std::vector<Card> &hand_, uint8_t client_ID_,const std::string &nick_);
    virtual ~Hand(){}

    char* serialize() override;

    int deserialize(char * bobj) override;  

    uint8_t getType() override;

    std::vector<Card> &getHand(){ return hand;}      
    uint8_t getClient_ID(){ return client_ID;}
    const std::string &getNick(){ return nick;}

private:
    std::vector<Card> hand;
    uint8_t client_ID;
    std::string nick;
};