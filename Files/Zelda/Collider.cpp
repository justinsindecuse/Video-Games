#include "Collider.h"
#include "Game.h"
#include "CollisionComponent.h"

Collider::Collider(Game* game) : Actor(game) {
	col = new CollisionComponent(this);
}

Collider::~Collider() {

}

void Collider::setCollision(int width, int height) {
	col->SetSize(width, height);
}