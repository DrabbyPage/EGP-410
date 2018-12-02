#ifndef ALIGN_STEERING_H_
#define ALIGN_STEERING_H_

#include <Trackable.h>

#include "Steering.h"

class AlignSteering : public Steering
{
public:
	AlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	void setTargetAngle(const float targetAngle);
	virtual Steering* getSteering() override;

protected:
	bool mIsTargetAngleGiven;
	float mTargetAngle;

private:
	static const float msTARGET_RADIUS;
	static const float msSLOW_RADIUS;
	static const float msTIME_TO_TARGET;
};


#endif