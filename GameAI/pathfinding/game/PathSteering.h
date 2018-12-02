#ifndef PATH_STEERING_H_
#define PATH_STEERING_H_

#include <Trackable.h>

#include "Steering.h"
#include "ArriveAndFaceSteering.h"
#include "Path.h"

class PathSteering : public Steering
{
public:
	PathSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);
	virtual ~PathSteering();

	virtual Steering* getSteering();

	void setPath(Path path) 
	{
		resetPathIndex();
		mPath = path;
	}

	void resetPathIndex()
	{
		targetPathIndex = 0;
	}

protected:
	ArriveAndFaceSteering mArriveAndFaceSteering;
	Path mPath;
	int targetPathIndex;
};

#endif