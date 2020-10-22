#include "Player.h"
#include "Game.h"
#include "Renderer.h"

Player::Player(Game* game) : Actor(game) {
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	pm = new PlayerMove(this);
	SetScale(0.75f);
	cm = new CameraComponent(this);
	cm->SnapToIdeal();
	pu = new PlayerUI(this);
}