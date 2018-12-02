#include <cassert>

#include "AlignSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float AlignSteering::msTARGET_RADIUS = 1.0f * DEG_TO_RAD;
const float AlignSteering::msSLOW_RADIUS = 250.0f * DEG_TO_RAD;
const float AlignSteering::msTIME_TO_TARGET = 0.1f;

void mapRotation(float& rotation);

AlignSteering::AlignSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::ALIGN, ownerID, targetLoc, targetID),
	mIsTargetAngleGiven(false),
	mTargetAngle(0.0f)
{

}

void AlignSteering::setTargetAngle(const float targetAngle)
{
	mTargetAngle = targetAngle;
	mIsTargetAngleGiven = true;
}

Steering* AlignSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	//float maxRotationalAcc = pOwner->getMaxRotAcc();
	//float maxRotationalVel = pOwner->getMaxRotVel();

	float rotationVelocity;
	float targetRotationVelocity;

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
		if (rotationVelocity > msSLOW_RADIUS)
		{
			targetRotationVelocity = data.maxRotAcc;
		}
		else
		{
			targetRotationVelocity = data.maxRotVel * rotationVelocity / msSLOW_RADIUS;
		}

		targetRotationVelocity *= (mTargetAngle / rotationVelocity);

		data.rotAcc = (targetRotationVelocity - data.rotVel);
		data.rotAcc /= msTIME_TO_TARGET;

		float angularAcc = abs(data.rotAcc);

		if (angularAcc > data.maxRotAcc)
		{
			data.rotAcc /= angularAcc;
			data.rotAcc *= data.maxRotAcc;
		}
	}

	this->mData = data;
	return this;
}

void mapRotation(float& rotation)
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