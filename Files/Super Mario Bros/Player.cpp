#include "Player.h"
#include "Game.h"
#include <SDL2/SDL_image.h>

Player::Player(Game* game) : Actor(game) {
	playerTexture = new AnimatedSprite(this, 200);
	playerTexture->SetTexture(game->GetTexture("Assets/Mario/Idle.png"));
	playerCol = new CollisionComponent(this);
	playerCol->SetSize(32.0f, 32.0f);
	playMove = new PlayerMove(this);
	//Idle
	std::vector<SDL_Texture*> idleAnim{
		GetGame()->GetTexture("Assets/Mario/Idle.png")
	};
	playerTexture->AddAnimation("idle", idleAnim);
	playerTexture->SetAnimation("idle");
	//Death
	std::vector<SDL_Texture*> deadAnim{
		GetGame()->GetTexture("Assets/Mario/Dead.png")
	};
	playerTexture->AddAnimation("dead", deadAnim);
	//Jump Left
	std::vector<SDL_Texture*> jlAnim{
		GetGame()->GetTexture("Assets/Mario/JumpLeft.png")
	};
	playerTexture->AddAnimation("jl", jlAnim);
	//Jump Right
	std::vector<SDL_Texture*> jrAnim{
		GetGame()->GetTexture("Assets/Mario/JumpRight.png")
	};
	playerTexture->AddAnimation("jr", jrAnim);
	//Run Left
	std::vector<SDL_Texture*> rlAnim{
		GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
		GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
		GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
	};
	playerTexture->AddAnimation("rl", rlAnim);
	//Run Right
	std::vector<SDL_Texture*> rrAnim{
		GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
		GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
		GetGame()->GetTexture("Assets/Mario/RunRight2.png")
	};
	playerTexture->AddAnimation("rr", rrAnim);
}

Player::~Player() {

}