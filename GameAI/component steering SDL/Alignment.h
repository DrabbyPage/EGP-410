
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <iostream>
#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class AlignmentSteering : public Steering
{
public:
	AlignmentSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);

	virtual Steering* getSteering();

private:
	FaceSteering alignFaceSteer;

	float threshold;
	int neighborCount;
};

#endif

