#pragma once
#include "StateMachine.h"
#include <Vector2D.h>
#include "Path.h"

class GhostChaseState : public StateMachineState
{
public:
	GhostChaseState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToWander() { mTransitionToWander = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToIdle(){ mTransitionToIdle = true; }

	void setId(int myId) { mID = myId; }
	inline void stopRun() { mContinueRun = false; }

private:
	bool mTransitionToWander;
	bool mTransitionToFlee;
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