
#ifndef KIN_FACE_STEER
#define KIN_FACE_STEER

#include <Trackable.h>

#include "Steering.h"
#include "KinematicAlignSteering.h"

class KinematicFaceSteer : public Steering
{
public:
	KinematicFaceSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

protected:
	KinematicAlignSteer mKinAlignSteering;

private:
	static const float msTARGET_RADIUS;
};

#endif // !KIN_FACE_STEER
