#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	if (mState == State::Scatter) {
		ScatterMove(deltaTime);
	}
	else if (mState == State::Frightened) {
		FrightenedMove(deltaTime);
		currScare += deltaTime;
		if (currScare >= 5.0f) {
			mOwner->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
		}
		if (currScare >= mScareTimer) {
			currScare = 0.0f;
			mState = State::Scatter;
		}
	}
	else if (mState == State::Dead) {
		DeadMove(deltaTime);
	}
	else if (mState == State::Chase) {
		ChaseMove(deltaTime);
	}
}


void GhostAI::Frighten()
{
	if (!mGhost->IsDead()) {
		mState = State::Frightened;
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
		PathNode* holder = mNextNode;
		mNextNode = mPrevNode;
		mPrevNode = holder;
		mPath.clear();
	}
}

void GhostAI::Start(PathNode* startNode)
{
	mOwner->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mPrevNode = nullptr;
	mNextNode = nullptr;
	mTargetNode = nullptr;
	aStar(startNode, mGhost->GetScatterNode());
}

void GhostAI::Die()
{
	mState = State::Dead;
	currScare = 0.0f;
	mPath.clear();
	mTargetNode = mOwner->GetGame()->mGhostPen;

	//Vector2 direction = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());

}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	if (mNextNode)
	{
		// Line from next node to subsequent on path
		SDL_RenderDrawLine(render,
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y),
			static_cast<int>(mPath.back()->GetPosition().x),
			static_cast<int>(mPath.back()->GetPosition().y));
	}

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}

void GhostAI::aStar(PathNode* start, PathNode* target) {
	mPath.clear();
	openSet.clear();
	PathNode* mCurrentNode = start;
	std::unordered_map<PathNode*, info*> nodes;
	mTargetNode = target;
	mPrevNode = mCurrentNode;
	nodes.insert(std::pair<PathNode*, info*>(mCurrentNode, new info));
	nodes.find(mCurrentNode)->second->explored = true;
	//loop until current node is the target node
	while (mCurrentNode != target) {
		//test every adjacent node to the current node
		for (unsigned int i = 0; i < mCurrentNode->mAdjacent.size(); i++) {
			PathNode* testNode = mCurrentNode->mAdjacent[i];
			//test to see if test node is avaialbe to be searched and moves on in the adjacency list otherwise
			if (testNode->GetType() == PathNode::Tunnel) {
			}
			else
			{
				//if we have not visited a node yet
				if (nodes.find(testNode) == nodes.end()) {
					nodes.insert(std::pair<PathNode*, info*>(testNode, new info));
					//update testNode values/add to open set
					updateValues(mCurrentNode, target, testNode, false, nodes);
				}
				else if (nodes.find(testNode)->second->explored == false) {
					//test to see if testNode is in the open set
					bool testNodeFound = false;
					for (unsigned int j = 0; j < openSet.size(); j++) {
						if (testNode == openSet[j]) {
							testNodeFound = true;
						}
					}
					//update testNode values/add to open set
					updateValues(mCurrentNode, target, testNode, testNodeFound, nodes);
				}
			}
		}
		//quick safety check
		if (openSet.size() <= 0)
			break;

		//test every open node
		PathNode* bestNode = openSet[0];
		int position = 0;
		for (unsigned int i = 1; i < openSet.size(); i++) {
			if (nodes.find(openSet[i])->second->totalValue < nodes.find(bestNode)->second->totalValue) {
				bestNode = openSet[i];
				position = i;
			}
		}
		//sets parent for current node, new node
		mCurrentNode = bestNode;
		openSet.erase(openSet.begin() + position);
		nodes.find(mCurrentNode)->second->explored = true;
	}
	//Constructs mPath and finds the next node
	while (mCurrentNode != mPrevNode) {
		mPath.push_back(mCurrentNode);
		if (nodes.find(mCurrentNode)->second->mParent == mPrevNode) {
			mNextNode = mCurrentNode;
		}
		mCurrentNode = nodes.find(mCurrentNode)->second->mParent;
	}
}

//Calculates the euclidean Distance between Nodes
float GhostAI::distance(PathNode* start, PathNode* end) {
	float mdistance = 0.0f;
	float xdist = end->GetPosition().x - start->GetPosition().x;
	xdist = pow(xdist, 2);
	float ydist = end->GetPosition().y - start->GetPosition().y;
	ydist = pow(ydist, 2);
	mdistance = pow(xdist + ydist, 0.5f);
	return mdistance;
}

//update its values if it is in the open set otherwise it will add it to the open set if it is not in it and assign its initial values
void GhostAI::updateValues(PathNode* current, PathNode* target, PathNode* testNode, bool found, std::unordered_map<PathNode*, info*> nodes) {
	nodes.find(testNode)->second->heuristic = distance(testNode, target);
	if (found) {
		double newG = nodes.find(current)->second->edgeWeight + distance(current, testNode);
		if (newG < nodes.find(testNode)->second->edgeWeight) {
			nodes.find(testNode)->second->mParent = current;
			nodes.find(testNode)->second->edgeWeight = newG;
			nodes.find(testNode)->second->totalValue = nodes.find(testNode)->second->edgeWeight + nodes.find(testNode)->second->heuristic;
		}
	}
	else {
		double newG = nodes.find(current)->second->edgeWeight + distance(current, testNode);
		nodes.find(testNode)->second->mParent = current;
		nodes.find(testNode)->second->edgeWeight = newG;
		nodes.find(testNode)->second->totalValue = nodes.find(testNode)->second->edgeWeight + nodes.find(testNode)->second->heuristic;
		openSet.push_back(testNode);
	}
	
}

void GhostAI::ScatterMove(float deltaTime) {
	aStar(mPrevNode, mGhost->GetScatterNode());
	//When the ghost hits a node, updates prevNode and NextNode from mpath
	if (mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
		mOwner->SetPosition(mNextNode->GetPosition());
		if (mNextNode != mTargetNode) {
			mPrevNode = mNextNode;
			mNextNode = mPath.back();
			mPath.pop_back();
		}
	}
	//otherwise proceeds as normal
	else {
		Vector2 pos = mOwner->GetPosition();
		Vector2 direction = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());
		pos += 90.0f * deltaTime * direction;
		mOwner->SetPosition(pos);
	}
}

void GhostAI::FrightenedMove(float deltaTime) {
	//When the ghost is at a node, choose a random direction
	if (mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
		PathNode* randNode;
		std::vector<PathNode*> choices;
		for (unsigned int i = 0; i < mNextNode->mAdjacent.size(); i++) {
			if ((mNextNode->mAdjacent[i]->GetType() != PathNode::Ghost && mNextNode->mAdjacent[i]->GetType() != PathNode::Ghost) && mNextNode->mAdjacent[i] != mPrevNode) {
				choices.push_back(mNextNode->mAdjacent[i]);
			}
		}
		if (choices.empty()) {
			for (unsigned int i = 0; i < mNextNode->mAdjacent.size(); i++) {
				if (mNextNode->mAdjacent[i]->GetType() == PathNode::Ghost) {
					choices.push_back(mNextNode->mAdjacent[i]);
				}
			}
		}
		int index = Random::GetIntRange(0, (int)choices.size() - 1);
		randNode = choices[index];
		

		mPrevNode = mNextNode;
		mNextNode = randNode;
	}
	//otherwise proceed normally
	else {
		Vector2 pos = mOwner->GetPosition();
		Vector2 direction = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());
		pos += 65.0f * deltaTime * direction;
		mOwner->SetPosition(pos);
	}
}

void GhostAI::DeadMove(float deltaTime) {
	if (mOwner->GetComponent<CollisionComponent>()->Intersect(mPrevNode->GetComponent<CollisionComponent>())) {
		aStar(mPrevNode, mGhost->GetGame()->mGhostPen);
	}
	//if the ghost is colliding with a node
	if (mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
		mOwner->SetPosition(mNextNode->GetPosition());
		if (mNextNode == mTargetNode) {
			Start(mNextNode);
		}
		else {
			mPrevNode = mNextNode;
			mOwner->SetPosition(mNextNode->GetPosition());
			if (mNextNode->GetType() != PathNode::Type::Tunnel) {
				//aStar(mOwner->GetGame()->mGhostPen);
				mNextNode = mPath.back();
				mPath.pop_back();
			}
		}
	}
	//otherwise ghost moves as normal
	else {
		Vector2 pos = mOwner->GetPosition();
		Vector2 direction = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());
		pos += 125.0f * deltaTime * direction;
		mOwner->SetPosition(pos);
	}
}

void GhostAI::ChaseMove(float deltaTime) {

}