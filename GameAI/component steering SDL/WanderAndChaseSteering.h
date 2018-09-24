
#ifndef WANDER_CHASE
#define WANDER_CHASE

#include <Trackable.h>
#include "Steering.h"
#include "WanderSteering.h"
#include "SeekSteering.h"

class WanderAndChase : public Steering
{
public:
	WanderAndChase(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);

	virtual Steering* getSteering();

private:
	const float wanderRadius = 200.0f;
	WanderSteering wanderSteer;
	FaceSteering faceSteer;
	SeekSteering seekSteer;
};

#endif