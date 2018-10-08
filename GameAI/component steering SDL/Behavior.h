
#ifndef BEHAVE_H
#define BEHAVE_H

#include "Steering.h"

struct Behavior
{
	// internal variables
	Steering* behaveSteer;
	float weight;

	void SetWeight(float newWeight) { weight = newWeight; }
	void SetBehavior(Steering* newSteer) { behaveSteer = newSteer; }

	void CreateBehavior(Steering* newSteer, float newWeight)
	{
		SetWeight(newWeight);
		SetBehavior(newSteer);
	}

};


#endif