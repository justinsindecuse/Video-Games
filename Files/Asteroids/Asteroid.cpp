#include "Asteroid.h"
#include "Game.h"

Asteroid::Asteroid(Game* game) : Actor(game), asteroidSprite(this), asteroidMovement(this) {
	stasis = game->GetTexture("Assets/Asteroid.png");
	asteroidSprite.SetTexture(stasis);
	asteroidMovement.SetForwardSpeed(150.0f);
	mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
	mPosition = Random::GetVector(Vector2(0, 0), Vector2(screenw, screenh));
	screenw = 1024;
	screenh = 768;
}

Asteroid::~Asteroid() {

}

void Asteroid::OnUpdate(float deltaTime) {
	if (mPosition.x < 0) {
		mPosition.x = screenw;
	}
	if (mPosition.y < 0) {
		mPosition.y = screenh;
	}
	if (mPosition.x > screenw) {
		mPosition.x = 0;
	}
	if (mPosition.y > screenh) {
		mPosition.y = 0;
	}
}