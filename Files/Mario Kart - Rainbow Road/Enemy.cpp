#include "Enemy.h"
#include "Game.h"
#include "Renderer.h"

Enemy::Enemy(Game* game) : Actor(game) {
	SetScale(0.75f);
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mc->SetTextureIndex(6);
	em = new EnemyMove(this);
}