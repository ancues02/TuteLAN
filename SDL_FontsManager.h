#pragma once

#include "Font.h"
#include <map>

// Clase para gestionar las fuentes
class SDL_FontsManager {
public:
	SDL_FontsManager();
	virtual ~SDL_FontsManager();

	bool init();

	Font* getFont(std::size_t tag);
	bool loadFont(std::size_t tag, const string& fileName, int size);
private:
	map<int, Font*> fonts_;
	bool initialized_;
};

