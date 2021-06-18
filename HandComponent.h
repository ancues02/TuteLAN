#pragma once

#include "Component.h"
#include <vector>

class Card;

class HandComponent : public Component
{
public:
    HandComponent();
    //HandComponent(int window_width, int window_height );
    virtual ~HandComponent();
    void init() override;
	void render() override;

    void setHand( const std::vector<Card>& cards);

private:
    std::vector<Card> hand;
    int _WINDOW_WIDTH_ = 640;
	int _WINDOW_HEIGHT_ = 480;
    const int _CARD_WIDTH_ = 20;
    const int _CARD_HEIGHT_ = 40;
    Texture* texture;
};