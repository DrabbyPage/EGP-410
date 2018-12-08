#include <cassert>

#include "KinematicArriveFaceSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

KinematicArriveFaceSteering::KinematicArriveFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::KIN_FACE_ARRIVE, ownerID, targetLoc, targetID),
	mKinArriveSteering(KinematicArriveSteer(ownerID, targetLoc, targetID)),
	mKinFaceSteering(KinematicFaceSteer(ownerID, targetLoc, targetID))
{

}

Steering* KinematicArriveFaceSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	mKinArriveSteering.setTargetLoc(mTargetLoc);
	mKinFaceSteering.setTargetLoc(mTargetLoc);

	Steering* arriveSteering = mKinArriveSteering.getSteering();
	Steering* faceSteering = mKinFaceSteering.getSteering();

	data.vel = arriveSteering->getData().vel;
	data.rotVel = faceSteering->getData().rotVel;

	this->mData = data;
	return this;
}