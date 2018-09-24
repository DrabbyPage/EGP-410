

#ifndef ARRIVE_ET_FACE
#define ARRIVE_ET_FACE

#include <Trackable.h>
#include "Steering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"

class ArriveFaceSteer : public Steering
{
public:
	ArriveFaceSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID,
		bool shouldFlee = false);

	virtual Steering* getSteering();

protected:
	ArriveSteering arriveSteer;
	FaceSteering faceSteer;

};

#endif