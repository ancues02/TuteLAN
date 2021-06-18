#pragma once

#include <iostream>
#include <array>
#include "Component.h"
#include "EntityManager.h"
#include "SDLGame.h"

class EntityManager;

class Entity {
public:
	Entity(SDLGame *game, EntityManager* mngr);
	virtual ~Entity();

	EntityManager* getEntityMngr();

	template<typename T, typename ... TArgs>
    T* addComponent(TArgs&& ...mArgs) {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        std::unique_ptr<Component> uPtr(c);
        components_.push_back(std::move(uPtr));
        componentsArray_[c->getId()] = c;
        c->setEntity(this);
        c->setGame(game_);
        c->init();
        return c;
    }                       

    template<typename T>
    T* getComponent(ecs::CmpId id) {
        return static_cast<T*>(componentsArray_[id]);
    }

	bool hasComponent(ecs::CmpId id);

	// void update() {
	// 	for (auto &c : components_) {
	// 		c->update();
	// 	}
	// }

	void render();
private:
	SDLGame *game_;
	EntityManager* mngr_;

	std::vector<unique_ptr<Component>> components_;
	std::array<Component*,ecs::_TotalComps_> componentsArray_ = {};
};

