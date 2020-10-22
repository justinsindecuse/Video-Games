#include "Paddle.h"
#include "Game.h"

Paddle::Paddle(Game* game) : Actor(game) {
	thisGame = game;
	PaddleTexture = new SpriteComponent(this);
	PaddleTexture->SetTexture(game->GetTexture("Assets/Paddle.png"));
	PaddleMove = new MoveComponent(this);
	PaddleCol = new CollisionComponent(this);
	PaddleCol->SetSize(104.0f, 24.0f);
	limitLeft = 84.0f;
	limitRight = 940.0f;
}

Paddle::~Paddle() {

}

void Paddle::OnProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_RIGHT]) {
		PaddleMove->SetForwardSpeed(300.0f);
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		PaddleMove->SetForwardSpeed(-300.0f);
	}
	if((!keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT]) || (keyState[SDL_SCANCODE_RIGHT] && keyState[SDL_SCANCODE_LEFT])){
		PaddleMove->SetForwardSpeed(0.0f);
	}
}

void Paddle::OnUpdate(float deltaTime) {
	if (mPosition.x < limitLeft) {
		mPosition.x = limitLeft;
	}
	if (mPosition.x > limitRight) {
		mPosition.x = limitRight;
	}
}