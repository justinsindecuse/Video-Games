#include "Player.h"
#include "Game.h"
#include "Renderer.h"

Player::Player(Game* game) : Actor(game) {
	playerCol = new CollisionComponent(this);
	playerCol->SetSize(100, 40, 60);
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
	pm = new PlayerMove(this);
}