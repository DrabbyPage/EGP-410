#pragma once
#include "StateMachine.h"
#include "Vector2D.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Path.h"
class GhostFleeState : public StateMachineState
{
public:
	GhostFleeState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToWander() { mTransitionToWander = true; }
	void transitionToIdle() { mTransitionToIdle = true; }

	void setId(int myId) { mID = myId; };
	inline void stopRun() { mContinueRun = false; };

private:
	bool mTransitionToChase;
	bool mTransitionToWander;
	bool mTransitionToIdle;

	bool mContinueRun;

	int mGhostXDist;
	int mGhostYDist;
	Vector2D mGhostXDir;
	Vector2D mGhostYDir;
	int timer;
	Path* mpPath;

	int mID, mPathIndex;

	std::pair<Vector2D, Vector2D> mGhostDir;

	int oldIndex, newIndex;
};