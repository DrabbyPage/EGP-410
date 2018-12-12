#include "Game.h"
#include "GhostWanderState.h"
#include <iostream>

using namespace std;

void GhostWanderState::onEntrance()
{
	//cout << "\nEntering GhostWanderState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToEdible = false;
	mTransitionToFlee = false;
}

void GhostWanderState::onExit()
{
	//cout << "\nexitting GhostWanderState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToEdible = false;
	mTransitionToFlee = false;
}

StateTransition* GhostWanderState::update()
{
	//cout << "GhostWanderState!" << endl;

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