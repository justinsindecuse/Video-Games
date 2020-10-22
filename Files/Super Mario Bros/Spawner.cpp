#include "Spawner.h"
#include "Goomba.h"
#include "Game.h"
#include "Player.h"

Spawner::Spawner(Game* game) : Actor(game) {

}

Spawner::~Spawner() {

}

void Spawner::OnUpdate(float deltaTime) {
	if (this->GetPosition().x - mGame->player1->GetPosition().x < 600.0f) {
		Goomba* temp = new Goomba(mGame);
		temp->SetPosition(this->GetPosition());
		this->SetState(ActorState::Destroy);
	}
}