#include <cassert>

#include "KinematicAlignSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float KinematicAlignSteer::msTARGET_RADIUS = 1.0f * DEG_TO_RAD;
const float KinematicAlignSteer::msSLOW_RADIUS = 250.0f * DEG_TO_RAD;
const float KinematicAlignSteer::msTIME_TO_TARGET = 0.1f;

void mapRotation(float& rotation);

KinematicAlignSteer::KinematicAlignSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::KIN_ALIGN, ownerID, targetLoc, targetID),
	mIsTargetAngleGiven(false),
	mTargetAngle(0.0f)
{

}

void KinematicAlignSteer::setTargetAngle(const float targetAngle)
{
	mTargetAngle = targetAngle;
	mIsTargetAngleGiven = true;
}

Steering* KinematicAlignSteer::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	float rotationVelocity;

	//Do we have a valid unit to align with?
	if (mTargetID != INVALID_UNIT_ID)
	{
		//Unit to algin with
		Unit* pTarget = pGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();

		/*
		* If we have a unit to align with, do we want to align with it?
		* Or did we set the angle somewhere else? (Ex: in the Face Steering class).
		*/
		if (!mIsTargetAngleGiven)
		{
			mTargetAngle = pTarget->getFacing() - pOwner->getFacing();
		}
	}

	mapRotation(mTargetAngle);
	rotationVelocity = abs(mTargetAngle);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (rotationVelocity < msTARGET_RADIUS)
	{
		//We are done! Return no steering.
		data.rotAcc = 0;
		data.rotVel = 0;
	}
	else
	{
		data.rotVel = data.maxRotVel;
	}

	this->mData = data;
	return this;
}

void KinematicAlignSteer::mapRotation(float& rotation)
{
	rotation = fmod(rotation, TAU);

	if (rotation > PI)
	{
		rotation -= TAU;
	}
	else if (rotation < -PI)
	{
		rotation += TAU;
	}
}