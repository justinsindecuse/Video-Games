#include "BallMove.h"
#include "Actor.h"
#include "Paddle.h"
#include "Ball.h"
#include "Game.h"
#include "Math.h"
#include "Block.h"
#include "CollisionComponent.h"

BallMove::BallMove(Actor* owner) : MoveComponent(owner) {
	mOwner = owner;
	Velocity = Vector2(250, -250);
	initialV = Velocity;
	amount = 17.0f;
	initial = mOwner->GetPosition();
	LeftNormal = Vector2(-1.0f, -6.0f);
	LeftNormal.Normalize();
	MidNormal = Vector2(0.0f, -1.0f);
	RightNormal = Vector2(1.0f, -6.0f);
	RightNormal.Normalize();
	ballCol = mOwner->GetComponent<CollisionComponent>();
	wallLeft = 40.0f;
	wallRight = 985.0f;
	wallTop = 40.0f;
}

BallMove::~BallMove() {

}

void BallMove::Update(float deltaTime) {
	//Moving the Ball
	Vector2 newPosition = mOwner->GetPosition() + deltaTime * Velocity;
	mOwner->SetPosition(newPosition);

	//Wall Bouncing
	if (newPosition.x < wallLeft) {
		newPosition.x = wallLeft;
		mOwner->SetPosition(newPosition);
		Velocity.x *= -1;
	}
	if (newPosition.x > wallRight) {
		newPosition.x = wallRight;
		mOwner->SetPosition(newPosition);
		Velocity.x *= -1;
	}
	if (newPosition.y < wallTop) {
		newPosition.y = wallTop;
		mOwner->SetPosition(newPosition);
		Velocity.y *= -1;
	}
	if (newPosition.y > 768) {
		newPosition = initial;
		mOwner->SetPosition(newPosition);
		Velocity = initialV;
	}

	//Paddle Reflection
	Paddle* other = mOwner->mGame->GetPaddle();
	if (ballCol->Intersect(other->PaddleCol)) {
		if (Velocity.y > 0) {
			//Left third
			if (newPosition.x < other->GetPosition().x - amount) {
				Velocity = Vector2::Reflect(Velocity, LeftNormal);
			}
			//Right third
			else if (newPosition.x > other->GetPosition().x + amount) {
				Velocity = Vector2::Reflect(Velocity, RightNormal);
			}
			//Middle third
			else {
				Velocity = Vector2::Reflect(Velocity, MidNormal);
			}
		}
	}

	//Hitting a block
	Vector2 offset;
	CollSide side = CollSide::None;
	for (unsigned int i = 0; i < mOwner->mGame->blocks.size(); i++) {
		side = ballCol->GetMinOverlap(mOwner->mGame->blocks[i]->GetComponent<CollisionComponent>(), offset);
		if (side != CollSide::None) {
			mOwner->mGame->blocks[i]->SetState(ActorState::Destroy);
			if (side == CollSide::Top || side == CollSide::Bottom) {
				Velocity.y *= -1.0f;
			}
			if (side == CollSide::Left || side == CollSide::Right) {
				Velocity.x *= -1.0f;
			}
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			break;
		}

	}
}