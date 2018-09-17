#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeekSteering::SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeekSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	diff.normalize();
	diff *= pOwner->getMaxAcc();

	float rotation = atan2(diff.getY(), diff.getX());
	rotation = rotation + (90 * (3.14f / 180));
	pOwner->getPositionComponent()->setFacing(rotation);

	// to find the speed aka magnitude of velocity
	/*
	float curVelX = pOwner->getPhysicsComponent()->getVelocity().getX();
	float curVelY = pOwner->getPhysicsComponent()->getVelocity().getY();

	float velMag = sqrt((curVelX * curVelX) + (curVelY * curVelY));
	*/

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff;
	
	this->mData = data;
	return this;
}

