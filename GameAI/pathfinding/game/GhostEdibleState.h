#pragma once
#include "StateMachine.h"

class GhostEdibleState : public StateMachineState
{
public:
	GhostEdibleState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToWander() { mTransitionToWander = true; }
private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
	bool mTransitionToWander;
};