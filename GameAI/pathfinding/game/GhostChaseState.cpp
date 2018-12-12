#include "Game.h"
#include "GhostChaseState.h"
#include <iostream>

using namespace std;

void GhostChaseState::onEntrance()
{
	//cout << "\nEntering GhostChaseState id:" << mID << endl;
	mTransitionToWander = false;
	mTransitionToEdible = false;
	mTransitionToFlee = false;
}

void GhostChaseState::onExit()
{
	//cout << "\nexitting GhostChaseState id:" << mID << endl;
	mTransitionToWander = false;
	mTransitionToEdible = false;
	mTransitionToFlee = false;
}

StateTransition* GhostChaseState::update()
{
	//cout << "GhostChaseState!" << endl;

	//find the right transition
	if (mTransitionToWander)
	{
		mTransitionToWander = false;
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
	if (mTransitionToEdible)
	{
		mTransitionToEdible = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_EDIBLE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}


	return NULL;//no transition
}