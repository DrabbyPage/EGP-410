#include "ComponentManager.h"
#include "SteeringComponent.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "FaceSteering.h"
#include "ArriveAndFaceSteering.h"
#include "PathSteering.h"
#include "PacManSteering.h"
#include "GhostSteering.h"

SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) :
	Component(id),
	mPhysicsComponentID(physicsComponentID),
	mpSteering(NULL)
{

}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void SteeringComponent::setData(const SteeringData& data)
{
	mData = data;

	switch (data.type)
	{
	case Steering::SEEK:
	{
		delete mpSteering;
		mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, false);
		break;
	}
	case Steering::FLEE:
	{
		delete mpSteering;
		mpSteering = new SeekSteering(data.ownerID, data.targetLoc, data.targetID, true);
		break;
	}
	case Steering::ARRIVE:
	{
		delete mpSteering;
		mpSteering = new ArriveSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::ALIGN:
	{
		delete mpSteering;
		mpSteering = new AlignSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::FACE:
	{
		delete mpSteering;
		mpSteering = new FaceSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::ARRIVE_AND_FACE:
	{
		delete mpSteering;
		mpSteering = new ArriveAndFaceSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::PATH:
	{
		delete mpSteering;
		mpSteering = new PathSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::PAC_MAN:
	{
		delete mpSteering;
		mpSteering = new PacManSteering(data.ownerID, data.targetLoc, data.targetID);
		break;
	}
	case Steering::GHOST:
	{
		delete mpSteering;
		//mpSteering = new GhostSteering(data.ownerID, data.targetLoc, data.targetID);
		mpSteering = new GhostSteering(data.ownerID, data.targetLoc, data.targetID, false);

		break;
	}
	default:
	{
		//std::cout << "Unknown steering component!" << std::endl;
		break;
	}
	};
}

