#pragma once
#include "StateMachine.h"
#include "Vector2D.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Unit.h"

class GhostWanderState : public StateMachineState
{
public:
	GhostWanderState( const SM_idType& id ) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToIdle() { mTransitionToIdle = true; }

	void setId(int myId) 
	{
		mID = myId; 
	};
	inline void stopRun() { mContinueRun = false; };

private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
	bool mTransitionToIdle;

	bool mContinueRun;

	int mGhostXDist;
	int mGhostYDist;
	Vector2D mGhostXDir;
	Vector2D mGhostYDir;
	int timer;

	int mID;

	std::pair<Vector2D, Vector2D> mGhostDir;

	int oldIndex, newIndex;
};