#ifndef FACE_STEERING_H_
#define FACE_STEERING_H_

#include <Trackable.h>

#include "Steering.h"
#include "AlignSteering.h"

class FaceSteering : public Steering
{
public:
	FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

protected:
	AlignSteering mAlignSteering;

private:
	static const float msTARGET_RADIUS;
};

#endif