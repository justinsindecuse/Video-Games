#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

enum direct {
	none,
	left,
	right,
};

class VehicleMove : public Component {
public:
	VehicleMove(Actor* owner);

	//dynamic Vars
	direct turn = none;
	bool pressed = false;
	Vector3 velocity;
	float angVel = 0.0f;
	float accelTime = 0.0f;

	//static Vars
	float minAccel = 1000.0f;
	float maxAccel = 2500.0f;
	float accelRamp = 2.0f;
	float angAccel = 5 * Math::Pi;
	float dragPressed = 0.9f;
	float dragNoPress = 0.975f;
	float dragAng = 0.9f;

	void Update(float deltaTime);

	std::vector< std::vector<int> > checkPoints;

	int currentLap = 0;
	int lastCheck = 0;
	bool startingOff = false;
	bool Lapped = false;

	virtual void OnLapChange(int newLap) { };

	float dToNextCheck();
};