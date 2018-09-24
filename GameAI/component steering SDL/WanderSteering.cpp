
#include "WanderSteering.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

#include "FaceSteering.h"


WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID,
	bool shouldFlee /*= false*/) : Steering(), mFaceSteer(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))

{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		std::cout << "Wander Steering" << std::endl;
		mType = Steering::WANDER;
	}

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

}

Steering* WanderSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	wanderOffset = 10.0;
	wanderRadius = 80.0;
	wanderRate = 2.0 * 3.1415;
	float wanderOrien = 0;
	float targetOrien = 0;

	// calculate the target to delegate to face

	//update the wander orientation
	wanderOrien += genRandomBinomial() * wanderRate;
	targetOrien = wanderOrien + (pOwner->getFacing() - 3.1415 / 2);

	float charConversion = pOwner->getFacing() - (3.14159f / 2.0f);

	Vector2D charDir = float2Vector2D(charConversion) * wanderOffset;

	//calculate the combined ttarget orientation
	Vector2D targetDir = float2Vector2D(targetOrien);

	// calculate the wander circle
	mTargetLoc = pOwner->getPositionComponent()->getPosition() + charDir * wanderOffset;

	//calculate the target location
	mTargetLoc += targetDir * wanderRadius;

	// delegate to face
	mFaceSteer.setTargetLoc(mTargetLoc);
	Steering* temp = mFaceSteer.getSteering();

	// now set the linear acceleration to be at full
	// acceleration in the direction of the orientation
	if (temp != NULL)
	{
		data.rotAcc = temp->getData().rotAcc;
		data.rotVel = temp->getData().rotVel;
	}

	data.acc = charDir * pOwner->getMaxAcc();

	this->mData = data;
	return this;
}

Vector2D WanderSteering::float2Vector2D(float convert)
{
	Vector2D temp;
	temp.setX(cos(convert));
	temp.setY(sin(convert));
	return temp;
}


