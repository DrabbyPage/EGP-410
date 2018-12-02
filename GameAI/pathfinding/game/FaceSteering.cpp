#include <cassert>

#include "FaceSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

const float FaceSteering::msTARGET_RADIUS = 1.0f;

FaceSteering::FaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::FACE, ownerID, targetLoc, targetID),
	mAlignSteering(AlignSteering(ownerID, targetLoc, targetID))
{

}

Steering* FaceSteering::getSteering()
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
		mAlignSteering.setTargetAngle(atan2(direction.getY(), direction.getX()) - pOwner->getFacing());
		return mAlignSteering.getSteering();
	}
}
