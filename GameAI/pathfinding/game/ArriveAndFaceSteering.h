#ifndef ARRIVE_AND_FACE_STEERING_H_
#define ARRIVE_AND_FACE_STEERING_H_

#include <Trackable.h>

#include "Steering.h"
#include "FaceSteering.h"
#include "ArriveSteering.h"

class ArriveAndFaceSteering : public Steering
{
public:
	ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

protected:
	ArriveSteering mArriveSteering;
	FaceSteering mFaceSteering;
};

#endif