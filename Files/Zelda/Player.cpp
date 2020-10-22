#include "Player.h"
#include "Game.h"
#include <SDL2/SDL_image.h>

Player::Player(Game* game) : Actor(game) {
	playerTexture = new AnimatedSprite(this, 200);
	playerCol = new CollisionComponent(this);
	playerCol->SetSize(20.0f, 20.0f);
	playMove = new PlayerMove(this);

	//Walk up
	std::vector<SDL_Texture*> walkUp{
		GetGame()->GetTexture("Assets/Link/WalkUp0.png"),
		GetGame()->GetTexture("Assets/Link/WalkUp1.png")
	};
	playerTexture->AddAnimation("walkUp", walkUp);
	playerTexture->SetAnimation("walkUp");
	playerTexture->SetIsPaused(true);
	//Walk down
	std::vector<SDL_Texture*> walkDown{
		GetGame()->GetTexture("Assets/Link/WalkDown0.png"),
		GetGame()->GetTexture("Assets/Link/WalkDown1.png")
	};
	playerTexture->AddAnimation("walkDown", walkDown);
	//walk right
	std::vector<SDL_Texture*> walkRight{
		GetGame()->GetTexture("Assets/Link/WalkRight0.png"),
		GetGame()->GetTexture("Assets/Link/WalkRight1.png")
	};
	playerTexture->AddAnimation("walkRight", walkRight);

	//walk left
	std::vector<SDL_Texture*> walkLeft{
		GetGame()->GetTexture("Assets/Link/WalkLeft0.png"),
		GetGame()->GetTexture("Assets/Link/WalkLeft1.png")
	};
	playerTexture->AddAnimation("walkLeft", walkLeft);
}

Player::~Player() {

}