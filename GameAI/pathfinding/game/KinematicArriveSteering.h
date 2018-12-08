

#ifndef KIN_ARR_STEER
#define KIN_ARR_STEER

#include <Trackable.h>

#include "Steering.h"

class KinematicArriveSteer : public Steering
{
public:
	KinematicArriveSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

private:
	static const float msTARGET_RADIUS;
	static const float msSLOW_RADIUS;
	static const float msTIME_TO_TARGET;
};

#endif // !KIN_ARR_STEER
