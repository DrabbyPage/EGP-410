#pragma once
#include "StateMachine.h"

class GhostFleeState : public StateMachineState
{
public:
	GhostFleeState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToWander() { mTransitionToWander = true; }
	void transitionToEdible() { mTransitionToEdible = true; }
private:
	bool mTransitionToChase;
	bool mTransitionToWander;
	bool mTransitionToEdible;
};