#include "SDL_TexturesManager.h"
#include <SDL2/SDL_image.h>
#include <assert.h>

SDL_TexturesManager::SDL_TexturesManager() :
		initialized_(false) {
}

SDL_TexturesManager::~SDL_TexturesManager() {

	if (!initialized_)
		return;

	for (const auto &pair : textures_) {
		if (pair.second != nullptr)
			delete pair.second;
	}

	IMG_Quit();

}

bool SDL_TexturesManager::init() {
	if (initialized_)
		return false;

	// SDL Image
	int imgInit_ret = IMG_Init(
			IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
	assert(imgInit_ret != 0);

	initialized_ = true;

	return true;
}

bool SDL_TexturesManager::loadFromImg(std::size_t tag,
		SDL_Renderer *renderer, const string &fileName) {

	if (!initialized_)
		return false;

	Texture *texture = new Texture(renderer, fileName);
	if (texture->isReady()) {
		storeTexture(tag, texture);
		return true;
	}

	return false;
}

void SDL_TexturesManager::storeTexture(std::size_t tag, Texture *texture) {
	Texture *curr = textures_[tag];
	if (curr != nullptr)
		delete curr;
	textures_[tag] = texture;
}