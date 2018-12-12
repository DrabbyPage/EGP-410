#include "Game.h"
#include "GhostFleeState.h"
#include <iostream>

using namespace std;

void GhostFleeState::onEntrance()
{
	//cout << "\nEntering GhostFleeState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToEdible = false;
	mTransitionToWander = false;
}

void GhostFleeState::onExit()
{
	//cout << "\nexitting GhostFleeState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToEdible = false;
	mTransitionToWander = false;
}

StateTransition* GhostFleeState::update()
{
	//cout << "GhostFleeState!" << endl;

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
	if (mTransitionToWander)
	{
		mTransitionToWander = false;
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