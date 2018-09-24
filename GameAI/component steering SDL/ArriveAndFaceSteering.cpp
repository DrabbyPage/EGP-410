
#include "ArriveAndFaceSteering.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveFaceSteer::ArriveFaceSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID,
	bool shouldFlee /*= false*/) : Steering(), arriveSteer(ArriveSteering(ownerID, targetLoc, targetID, shouldFlee)),
	faceSteer(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::ARRIVE_AND_FACE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}


Steering* ArriveFaceSteer::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Steering* tempFace = faceSteer.getSteering();
	Steering* tempArrive = arriveSteer.getSteering();

	if (tempArrive != NULL)
	{
		data.acc = tempArrive->getData().acc;

		if (tempFace != NULL)
		{
			data.rotAcc = tempFace->getData().rotAcc;
		}
	}
	else
	{
		data.rotAcc = 0;
		data.acc = 0;
		data.vel = 0;
		data.rotVel = 0;
	}

	this->mData = data;
	return this;
}