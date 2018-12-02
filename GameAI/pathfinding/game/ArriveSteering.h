#ifndef ARRIVE_STEERING_H_
#define ARRIVE_STEERING_H_

#include <Trackable.h>

#include "Steering.h"

class ArriveSteering : public Steering
{
public:
	ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

private:
	static const float msTARGET_RADIUS;
	static const float msSLOW_RADIUS;
	static const float msTIME_TO_TARGET;
};

#endif