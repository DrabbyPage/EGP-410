

#ifndef FACE_STEERING
#define FACE_STEERING

#include <Trackable.h>
#include "Steering.h"

class FaceSteering : public Steering
{
public:
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);


protected:
	virtual Steering* getSteering();

private:
	float slowRadius;
	float targetRadius;
	float timeToLook;
};

#endif