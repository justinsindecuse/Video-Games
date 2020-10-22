#include "EnemyMove.h"
#include "HeightMap.h"
#include "Game.h"
#include "Actor.h"
#include "CSVHelper.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

EnemyMove::EnemyMove(Actor* owner) : VehicleMove(owner) {
	std::ifstream fileReader("Assets/HeightMap/Path.csv");

	maxAccel = 2000.0f;

	std::string line;
	//constructs csv vector
	getline(fileReader, line);
	getline(fileReader, line);
	while (!line.empty()) {
		std::vector<std::string> holder;
		std::vector<int> temp;

		holder = CSVHelper::Split(line);
		Vector3 node = mOwner->mGame->mMap->CellToWorld(std::stoi(holder[1]), std::stoi(holder[2]));
		pathNodes.push_back(node);
		getline(fileReader, line);
	}
	mOwner->SetPosition(pathNodes[0]);
}

void EnemyMove::Update(float deltaTime) {
	//test if it has reached the node
	if (distance(mOwner->GetPosition(), pathNodes[nextNode]) < 100.0f) {
		if (nextNode == 27) {
			nextNode = 0;
		}
		else {
			nextNode++;
		}
	}
	Vector3 targetVector = pathNodes[nextNode] - mOwner->GetPosition();
	targetVector.Normalize();
	
	//test to see if it should press on the pedal
	float dotProduct = Vector3::Dot(mOwner->GetForward(), targetVector);
	if (dotProduct > 0.80) {
		pressed = true;
	}
	else {
		pressed = false;
	}
	//test to see if and how it should turn
	if (Math::Abs(dotProduct) > .1) {
		if (Vector3::Cross(mOwner->GetForward(), targetVector).z < 0) {
			turn = left;
		}
		else {
			turn = right;
		}
	}
	else {
		turn = none;
	}

	VehicleMove::Update(deltaTime);

}

float EnemyMove::distance(Vector3 first, Vector3 second) {
	float mdistance = 0.0f;
	float xdist = second.x - first.x;
	xdist = pow(xdist, 2);
	float ydist = second.y - first.y;
	ydist = pow(ydist, 2);
	float zdist = second.z - first.z;
	zdist = pow(zdist, 2);
	mdistance = pow(xdist + ydist + zdist, 0.5f);
	return mdistance;
}