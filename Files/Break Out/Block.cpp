#include "Block.h"
#include "Game.h"

Block::Block(Game* game) : Actor(game) {
	blockTexture = new SpriteComponent(this);
	blockCol = new CollisionComponent(this);
	blockCol->SetSize(64.0f, 32.0f);
	thisGame = game;
	game->blocks.push_back(this);
	placement = game->blocks.size() - 1;
}

Block::~Block() {
	thisGame->RemoveBlock(this);
}

void Block::changeTexture(std::string fileName) {
	blockTexture->SetTexture(thisGame->GetTexture(fileName));

}