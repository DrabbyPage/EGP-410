
#include "BlendedSteer.h"

#include <iostream>
#include <math.h>
#include <cassert>

#include "Steering.h"
#include "Cohesion.h"
#include "Separation.h"
#include "Alignment.h"
#include "WanderSteering.h"

#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


BlendedSteering::BlendedSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee):
	Steering()
{
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

BlendedSteering::~BlendedSteering()
{
	behaviorList.empty();
}

Steering* BlendedSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Steering* tempBehaveSteer;

	data.acc = 0;
	data.rotAcc = 0;

	for (unsigned int i = 0; i < behaviorList.size(); i++)
	{
		//std::cout << "getting the behavior" << std::endl;
		if (behaviorList[i]->weight != 0)
		{
			tempBehaveSteer = behaviorList[i]->behaveSteer->getSteering();
			data.acc += (tempBehaveSteer->getData().acc * behaviorList[i]->weight);
			data.rotAcc += (tempBehaveSteer->getData().rotAcc * behaviorList[i]->weight);
		}
	}

	data.acc.normalize();
	data.acc *= pOwner->getMaxAcc();

	if (data.rotAcc > pOwner->getMaxRotAcc())
	{
		data.rotAcc = pOwner->getMaxRotAcc();
	}

	this->mData = data;
	return this;
}

void BlendedSteering::AddBehavior(Steering* steer, float weight)
{
	Behavior* newBehavior = new Behavior;
	newBehavior->CreateBehavior(steer, weight);
	behaviorList.push_back(newBehavior);
}
