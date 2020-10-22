#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	Vector2 newPosition = deltaTime * mForwardSpeed * mOwner->GetForward();
	mOwner->SetPosition(Vector2(mOwner->GetPosition().x + newPosition.x, mOwner->GetPosition().y + newPosition.y));
}
