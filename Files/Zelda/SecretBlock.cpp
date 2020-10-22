#include "SecretBlock.h"
#include "Game.h"

SecretBlock::SecretBlock(Game* game) : Actor(game) {
	sbTexture = new SpriteComponent(this);
	sbTexture->SetTexture(mGame->GetTexture("Assets/SecretBlock.png"));
	sbCol = new CollisionComponent(this);
	sbCol->SetSize(32, 32);
}

SecretBlock::~SecretBlock() {

}