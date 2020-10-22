#pragma once
#include "Component.h"
#include "Math.h"
#include <unordered_map>
#include <vector>

class PathNode;

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};

	struct info {
		bool explored = false;

		double edgeWeight = 100000;

		double heuristic;

		double totalValue;

		PathNode* mParent;
	};
	
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);

	void aStar(PathNode* start, PathNode* target);

	float distance(PathNode* start, PathNode* end);

	void updateValues(PathNode* current, PathNode* target, PathNode* test, bool found, std::unordered_map<PathNode*, info*> nodes);

	std::vector<PathNode*> openSet;


private:
	// Member data for pathfinding
	float mScareTimer = 7.0f;
	float currScare = 0.0f;

	void ScatterMove(float deltaTime);
	void FrightenedMove(float deltaTime);
	void DeadMove(float deltaTime);
	void ChaseMove(float deltaTime);

	// TargetNode is our current goal node
	PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	// TODO: Add any member data/helper functions here!
};
