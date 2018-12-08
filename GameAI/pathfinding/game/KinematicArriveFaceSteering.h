#ifndef KIN_ARRIVE_FACE_STEER
#define KIN_ARRIVE_FACE_STEER

#include <Trackable.h>

#include "Steering.h"
#include "KinematicFaceSteering.h"
#include "KinematicArriveSteering.h"

class KinematicArriveFaceSteering : public Steering
{
public:
	KinematicArriveFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	virtual Steering* getSteering() override;

protected:
	KinematicArriveSteer mKinArriveSteering;
	KinematicFaceSteer mKinFaceSteering;
};

#endif