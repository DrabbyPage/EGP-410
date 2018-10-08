#pragma once
#ifndef SEPARATION_H
#define SEPARATION_H

#include <Trackable.h>
#include "Steering.h"

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);

	virtual Steering* getSteering();

private:
	float decayCoeff;// = 200.0f;
	float threshold;// = 30.0f;
	int neighborCount;
};

#endif