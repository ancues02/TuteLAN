#include "Resources.h"
#include "SDL_macros.h"

std::vector<Resources::FontInfo> Resources::fonts_ {
//
		{ ARIAL16, "Resources/ARIAL.ttf", 16 }, //
		{ ARIAL24, "Resources/ARIAL.ttf", 24 } //
};

std::vector<Resources::ImageInfo> Resources::images_ {
//
		{ Deck, "Resources/Deck.png" }, //
};