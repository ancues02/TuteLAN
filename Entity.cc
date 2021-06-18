#include "Entity.h"

Entity::Entity(SDLGame *game, EntityManager* mngr) : game_(game), mngr_(mngr) {
}

Entity::~Entity() {
}

EntityManager* Entity::getEntityMngr() {
    return mngr_;
}



bool Entity::hasComponent(ecs::CmpIdType id) {
    return componentsArray_[id] != nullptr;
}

// void update() {
// 	for (auto &c : components_) {
// 		c->update();
// 	}
// }

void Entity::render() {
    for (auto &c : components_) {
        c->render();
    }
}                       