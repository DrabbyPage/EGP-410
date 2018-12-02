#include <cassert>

#include "ArriveAndFaceSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::ARRIVE_AND_FACE, ownerID, targetLoc, targetID),
	mArriveSteering(ArriveSteering(ownerID, targetLoc, targetID)),
	mFaceSteering(FaceSteering(ownerID, targetLoc, targetID))
{

}

Steering* ArriveAndFaceSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mArriveSteering.setTargetLoc(mTargetLoc);
	mFaceSteering.setTargetLoc(mTargetLoc);

	Steering* arriveSteering = mArriveSteering.getSteering();
	Steering* faceSteering = mFaceSteering.getSteering();

	data.acc = arriveSteering->getData().acc;
	data.rotAcc = faceSteering->getData().rotAcc;

	this->mData = data;
	return this;
}