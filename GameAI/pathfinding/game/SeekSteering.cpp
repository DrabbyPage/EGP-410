#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"


SeekSteering::SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee) :
	Steering(getSteeringType(shouldFlee), ownerID, targetLoc, targetID) { }

Steering* SeekSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Vector2D distance;
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//are we seeking a location or a unit?
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = pGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		distance = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		distance = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	distance.normalize();
	distance *= pOwner->getMaxAcc();

	data.acc = distance;
	this->mData = data;
	return this;
}

