#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager(SDLGame *game) :
		game_(game) {
}

EntityManager::~EntityManager() {
}

// void EntityManager::update() {
// 	for (auto &e : entities) {
// 		e->update();
// 	}
// }

void EntityManager::render() {
	for (auto &e : entities) {
		e->render();
	}
}

Entity* EntityManager::addEntity() {
	Entity* e = new Entity(game_,this);
	std::unique_ptr<Entity> uPtr( e );
	entities.emplace_back(std::move(uPtr));
	return e;
}
