#include <cassert>

#include "ArriveSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float ArriveSteering::msTARGET_RADIUS = 0.01f;
const float ArriveSteering::msSLOW_RADIUS = 300.0f;
const float ArriveSteering::msTIME_TO_TARGET = 0.1f;

ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::ARRIVE, ownerID, targetLoc, targetID)
{

}

Steering* ArriveSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Vector2D direction;
	float distance;
	float targetSpeed;
	Vector2D targetVelocity;

	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	if (mTargetID != INVALID_UNIT_ID)
	{
		//arriving unit
		Unit* pTarget = pGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	distance = direction.getLength();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (distance < msTARGET_RADIUS)
	{
		//We are done! Return no steering.
		data.acc = 0;
		data.vel = 0;
	}
	else
	{
		if (distance > msSLOW_RADIUS)
		{
			targetSpeed = pOwner->getMaxSpeed();
		}
		else
		{
			targetSpeed = pOwner->getMaxSpeed() * distance / msSLOW_RADIUS;
		}

		targetVelocity = direction;
		targetVelocity.normalize();
		targetVelocity *= targetSpeed;

		data.acc = targetVelocity - data.vel;
		data.acc /= msTIME_TO_TARGET;

		if (data.acc.getLengthSquared() > pOwner->getMaxAcc() * pOwner->getMaxAcc())
		{
			data.acc.normalize();
			data.acc *= pOwner->getMaxAcc();
		}
	}

	this->mData = data;
	return this;
}