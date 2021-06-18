#pragma once

#include "ecs.h"
#include "SDLGame.h"
//#include "checkML.h"

class Entity;

class Component {
protected:
	Entity* entity_;
	SDLGame* game_;
	ecs::CmpId id_;
public:
	Component(ecs::CmpId id);
	virtual ~Component();

	void setEntity(Entity* entity);

	void setGame(SDLGame* game);

	void setId(ecs::CmpId id);

	ecs::CmpId getId() const;

	virtual void init(){}
	// virtual void update() {
	// }
	virtual void render(){}

};

