#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;

	if (Intersect(other)) {
		float MaxX = other->GetMax().x - this->GetMin().x;
		float MaxY = other->GetMax().y - this->GetMin().y;
		float MaxZ = other->GetMax().z - this->GetMin().z;
		float MinX = -1.0f * (other->GetMin().x - this->GetMax().x);
		float MinY = -1.0f * (other->GetMin().y - this->GetMax().y);
		float MinZ = -1.0f * (other->GetMin().z - this->GetMax().z);
		if (((MaxZ < MinZ && MaxZ < MaxX) && (MaxZ < MaxY && MaxZ <= MaxZ)) && (MaxZ < MinX && MaxZ < MinY)) {
			offset.z = MaxZ;
			return CollSide::Top;
		}
		else if (((MinZ < MaxZ && MinZ < MaxX) && (MinZ < MaxY && MinZ <= MinZ)) && (MinZ < MinX && MinZ < MinY)) {
			offset.z = -1.0f * MinZ;
			return CollSide::Bottom;
		}
		else if (((MaxX < MaxZ && MaxX < MinZ) && (MaxX < MaxY && MaxX <= MaxX)) && (MaxX < MinX && MaxX < MinY)) {
			offset.x = 1.0f * MaxX;
			return CollSide::SideMaxX;
		}
		else if (((MinX < MaxZ && MinX < MaxX) && (MinX < MaxY && MinX <= MinX)) && (MinX < MinZ && MinX < MinY)) {
			offset.x = -1.0f * MinX;
			return CollSide::SideMinX;
		}
		else if (((MaxY < MaxZ && MaxY < MinZ) && (MaxY < MaxX && MaxY <= MaxY)) && (MaxY < MinX && MaxY < MinY)) {
			offset.y = 1.0f * MaxY;
			return CollSide::SideMaxY;
		}
		else {
			offset.y = -1.0f * MinY;
			return CollSide::SideMinY;
		}
	}
	return CollSide::None;
}
