#include "Door.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Game.h"

Door::Door(Game* game) : Actor(game) {
	DoorCol = new CollisionComponent(this);
	DoorTexture = new SpriteComponent(this);
}

Door::~Door() {

}

void Door::DoorSetup(std::string Destination, std::string Direction, std::string State) {
	mDestination = Destination;
	mDirection = Direction;
	mState = State;
	std::string DoorName = "" + Direction + State;
	DoorTexture->SetTexture(mGame->GetTexture("Assets/Door/" + DoorName + ".png"));
	if (Direction == "Up") {
		DoorCol->SetSize(30, 64);
		mResult = Vector2(mPosition.x, mPosition.y + 32);
		destDirection = "Down";
	}
	else if (Direction == "Down") {
		DoorCol->SetSize(30, 64);
		mResult = Vector2(mPosition.x, mPosition.y - 32);
		destDirection = "Up";
	}
	else if (Direction == "Right") {
		DoorCol->SetSize(64, 30);
		mResult = Vector2(mPosition.x - 32, mPosition.y);
		destDirection = "Left";
	}
	else {
		DoorCol->SetSize(64, 30);
		mResult = Vector2(mPosition.x + 32, mPosition.y);
		destDirection = "Right";
	}
}