#pragma once

#include "Texture.h"
#include <map>
#include <string>

/*
 *
 */
class SDL_TexturesManager {
public:
	SDL_TexturesManager();
	virtual ~SDL_TexturesManager();

	// supposed to be called before start using the object
	bool init();

	Texture* getTexture(std::size_t tag) {
		return textures_[tag];
	}

	bool loadFromImg(std::size_t, SDL_Renderer *renderer,
			const std::string &fileName);
	// bool loadFromText(std::size_t, SDL_Renderer *renderer, const std::string &text,
	// 		const Font *font, const SDL_Color &color) override;
private:
	void storeTexture(std::size_t tag, Texture *texture);

	bool initialized_;
	std::map<std::size_t, Texture*> textures_;

};
