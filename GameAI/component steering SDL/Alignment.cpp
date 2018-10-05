
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
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* AlignmentSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* listOfCharacters = gpGame->getUnitManager();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Steering* tempFace = alignFaceSteer.getSteering();

	float totalAngle = 0;
	float averageAngle = 0;

	neighborCount = 0;

	// loop through each target
	for (int i = 0; i < gpGame->getUnitManager()->size(); i++)
	{
		Vector2D direction;
		float distance;

		// if not the character then check for distance
		if (i != pOwner->getPositionComponent()->getID())
		{
			// check if the target is close
			direction = listOfCharacters->getUnit(i)->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			distance = direction.getLength();

			// if within distance send its position to the total positions
			if (distance < threshold)
			{
				totalAngle = totalAngle + listOfCharacters->getUnit(i)->getFacing();
				neighborCount = neighborCount + 1;
			}
		}
	}

	if (neighborCount == 0)
	{
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	else
	{
		averageAngle = totalAngle / neighborCount;

		float pOwnerX = pOwner->getPositionComponent()->getPosition().getX();
		float pOwnerY = pOwner->getPositionComponent()->getPosition().getY();

		mTargetLoc = Vector2D(pOwnerX + cos(averageAngle), pOwnerY + sin(averageAngle));

		tempFace->setTargetLoc(mTargetLoc);
		
		// give steering priviledge to arrival steering
		if (tempFace != NULL)
		{
			data.rotAcc = tempFace->getData().rotAcc;
			data.rotVel = tempFace->getData().rotVel;
		}
		else
		{
			data.rotAcc = 0;
			data.rotVel = 0;
		}

		this->mData = data;
		return this;
	}
}