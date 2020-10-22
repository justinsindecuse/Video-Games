#include "Key.h"
#include "Game.h"

Key::Key(Game* game) : Actor(game) {
	keyTexture = new SpriteComponent(this);
	keyTexture->SetTexture(mGame->GetTexture("Assets/Key.png"));
	keyCol = new CollisionComponent(this);
	keyCol->SetSize(16, 32);
}