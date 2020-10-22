#include "Ship.h"
#include "Game.h"

Ship::Ship(Game* game) : Actor(game), shipSprite(this), shipMovement(this) {
	gameReal = game;
	SetPosition(Vector2(512, 384));
	stasis = game->GetTexture("Assets/Ship.png");
	thrust = game->GetTexture("Assets/ShipThrust.png");
	shipSprite.SetTexture(stasis);
	screenw = 1024;
	screenh = 768;
}

Ship::~Ship() {

}

void Ship::OnProcessInput(const Uint8* keyState) {

	//if rotating block: if left, if right, if neither/both
	//if moving block: if forward, if back, if neither/both
	//if other block: if no movement
	keyState = SDL_GetKeyboardState(NULL);
	//Moving
	if (keyState[SDL_SCANCODE_UP]) {
		shipSprite.SetTexture(thrust);
		shipMovement.SetForwardSpeed(100);
	}
	else if (keyState[SDL_SCANCODE_DOWN]) {
		shipSprite.SetTexture(thrust);
		shipMovement.SetForwardSpeed(-100);
	}
	if ((!keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN]) || (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN])) {
		shipSprite.SetTexture(stasis);
		shipMovement.SetForwardSpeed(0);
	}
	//Rotating
	if (keyState[SDL_SCANCODE_RIGHT]) {
		shipSprite.SetTexture(thrust);
		shipMovement.SetAngularSpeed(-3);
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		shipSprite.SetTexture(thrust);
		shipMovement.SetAngularSpeed(3);
	}
	if ((!keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT]) || (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_LEFT])) {
		if ((!keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN]) || (keyState[SDL_SCANCODE_UP] && keyState[SDL_SCANCODE_DOWN])) {
			shipSprite.SetTexture(stasis);
		}
		shipMovement.SetAngularSpeed(0);
	}
	if (keyState[SDL_SCANCODE_SPACE] && duration <= 0.0) {
		duration = 1.0;
		Laser* shipLaser = new Laser(gameReal);
		shipLaser->SetPosition(mPosition);
		shipLaser->SetRotation(mRotation);
	}
}

void Ship::OnUpdate(float deltaTime) {
	duration -= deltaTime;
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