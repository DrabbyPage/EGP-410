
#ifndef COHESION_H
#define COHESION_H

#include <iostream>
#include <Trackable.h>
#include "Steering.h"
#include "ArriveSteering.h"

class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);
	~CohesionSteering();

	virtual Steering* getSteering();

private:
	ArriveSteering cohesionArriveSteer;

	float threshold;
	int neighborCount;

};

#endif

