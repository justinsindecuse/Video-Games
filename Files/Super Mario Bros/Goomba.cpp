#include "Goomba.h"
#include "Game.h"
#include <SDL2/SDL_image.h>

Goomba::Goomba(Game* game) : Actor(game) {
	GoombaTexture = new AnimatedSprite(this, 150);
	GoombaCol = new CollisionComponent(this);
	GoombaCol->SetSize(32.0f, 32.0f);
	GoomMove = new GoombaMove(this);
	game->enemies.push_back(this);
	stomped = false;
	std::vector<SDL_Texture*> walkAnim{
		GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
		GetGame()->GetTexture("Assets/Goomba/Walk1.png")
	};
	GoombaTexture->AddAnimation("walk", walkAnim);
	GoombaTexture->SetAnimation("walk");
	std::vector<SDL_Texture*> deadAnim{
		GetGame()->GetTexture("Assets/Goomba/Dead.png")
	};
	GoombaTexture->AddAnimation("dead", deadAnim);
}

Goomba::~Goomba() {
	mGame->removeEnemy(this);
}