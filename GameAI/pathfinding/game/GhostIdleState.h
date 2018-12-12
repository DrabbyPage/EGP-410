#pragma once
#pragma once
#include "StateMachine.h"
#include "Vector2D.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Path.h"

class GhostIdleState : public StateMachineState
{
public:
	GhostIdleState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToWander() { mTransitionToWander = true; }

	void setId(int myId) { mID = myId; };

private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
	bool mTransitionToWander;

	bool mContinueRun;

	int mGhostXDist;
	int mGhostYDist;
	Vector2D mGhostXDir;
	Vector2D mGhostYDir;
	Path* mpPath;
	int mID, mPathIndex;
	int timer;

	std::pair<Vector2D, Vector2D> mGhostDir;

	int oldIndex, newIndex;
};