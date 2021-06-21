#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>


class Resources {
public:

	enum TextureId : std::size_t {
		// images
		Deck,
		Turn,
	};

	enum FontId : std::size_t {
		// fonts
		ARIAL16, ARIAL24,
	};

	struct FontInfo {
		FontId id;
		std::string fileName;
		int size;
	};

	struct ImageInfo {
		TextureId id;
		std::string fileName;
	};


	static std::vector<FontInfo> fonts_; // initialized in .cpp
	static std::vector<ImageInfo> images_; // initialized in .cpp
	

};
