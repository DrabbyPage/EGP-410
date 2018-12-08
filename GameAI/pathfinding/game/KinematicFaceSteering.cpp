#include <cassert>

#include "KinematicFaceSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float KinematicFaceSteer::msTARGET_RADIUS = 1.0f;

KinematicFaceSteer::KinematicFaceSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::KIN_FACE, ownerID, targetLoc, targetID),
	mKinAlignSteering(KinematicAlignSteer(ownerID, targetLoc, targetID))
{

}

Steering* KinematicFaceSteer::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	
	if (direction.getLengthSquared() < msTARGET_RADIUS * msTARGET_RADIUS)
	{
		PhysicsData data = pOwner->getPhysicsComponent()->getData();

		//We are done! Return no steering.
		data.rotAcc = 0;
		data.rotVel = 0;
		this->mData = data;
		return this;
	}
	else
	{
		pOwner->getPositionComponent()->setFacing(atan2(direction.getY(), direction.getX()));

		mKinAlignSteering.setTargetAngle(atan2(direction.getY(), direction.getX()) - pOwner->getFacing());
		return mKinAlignSteering.getSteering();
	}
	
}
