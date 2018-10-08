
#include "Separation.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeparationSteering::SeparationSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee) : Steering()
{
	mType = Steering::SEPARATION;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	
	threshold = 95.0f;
	decayCoeff = 1000.0f;

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
	data.acc = 0;

	// loop through each target
	for (int i = 0; i < gpGame->getUnitManager()->size(); i++)
	{
		Vector2D direction;
		float distance, strength;
		Unit* currentUnit = listOfTargets->getUnit(i);
		// if not the character then check for distance
		if (currentUnit != pOwner)
		{
			// check if the target is close
			direction = currentUnit->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			distance = direction.getLength();

			if (distance < threshold)
			{
				// calculate the strength of repulsion
				strength = min((decayCoeff / (distance*distance)), maxAccel);

				// add the acceleration
				direction.normalize();
				data.acc += direction * strength;
				
				neighborCount = neighborCount + 1;
			}

		}
	}

	data.rotAcc = 0;
	data.rotVel = 0;

	// return the result
	this->mData = data;
	return this;
	
}