#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
	:Component(owner, 50)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	Vector3 newPosition = deltaTime * mForwardSpeed * mOwner->GetForward();
	mOwner->SetPosition(Vector3(mOwner->GetPosition() + newPosition));

	Vector3 newPosition1 = deltaTime * mStrafeSpeed * mOwner->GetRight();
	mOwner->SetPosition(Vector3(mOwner->GetPosition() + newPosition1));

	mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed());
}
