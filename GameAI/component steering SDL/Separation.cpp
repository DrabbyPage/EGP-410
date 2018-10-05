
#include "Separation.h"
#include <iostream>
#include <math.h>

#include <cassert>

//#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeparationSteering::SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee) : Steering()
{
	mType = Steering::SEPARATION;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeparationSteering::getSteering()
{
	// character object
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);

	// obj that holds list of targets
	UnitManager* listOfTargets = gpGame->getUnitManager();

	// the steering variable holds the output
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// holds the maximum aceleration of the character
	float maxAccel = data.maxAccMagnitude;

	neighborCount = 0;

	// loop through each target
	for (int i = 0; i < gpGame->getUnitManager()->size(); i++)
	{
		Vector2D direction;
		float distance, strength;

		// if not the character then check for distance
		if (i != pOwner->getPositionComponent()->getID())
		{
			// check if the target is close
			direction = listOfTargets->getUnit(i)->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			distance = direction.getLength();

			if (distance < threshold)
			{
				// calculate the strength of repulsion
				strength = min(decayCoeff / pow(distance, 2), maxAccel);

				// add the acceleration
				direction.normalize();
				data.acc += strength * direction.getLength();
				
				neighborCount = neighborCount + 1;
			}

		}
	}

	// return the result
	this->mData = data;
	return this;
	
}