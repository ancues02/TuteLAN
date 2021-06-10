#include "Texture.h"
#include <SDL2/SDL_image.h>

#include <iostream>

using namespace std;

Texture::Texture() :
		texture_(nullptr), renderer_(nullptr), width_(0), height_(0) {
}

Texture::Texture(SDL_Renderer *renderer, const string& fileName) :
		texture_(nullptr), width_(0), height_(0) {
	loadFromImg(renderer, fileName);
}

Texture::~Texture() {
	close();
}

void Texture::close() {
	if (texture_ != nullptr) {
		SDL_DestroyTexture(texture_); // delete current texture
		texture_ = nullptr;
		width_ = 0;
		height_ = 0;
	}
}

bool Texture::loadFromImg(SDL_Renderer *renderer, const string& fileName) {
	SDL_Surface *surface = IMG_Load(fileName.c_str());
	if (surface != nullptr) {
		close(); // destroy current texture
		texture_ = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture_ != nullptr) {
			width_ = surface->w;
			height_ = surface->h;
		}
		SDL_FreeSurface(surface);
	} else {
		throw "Couldn't load image: " + fileName;
	}
	renderer_ = renderer;
	return texture_ != nullptr;
}

void Texture::render(int x, int y) const {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = width_;
	dest.h = height_;
	render(dest);
}

void Texture::render(const SDL_Rect &dest, const SDL_Rect& clip) const {
	if (texture_) {
		SDL_RenderCopy(renderer_, texture_, &clip, &dest);
	}
}

void Texture::render(const SDL_Rect &dest) const {
	SDL_Rect clip = {0, 0, width_, height_ };
	render(dest, clip);
}


void Texture::render(const SDL_Rect &dest, double angle,
		const SDL_Rect &clip) const {
	if (texture_) {
		SDL_RenderCopyEx(renderer_, texture_, &clip, &dest, angle, nullptr,
				SDL_FLIP_NONE);
	}
}

void Texture::render(const SDL_Rect &dest, double angle) const {
	SDL_Rect clip = {0, 0, width_, height_ };
	render(dest, angle, clip);
}