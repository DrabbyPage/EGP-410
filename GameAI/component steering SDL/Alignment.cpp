
#include "Alignment.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

AlignmentSteering::AlignmentSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee) :
	Steering(), alignFaceSteer(FaceSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	mType = Steering::ALIGNMENT;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	
	threshold = 100.0f;
}

Steering* AlignmentSteering::getSteering()
{
	//std::cout << "alignment steering is called" << std::endl;

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* listOfCharacters = gpGame->getUnitManager();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	float totalAngle = 0;
	float averageAngle = 0;

	neighborCount = 0;

	// loop through each target
	for (int i = 0; i < gpGame->getUnitManager()->size(); i++)
	{
		Vector2D direction;
		float distance;
		Unit* currentUnit = listOfCharacters->getUnit(i);
		// if not the character then check for distance
		if (currentUnit != pOwner)
		{
			// check if the target is close
			direction = currentUnit->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			distance = direction.getLength();

			// if within distance send its position to the total positions
			if (distance < threshold)
			{
				totalAngle = totalAngle + currentUnit->getFacing();
				neighborCount = neighborCount + 1;
			}
		}
	}

	if (neighborCount > 0)
	{
		averageAngle = totalAngle / neighborCount;

		float pOwnerX = pOwner->getPositionComponent()->getPosition().getX();
		float pOwnerY = pOwner->getPositionComponent()->getPosition().getY();

		//Vector2D newTargetLoc = Vector2D(pOwnerX + cos(averageAngle), pOwnerY + sin(averageAngle));

		alignFaceSteer = FaceSteering(mOwnerID, mTargetLoc, mTargetID, false);
		Steering* tempSteer;
		tempSteer = alignFaceSteer.getSteering();

		if (tempSteer != NULL)
		{
			data.rotAcc = tempSteer->getData().rotAcc;
			data.rotVel = tempSteer->getData().rotVel;
		}
		else
		{
			data.rotAcc = 0;
			data.rotVel = 0;
		}

		data.acc = 0;
		data.vel = 0;

		this->mData = data;
		return this;
	}
	else
	{
		data.rotAcc = 0;
		data.rotVel = 0;

		data.acc = 0;
		data.vel = 0;

		this->mData = data;
		return this;
	}

}
