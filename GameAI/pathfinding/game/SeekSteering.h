#ifndef SEEK_STEERING_H_
#define SEEK_STEERING_H_

#include <Trackable.h>

#include "Steering.h"

class SeekSteering : public Steering
{
public:
	SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, bool shouldFlee = false);

	virtual Steering* getSteering();

private:
	inline SteeringType getSteeringType(bool shouldFlee) { return shouldFlee ? Steering::FLEE : Steering::SEEK; }
};

#endif