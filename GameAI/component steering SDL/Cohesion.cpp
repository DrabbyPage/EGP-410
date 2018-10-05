
#include "Cohesion.h"
#include <iostream>
#include <math.h>

#include <cassert>

#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

CohesionSteering::CohesionSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee) :
	Steering(), cohesionArriveSteer(ArriveSteering(ownerID, targetLoc, targetID, shouldFlee))
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

CohesionSteering::~CohesionSteering()
{

}

Steering* CohesionSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* listOfCharacters = gpGame->getUnitManager();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Steering* tempArrive = cohesionArriveSteer.getSteering();

	Vector2D charTotalVec = Vector2D(0, 0);
	Vector2D charAverageVec;

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
				neighborCount = neighborCount + 1;
				charTotalVec = charTotalVec + listOfCharacters->getUnit(i)->getPositionComponent()->getPosition();
			}
		}
	}

	if (neighborCount == 0)
	{
		data.acc = 0;
		data.vel = 0;
		this->mData = data;
		return this;
	}
	else
	{
		// average all the positions together
		charAverageVec = Vector2D(charTotalVec.getX() / neighborCount, charTotalVec.getY() / neighborCount);
		mTargetLoc = charAverageVec;

		// set the future position to arive steering
		tempArrive->setTargetLoc(mTargetLoc);

		// give steering priviledge to arrival steering
		if (tempArrive != NULL)
		{
			data.acc = tempArrive->getData().acc;
		}
		else
		{
			data.rotAcc = 0;
			data.acc = 0;
			data.vel = 0;
			data.rotVel = 0;
		}

		this->mData = data;
		return this;
	}
}
