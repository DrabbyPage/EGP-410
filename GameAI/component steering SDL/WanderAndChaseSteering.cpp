
#include "WanderAndChaseSteering.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


WanderAndChase::WanderAndChase(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering(),
	wanderSteer(WanderSteering(ownerID, targetLoc, mTargetID)),
	faceSteer(FaceSteering(ownerID, targetLoc, mTargetID)),
	seekSteer(SeekSteering(ownerID, targetLoc, mTargetID))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::WANDER_AND_CHASE;
	}

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* WanderAndChase::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Vector2D characterLoc = gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition();
	Vector2D diff = characterLoc - pOwner->getPositionComponent()->getPosition();
	Steering* tempSteer = NULL;

	if (diff.getLengthSquared() > wanderRadius * wanderRadius)
	{
		tempSteer = wanderSteer.getSteering();
		data.rotAcc = tempSteer->getData().rotAcc;
	}
	else
	{
		seekSteer.setTargetLoc(characterLoc);
		tempSteer = seekSteer.getSteering();
		faceSteer.setTargetLoc(characterLoc);
		data.rotAcc = faceSteer.getSteering()->getData().rotAcc;
	}

	data.acc = tempSteer->getData().acc;

	this->mData = data;
	return this;
}