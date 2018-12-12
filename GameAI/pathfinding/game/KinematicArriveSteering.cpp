#include <cassert>

#include "KinematicArriveSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

#include <fstream>
#include <string>

const float KinematicArriveSteer::msTARGET_RADIUS = 2.0f;
const float KinematicArriveSteer::msSLOW_RADIUS = 300.0f;
const float KinematicArriveSteer::msTIME_TO_TARGET = 0.1f;

KinematicArriveSteer::KinematicArriveSteer(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::ARRIVE, ownerID, targetLoc, targetID)
{

}

Steering* KinematicArriveSteer::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Vector2D direction;
	float distance;
	float targetSpeed;
	Vector2D targetVelocity;

	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	if (mTargetID != INVALID_UNIT_ID)
	{
		//arriving unit
		Unit* pTarget = pGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	distance = direction.getLength();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	targetSpeed = pOwner->getMaxSpeed();

	std::ifstream characterSpeed;

	if (pOwner->getTag() == Unit::GHOST)
	{
		characterSpeed.open("GhostSpeed.txt");
		std::string charSpeedString;
		characterSpeed >> charSpeedString;
		targetSpeed = std::stof(charSpeedString);
		characterSpeed.close();
	}
	else if (pOwner->getTag() == Unit::PAC_MAN)
	{
		characterSpeed.open("PacManSpeed.txt");
		std::string charSpeedString;
		characterSpeed >> charSpeedString;
		targetSpeed = std::stof(charSpeedString);
		characterSpeed.close();
	}
	else
	{
		targetSpeed = 40;
	}

	if (distance > msTARGET_RADIUS)
	{
		targetVelocity = direction;
		targetVelocity.normalize();
		targetVelocity *= targetSpeed;

		data.vel = targetVelocity;
		data.rotVel = 0;
	}
	else
	{
		data.vel = 0;
		data.rotVel = 0;
	}


	this->mData = data;
	return this;
}