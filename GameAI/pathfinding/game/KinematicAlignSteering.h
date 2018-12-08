
#ifndef KIN_ALIGN_STEER
#define KIN_ALIGN_STEER

#include <Trackable.h>

#include "Steering.h"

class KinematicAlignSteer : public Steering
{
public:
	KinematicAlignSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	void setTargetAngle(const float targetAngle);
	virtual Steering* getSteering() override;
	void mapRotation(float& rotation);

protected:
	bool mIsTargetAngleGiven;
	float mTargetAngle;

private:
	static const float msTARGET_RADIUS;
	static const float msSLOW_RADIUS;
	static const float msTIME_TO_TARGET;
};


#endif
