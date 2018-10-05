

#include "FaceSteering.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID,
	bool shouldFlee) : Steering()
{
	mType = Steering::FACE;
	
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	slowRadius = 0.5;
	targetRadius = 0.2f;
	timeToLook = 0.1f;
}

Steering* FaceSteering::getSteering()
{
	// initializing needed components to face towards the target
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	float rotation;
	float targetRotation = 0.1f;
	float rotationSize;

	// making sure the target exists before trying to access its components
	/*if (mTargetID != INVALID_UNIT_ID)
	{
		//std::cout << "there is a target" << std::endl;
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}*/

	// the angle to the target
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	if (diff.getLength() == 0.0f)
	{
		return this;
	}

	float rotTarget = atan2(diff.getY(), diff.getX());

	// get native description to the target
	rotation = rotTarget - pOwner->getPositionComponent()->getFacing();

	// map the reault to the (pi, -pi) interval
	if (rotation > 3.14159f)
		rotation  -= (2.0f * 3.14159f);
	else if (rotation < -3.14159f)
		rotation += (2.0f * 3.14159f);

	rotationSize = abs(rotation);

	// check if we are there, return no steering
	if (rotationSize < targetRotation)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return nullptr;
	}

	// if we are outside the slow radius then use maximum rotation
	// otherwise calculate a scaled rotation
	if (rotationSize > slowRadius)
		targetRotation = pOwner->getMaxRotAcc();
	else
		targetRotation = pOwner->getMaxRotAcc() * rotationSize / slowRadius;

	// the final target rotation combines
	// speed (already in the variable) and direction
	targetRotation *= rotation / rotationSize;

	// acceleration tries to get the target rotation
	data.rotAcc = targetRotation - data.rotVel;
	data.rotAcc /= timeToLook;

	// check if the acceleration is too great 
	if (data.rotAcc > data.maxRotAcc)
	{
		data.rotAcc /= data.rotAcc;
		data.rotAcc *= data.maxRotAcc;
	}

	// output steering
	this->mData = data;

	// return steering
	return this;


}
