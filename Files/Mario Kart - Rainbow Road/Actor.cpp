#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <iostream>

Actor::Actor(Game* game) :mGame(game), mState(ActorState::Active), mPosition(Vector3::Zero), mScale(1.0f), mRotation(0.0f)
{

	mGame->AddActor(this);

}

Actor::~Actor()
{

	for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
		delete mComponents[counter];
	}
	mGame->RemoveActor(this);
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{

	if (mState == ActorState::Active) {
		for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
			mComponents[counter]->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}

	Matrix4 scale = Matrix4::CreateScale(mScale);
	Matrix4 rotate = Matrix4::CreateRotationZ(mRotation);
	Matrix4 translate = Matrix4::CreateTranslation(mPosition);
	mWorldTransform = scale * rotate * translate;
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