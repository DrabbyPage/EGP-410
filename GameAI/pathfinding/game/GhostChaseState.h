#pragma once
#include "StateMachine.h"

class GhostChaseState : public StateMachineState
{
public:
	GhostChaseState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToWander() { mTransitionToWander = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToEdible() { mTransitionToEdible = true; }
private:
	bool mTransitionToWander;
	bool mTransitionToFlee;
	bool mTransitionToEdible;
};