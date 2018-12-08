#include <cassert>

#include "KinematicArriveSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float KinematicArriveSteer::msTARGET_RADIUS = 0.01f;
const float KinematicArriveSteer::msSLOW_RADIUS = 300.0f;
const float KinematicArriveSteer::msTIME_TO_TARGET = 0.1f;

KinematicArriveSteer::KinematicArriveSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::ARRIVE, ownerID, targetLoc, targetID)
{

}

Steering* KinematicArriveSteer::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Vector2D direction;
	//float distance;
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
	//distance = direction.getLength();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	targetSpeed = pOwner->getMaxSpeed();
	targetSpeed = 40.0f;

	targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	data.vel = targetVelocity;
	data.rotVel = 0;

	this->mData = data;
	return this;
}