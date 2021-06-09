#pragma once

#include <vector>
#include "SDLGame.h"

class TuteLAN {

public:
	TuteLAN();
	virtual ~TuteLAN();

	void start();
	void stop();

private:
	void initGame();
	void closeGame();
	void initFactories();

	SDLGame *game_;
	bool exit_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
