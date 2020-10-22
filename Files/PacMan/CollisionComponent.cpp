#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	mOwner = owner;
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	bool case1 = false;
	bool case2 = false;
	bool case3 = false;
	bool case4 = false;
	
	if (this->GetMax().x < other->GetMin().x) {
		case1 = true;
	}
	if (this->GetMax().y < other->GetMin().y) {
		case2 = true;
	}
	if (this->GetMin().x > other->GetMax().x) {
		case3 = true;
	}
	if (this->GetMin().y > other->GetMax().y) {
		case4 = true;
	}
	if (case1 || case2 || case3 || case4) {
		return false;
	}
	return true;
}

Vector2 CollisionComponent::GetMin() const
{
	float minX = mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f;
	float minY = mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(minX, minY);
}

Vector2 CollisionComponent::GetMax() const
{
	float maxX = mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f;
	float maxY = mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(maxX, maxY);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;

	if (Intersect(other)) {
		float MaxXdiff = other->GetMax().x - this->GetMin().x;
		float MaxYdiff = other->GetMax().y - this->GetMin().y;
		float MinXdiff = -1.0f * (other->GetMin().x - this->GetMax().x);
		float MinYdiff = -1.0f * (other->GetMin().y - this->GetMax().y);
		if ((MaxXdiff < MaxYdiff && MaxXdiff < MinYdiff) && MaxXdiff < MinXdiff) {
			offset.x = MaxXdiff;
			return CollSide::Right;
		}
		if ((MinXdiff < MaxYdiff && MinXdiff < MinYdiff) && MinXdiff < MaxXdiff) {
			offset.x = -1.0f * MinXdiff;
			return CollSide::Left;
		}
		if ((MaxYdiff < MaxXdiff && MaxYdiff < MinYdiff) && MaxYdiff < MinXdiff) {
			offset.y = MaxYdiff;
			return CollSide::Bottom;
		}
		else {
			offset.y = -1.0f * MinYdiff;
			return CollSide::Top;
		}
	}
	return CollSide::None;
}

