#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <iostream>

Actor::Actor(Game* game):mGame(game),mState(ActorState::Active),mPosition(Vector2::Zero),mScale(1.0f),mRotation(0.0f)
{
	// TODO
	mGame->AddActor(this);

}

Actor::~Actor()
{
	// TODO
	for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
		delete mComponents[counter];
	}
	mGame->RemoveActor(this);
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	// TODO
	if (mState == ActorState::Active) {
		for (unsigned int counter = 0; counter < mComponents.size(); counter++) {
			mComponents[counter]->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	// TODO
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

Vector2 Actor::GetForward() {
	float Vx = cos(mRotation);
	float Vy = sin(mRotation);
	Vector2 forwardVector(Vx, -Vy);
	return forwardVector;
}
