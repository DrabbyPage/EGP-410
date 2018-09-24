
#ifndef ARRIVE_STEER
#define ARRIVE_STEER

#include <Trackable.h>
#include "Steering.h"

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