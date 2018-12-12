#include "Game.h"
#include "GhostEdibleState.h"
#include <iostream>

using namespace std;

void GhostEdibleState::onEntrance()
{
	//cout << "\nEntering GhostEdibleState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToWander = false;
	mTransitionToFlee = false;
}

void GhostEdibleState::onExit()
{
	//cout << "\nexitting GhostEdibleState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToWander = false;
	mTransitionToFlee = false;
}

StateTransition* GhostEdibleState::update()
{
	//cout << "GhostEdibleState!" << endl;

	//find the right transition
	if (mTransitionToChase)
	{
		mTransitionToChase = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_CHASE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToFlee)
	{
		mTransitionToFlee = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_FLEE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToWander)
	{
		mTransitionToWander = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_WANDER);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}


	return NULL;//no transition
}