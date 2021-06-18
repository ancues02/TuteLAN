#include "HandComponent.h"
#include "TuteSerializable.h"
#include "SDL_macros.h"
#include "Entity.h"
#include "SDL2/SDL.h"

HandComponent::HandComponent():
     Component(ecs::HandComponent), hand(std::vector<Card>()), texture(nullptr)
{
}


/*HandComponent::HandComponent(int window_width, int window_height ):
     Component(ecs::HandComponent), texture(nullptr),_WINDOW_WIDTH_(window_width), _WINDOW_HEIGHT_(window_height)
{
}*/

HandComponent::~HandComponent(){

}

void HandComponent::init(){
    texture = game_->getTextureMngr()->getTexture(Resources::Deck);
}

void HandComponent::render() {

    //TO DO:hacer un switch para cada jugador (yo, derecha, delante, izquierda)
    float posCard = (_WINDOW_WIDTH_ / 2 )- (_CARD_WIDTH_ * hand.size() / 2);
    for(int i=0; hand.size(); ++i){
        SDL_Rect rect;
        rect = RECT(posCard + i*_CARD_WIDTH_,_WINDOW_HEIGHT_ - 2 * _CARD_HEIGHT_, _CARD_WIDTH_, _CARD_HEIGHT_);

        texture->render(rect,0, { 102 * hand[i].getSuit(), 67 * hand[i].getNumber() -1 , 67, 102 });
    }
}

void HandComponent::setHand(const std::vector<Card>& cards){
    hand = cards;
}

