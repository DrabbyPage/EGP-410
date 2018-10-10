
#include "FlockSteering.h"

#include <iostream>
#include <math.h>
#include <cassert>
#include <fstream>
#include <string>

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
	float alignWeight, cohesionWeight, separWeight, wanderWeight;
	std::string value;
	fileName = "FlockValues.txt";
	mFile.open(fileName);

	mFile.ignore(256, ':');
	std::getline(mFile, value);
	alignWeight = stof(value);

	mFile.ignore(256, ':');
	std::getline(mFile, value);
	cohesionWeight = stof(value);

	mFile.ignore(256, ':');
	std::getline(mFile, value);
	separWeight = stof(value);

	mFile.ignore(256, ':');
	std::getline(mFile, value);
	wanderWeight = stof(value);
	
	mFile.close();

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

	flockBlendSteer->AddBehavior(flockCoSteer, cohesionWeight);
	flockBlendSteer->AddBehavior(flockAlignSteer, alignWeight); 
	flockBlendSteer->AddBehavior(flockSepSteer, separWeight);
	flockBlendSteer->AddBehavior(flockWanderSteer, wanderWeight);
}

FlockSteering::~FlockSteering()
{
	delete flockBlendSteer;
	delete flockAlignSteer;
	delete flockCoSteer;
	delete flockSepSteer;
	delete flockWanderSteer;

	mOutFile.open(fileName);
	mOutFile << "Alignment: " << alignWeight << std::endl;
	mOutFile << "Cohesion: " << cohesionWeight << std::endl;
	mOutFile << "Separation: " << separWeight << std::endl;
	mOutFile << "Wander: " << wanderWeight << std::endl;
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

void FlockSteering::setNewWeight(std::string steer, float newWeight)
{
	if (steer == "Cohesion")
	{
		cohesionWeight = newWeight;
	}
	else if (steer == "Alignment")
	{
		alignWeight = newWeight;
	}
	else if (steer == "Separation")
	{
		separWeight = newWeight;
	}
	else if (steer == "Wander")
	{
		wanderWeight = newWeight;
	}
}

