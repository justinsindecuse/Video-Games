#include "Laser.h"
#include "Game.h"
#include "math.h"

Laser::Laser(Game* game) : Actor(game), LaserSprite(this), LaserMovement(this) {
	gameReal = game;
	stasis = game->GetTexture("Assets/Laser.png");
	LaserSprite.SetTexture(stasis);
	LaserMovement.SetForwardSpeed(400.0f);
	duration = 1.0;
}

Laser::~Laser() {

}

void Laser::OnUpdate(float deltaTime) {
	duration -= deltaTime;
	if (duration <= 0.0) {
		SetState(ActorState::Destroy);
	}

	//loop through asteroids and detects if they colldie with the laser and deletes them both if they do
	for (unsigned int i = 0; i < gameReal->asteroids.size(); i++) {
		Vector2 resultant = gameReal->asteroids[i]->GetPosition() - mPosition;
		if (std::sqrt(std::pow(resultant.x, 2) + std::pow(resultant.y, 2)) <= 70 ) {
			gameReal->asteroids[i]->SetState(ActorState::Destroy);
			SetState(ActorState::Destroy);
			break;
		}
	}
}