#pragma once

#include "Texture.h"
#include <map>
#include <string>

// Clase para gestionar las texturas
class SDL_TexturesManager {
public:
	SDL_TexturesManager();
	virtual ~SDL_TexturesManager();

	bool init();

	Texture* getTexture(std::size_t tag) {
		return textures_[tag];
	}

	bool loadFromImg(std::size_t, SDL_Renderer *renderer,
			const std::string &fileName);

private:
	void storeTexture(std::size_t tag, Texture *texture);

	bool initialized_;
	std::map<std::size_t, Texture*> textures_;

};
