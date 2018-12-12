#pragma once
#include "StateMachine.h"

class GhostWanderState : public StateMachineState
{
public:
	GhostWanderState( const SM_idType& id ) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update();

	void transitionToChase() { mTransitionToChase = true; }
	void transitionToFlee() { mTransitionToFlee = true; }
	void transitionToEdible() { mTransitionToEdible = true; }
private:
	bool mTransitionToChase;
	bool mTransitionToFlee;
	bool mTransitionToEdible;
};