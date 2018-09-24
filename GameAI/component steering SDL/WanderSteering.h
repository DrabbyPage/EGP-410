
#ifndef WANDER_STEER
#define WANDER_STEER

#include <Trackable.h>
#include "Steering.h"
#include "FaceSteering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);

	Vector2D float2Vector2D(float convert);

protected:
	virtual Steering* getSteering();

private:
	FaceSteering mFaceSteer;

	float wanderOffset;
	float wanderRadius;
	float wanderRate;
};

#endif