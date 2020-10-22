#include "VehicleMove.h"
#include "Actor.h"
#include "Game.h"
#include "HeightMap.h"
#include "CSVHelper.h"
#include "HeightMap.h"
#include <iostream>
#include <fstream>

VehicleMove::VehicleMove(Actor* owner) : Component(owner, 50) {
	std::ifstream fileReader("Assets/HeightMap/Checkpoints.csv");

	std::string line;
	//constructs csv vector
	getline(fileReader, line);
	getline(fileReader, line);
	while (!line.empty()) {
		std::vector<std::string> holder;
		std::vector<int> temp;

		holder = CSVHelper::Split(line);
		for (int i = 1; i <= 4; i++) {
			temp.push_back(std::stoi(holder[i]));
		}
		checkPoints.push_back(temp);
		getline(fileReader, line);
	}
}

void VehicleMove::Update(float deltaTime) {
	float accMag;
	//update velocity
	if (pressed) {
		accelTime += deltaTime;
		float ramp = accelTime / accelRamp;
		accMag = Math::Lerp(minAccel, maxAccel, ramp);
		float temp = pow(velocity.x, 2) + pow(velocity.y, 2);
		if (!(pow(temp, .5) > 400)) {
			velocity += mOwner->GetForward() * accMag * deltaTime;
		}
	}
	else {
		accelTime = 0.0f;
	}
	//update position
	mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));
	//update z based on height
	Vector3 holder = mOwner->GetPosition();
	if (mOwner->mGame->mMap->isOnTrack(holder.x, holder.y)) {
		float z = mOwner->mGame->mMap->GetHeight(holder.x, holder.y);
		mOwner->SetPosition(Vector3(holder.x, holder.y, Math::Lerp(holder.z, z, 0.1f)));
	}
	//linear drag
	if (pressed) {
		velocity *= dragPressed;
	}
	else {
		velocity *= dragNoPress;
	}

	//update turn velocity
	if (turn != none) {
		if (turn == right) {
			angVel += angAccel * 1.0f * deltaTime;
		}
		else if (turn == left) {
			angVel += angAccel * -1.0f * deltaTime;
		}
	}

	//update angle
	mOwner->SetRotation(mOwner->GetRotation() + angVel * deltaTime);

	//update angular drag
	angVel *= dragAng;

	Vector3 temp;

	if (!startingOff) {
		if (lastCheck != 7) {
			temp = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck][0], checkPoints[lastCheck + 1][2]);
		}
		else {
			temp = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck][0], checkPoints[0][2]);
		}
	}
	
	Vector3 temp1;
	Vector3 temp2;

	if (lastCheck == 7) {
		temp1 = mOwner->mGame->mMap->CellToWorld(checkPoints[0][0], checkPoints[0][2]);
		temp2 = mOwner->mGame->mMap->CellToWorld(checkPoints[0][1], checkPoints[0][3]);
	}
	else {
		temp1 = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck + 1][0], checkPoints[lastCheck + 1][2]);
		temp2 = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck + 1][1], checkPoints[lastCheck + 1][3]);
	}

	Vector2 nextPos1 = Vector2(temp1.x, temp1.y);
	Vector2 nextPos2 = Vector2(temp2.x, temp2.y);


	//checks if first lap
	if (startingOff) {
		if (mOwner->GetPosition().x > nextPos1.x) {
			startingOff = false;
			lastCheck++;
		}
	}
	else {
		//if x is the line
		if (nextPos1.x == nextPos2.x) {
			//if coming from smaller x value
			if (temp.x < nextPos1.x) {
				if (mOwner->GetPosition().x >= nextPos1.x) {
					lastCheck++;
				}
			}
			//if coming from a larger y value
			else {
				if (mOwner->GetPosition().x <= nextPos1.x) {
					lastCheck++;
				}
			}
		}
		//if y is the line
		else {
			//if coming from smaller y value
			if (temp.x < nextPos1.x) {
				if (mOwner->GetPosition().y >= nextPos1.y) {
					lastCheck++;
				}
			}
			//if coming from a larger y value
			else {
				if (mOwner->GetPosition().y <= nextPos1.y) {
					lastCheck++;
				}
			}
		}
	}
	if (lastCheck == 0 && !Lapped) {
		currentLap++;
		OnLapChange(currentLap);
		Lapped = true;
	}
	if (lastCheck > 0) {
		Lapped = false;
	}
	if (lastCheck == 8) {
		lastCheck = 0;
	}
	
}

float VehicleMove::dToNextCheck() {
	Vector3 temp1;
	Vector3 temp2;

	if (lastCheck == 7) {
		temp1 = mOwner->mGame->mMap->CellToWorld(checkPoints[0][0], checkPoints[0][2]);
		temp2 = mOwner->mGame->mMap->CellToWorld(checkPoints[0][1], checkPoints[0][3]);
	}
	else {
		temp1 = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck + 1][0], checkPoints[lastCheck + 1][2]);
		temp2 = mOwner->mGame->mMap->CellToWorld(checkPoints[lastCheck + 1][1], checkPoints[lastCheck + 1][3]);
	}

	Vector3 distV = temp1 - mOwner->GetPosition();
	return distV.Length();
}