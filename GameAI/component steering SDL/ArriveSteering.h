
#ifndef ARRIVE_STEER
#define ARRIVE_STEER

#include <Trackable.h>
#include "Steering.h"

// Class:
// holds the kinematic data for the character and target
// character:

// target:

// holds the max acceleration and speed of the character
// max acceleration:

// max speed:

// holds the radius for arriving at the target
// target radius:

// holds the radius for beginning to slow down
// slow radius

// holds the time over which to achieve target speed
// time to target += 0.1


class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID, 
		bool shouldFlee = false);


protected:
	virtual Steering* getSteering();

private:
	float slowRadius;
	float targetRadius;
};

#endif