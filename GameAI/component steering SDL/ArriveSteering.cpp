
#include "ArriveSteering.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering() {
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		std::cout << "Arrival Steering" << std::endl;
		mType = Steering::ARRIVE;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	slowRadius = 100.0;
	targetRadius = 0.2f;
}

Steering* ArriveSteering::getSteering()
{	
	// get the direction to the target
	// direction = target position - character position
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
	float targetSpeed;
	Vector2D targetVel;
	float timeToTarget = 0.1f;

	if (mTargetID != INVALID_UNIT_ID)
	{
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();

	//std::cout << "dist: " << dist << std::endl;
	// if distance < target raidus 
	//    return none
	// otherwise calculate the scaled speed
	// else
	//     target speed = (max speed * distance) / slowRadius
	if (diff.getLength() < targetRadius)
		return nullptr;
	else if (diff.getLength() > slowRadius)
	{
		targetSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		targetSpeed = ((pOwner->getMaxSpeed() * diff.getLength()) / slowRadius);
	}

	// the target velocity combines speed and direction
	// target velocity = direction
	// target velocity.normalize
	// target velocity *= target speed
	
	targetVel = diff;
	targetVel.normalize();
	targetVel *= targetSpeed;

	// acceleration tries to get to the target velocity
	// steering.linear = target velocity - character velocity
	// steering.linear /= time to target
	data.acc = targetVel - data.vel;
	data.acc /= timeToTarget;

	// check if the acceleration is too fast
	// if steering.linear.length > max acceleration
	//     steering.linear.normalize
	//     steering.linear *= max acceleration
	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	// output the steering
	// steering.angular = 0
	this->mData = data;
	// return steering
	return this;
}
