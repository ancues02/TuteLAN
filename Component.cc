#include "Component.h"

Component::Component(ecs::CmpId id): entity_(nullptr), game_(nullptr), id_(id) {
}
Component::~Component() {
}

void Component::setEntity(Entity* entity) {
    entity_ = entity;
}

void Component::setGame(SDLGame* game) {
    game_ = game;
}

void Component::setId(ecs::CmpId id) {
    id_ = id;
}

ecs::CmpId Component::getId() const {
    return id_;
}

// void Component::init() {
// }
// // virtual void update() {
// // }
// void Component::render() {
// }