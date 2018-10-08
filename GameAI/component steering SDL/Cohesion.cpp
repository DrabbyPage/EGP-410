
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
	mType = Steering::COHESION;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	threshold = 100.0f;
}

CohesionSteering::~CohesionSteering()
{

}

Steering* CohesionSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	UnitManager* listOfCharacters = gpGame->getUnitManager();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D charTotalVec = Vector2D(0, 0);
	Vector2D charAverageVec;

	neighborCount = 0;

	// loop through each target
	for (int i = 0; i < gpGame->getUnitManager()->size(); i++)
	{
		Vector2D direction;
		float distance;
		Unit* currentUnit = listOfCharacters->getUnit(i);
		// if not the character then check for distance
		if (currentUnit != pOwner && currentUnit != NULL)
		{
			// check if the target is close
			direction = currentUnit->getPositionComponent()->getPosition() - pOwner->getPositionComponent()->getPosition();
			distance = direction.getLength();

			// if within distance send its position to the total positions
			if (distance < threshold)
			{
				neighborCount = neighborCount + 1;
				charTotalVec = charTotalVec + currentUnit->getPositionComponent()->getPosition();
			}
		}
	}

	if (neighborCount == 0)
	{
		data.acc = 0;
		data.vel = 0;
		data.rotAcc = 0;
		data.rotVel = 0;

		this->mData = data;
		return this;
	}
	else
	{
		// average all the positions together
		charAverageVec /= neighborCount;

		// set the future position to arive steering
		cohesionArriveSteer.setTargetLoc(charAverageVec);
		cohesionArriveSteer.getSteering();

		data.acc = cohesionArriveSteer.getData().acc;

		data.rotAcc = 0;
		data.rotVel = 0;

		this->mData = data;
		return this;
	}
}
