#ifndef PAC_MAN_STEER
#define PAC_MAN_STEER

#include <Trackable.h>

#include "Steering.h"
#include "KinematicArriveFaceSteering.h"
#include "Path.h"

class PacManSteering : public Steering
{
public:
	PacManSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	virtual ~PacManSteering();

	virtual Steering* getSteering();

	void setPath(Path path);


	void resetPathIndex()
	{
		targetPathIndex = 0;
	}

protected:
	KinematicArriveFaceSteering mKinArriveFaceSteer;
	Path mPath;
	int targetPathIndex;
};

#endif
