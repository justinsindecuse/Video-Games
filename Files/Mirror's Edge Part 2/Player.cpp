#include "Player.h"
#include "Game.h"
#include "Renderer.h"

Player::Player(Game* game, Actor* parent) : Actor(game, parent) {
	pm = new PlayerMove(this);
	cm = new CameraComponent(this);
	mGame->playerCam = cm;
	playColl = new CollisionComponent(this);
	playColl->SetSize(50.0f, 175.0f, 50.0f);
	mGame->playerCol = playColl;
	mGame->playerMove = pm;
}