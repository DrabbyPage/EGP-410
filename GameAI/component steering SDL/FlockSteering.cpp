
#include "FlockSteering.h"

#include <iostream>
#include <math.h>
#include <cassert>

#include "Steering.h"
#include "Cohesion.h"
#include "Separation.h"
#include "Alignment.h"
#include "FaceSteering.h"
#include "WanderSteering.h"
#include "BlendedSteer.h"

#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"

FlockSteering::FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee): Steering()
{
	mType = Steering::FLOCKING;

	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
	
	std::cout << "adding the behaviors" << std::endl;

	flockBlendSteer = new BlendedSteering(ownerID, targetLoc, targetID, shouldFlee);

	flockAlignSteer = new AlignmentSteering(ownerID, targetLoc, targetID, shouldFlee);
	flockCoSteer = new CohesionSteering(ownerID, targetLoc, targetID, shouldFlee);
	flockSepSteer = new SeparationSteering(ownerID, targetLoc, targetID, shouldFlee);
	flockWanderSteer = new WanderSteering(ownerID, targetLoc, targetID, shouldFlee);

	flockBlendSteer->AddBehavior(flockCoSteer, 0.08f);
	flockBlendSteer->AddBehavior(flockAlignSteer, 0.6f);
	flockBlendSteer->AddBehavior(flockSepSteer, 1.5f);
	flockBlendSteer->AddBehavior(flockWanderSteer, 0.03f);
}
FlockSteering::~FlockSteering()
{
	delete flockBlendSteer;
	delete flockAlignSteer;
	delete flockCoSteer;
	delete flockSepSteer;
	delete flockWanderSteer;
};

Steering* FlockSteering::getSteering()
{
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Steering* tempBlend = flockBlendSteer->getSteering();

	data.acc = tempBlend->getData().acc;
	data.rotVel = tempBlend->getData().rotAcc;

	this->mData = data;
	return this;
}
