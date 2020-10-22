#include "SideBlock.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

SideBlock::SideBlock(Game* game) : Actor(game) {
	SetScale(500.0f);
	mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mc->SetTextureIndex(1);
}

void SideBlock::OnUpdate(float deltaTime) {
	if (mPosition.x < mGame->mPlayer->GetPosition().x - 500) {
		SetState(ActorState::Destroy);
	}
}