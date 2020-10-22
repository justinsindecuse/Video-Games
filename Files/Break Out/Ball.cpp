#include "Ball.h"
#include "Game.h"

Ball::Ball(Game* game) : Actor(game) {
	BallTexture = new SpriteComponent(this);
	BallMove1 = new BallMove(this);
	BallTexture->SetTexture(game->GetTexture("Assets/Ball.png"));
	BallCol = new CollisionComponent(this);
	BallCol->SetSize(20.0f, 20.0f);
	thisGame = game;
}

Ball::~Ball() {

}