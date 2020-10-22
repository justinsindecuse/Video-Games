#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <iostream>

Actor::Actor(Game* game, Actor* parent) :mGame(game), mState(ActorState::Active), mPosition(Vector3::Zero), mScale(1.0f), mParent(parent), mRotation(0.0f)
{
	if (mParent == NULL) {
		mGame->AddActor(this);
	}
	else {
		mParent->AddChild(this);
	}
}

Actor::~Actor()
{
	while (!mChildren.empty()) {
		mChildren.pop_back();
	}

	for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
		delete mComponents[counter];
	}

	if (mParent == NULL) {
		mGame->RemoveActor(this);
	}
	else {
		mParent->RemoveChild(this);
	}

	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	CalcWorldTransform();

	if (mState == ActorState::Active) {
		for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
			mComponents[counter]->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}

	CalcWorldTransform();

	for (unsigned int i = 0; i < mChildren.size(); i++) {
		mChildren[i]->Update(deltaTime);
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{

	if (mState == ActorState::Active) {
		for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
			mComponents[counter]->ProcessInput(keyState);
		}
		OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{

}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
		});
}

Vector3 Actor::GetForward() {
	float Vx = cos(mRotation);
	float Vy = sin(mRotation);
	Vector3 forwardVector(Vx, Vy, 0.0f);
	return forwardVector;
}

Vector3 Actor::GetRight() {
	float Vx = cos(mRotation + Math::PiOver2);
	float Vy = sin(mRotation + Math::PiOver2);
	Vector3 forwardVector(Vx, Vy, 0.0f);
	return forwardVector;
}

Vector3 Actor::GetQuatForward() {
	Vector3 temp = Vector3::Transform(Vector3::UnitX, GetQuaternion());
	temp.Normalize();
	return temp;
}

void Actor::CalcWorldTransform() {
	Matrix4 scale = Matrix4::CreateScale(mScale);
	Matrix4 rotate = Matrix4::CreateRotationZ(mRotation);
	Matrix4 rotate2 = Matrix4::CreateFromQuaternion(GetQuaternion());
	Matrix4 translate = Matrix4::CreateTranslation(mPosition);
	mWorldTransform = scale * rotate * rotate2 * translate;
	if (mParent != NULL) {
		if (mInheritScale == true) {
			mWorldTransform *= mParent->GetWorldTransform();
		}
		else {
			mWorldTransform *= mParent->GetWorldRotTrans();
		}
	}
}

Matrix4 Actor::GetWorldRotTrans() {
	Matrix4 rotate = Matrix4::CreateRotationZ(mRotation);
	Matrix4 rotate2 = Matrix4::CreateFromQuaternion(GetQuaternion());
	Matrix4 translate = Matrix4::CreateTranslation(mPosition);
	Matrix4 temp = rotate * rotate2 * translate;
	if (mParent == NULL) {
		return temp;
	}
	return temp * mParent->GetWorldRotTrans();
}

Vector3 Actor::GetWorldPosition() {
	return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward() {
	return mWorldTransform.GetXAxis();
}

void Actor::RemoveChild(Actor* actor) {
	auto iter = std::find(mChildren.begin(), mChildren.end(), actor);
	if (iter != mChildren.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mChildren.end() - 1;
		std::iter_swap(iter, iter2);
		mChildren.pop_back();
	}
}