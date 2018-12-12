#include "GhostSteering.h"
#include "ArriveAndFaceSteering.h"
#include "Graph.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "GraphicsSystem.h"

GhostSteering::GhostSteering(const UnitID & ownerID, const Vector2D & targetLoc, const UnitID & targetID, bool shouldFlee)
	: Steering(Steering::PAC_MAN, ownerID, targetLoc, targetID)//, mPlayerSteering(Player(ownerID, targetLoc, targetID, shouldFlee))
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::GHOST;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);

	isUnitDown = false;
	isUnitRight = false;
	isUnitLeft = false;
	isUnitUp = false;

	std::ifstream characterSpeed;
	characterSpeed.open("GhostSpeed.txt");
	std::string charSpeedString;
	characterSpeed >> charSpeedString;
	mSpeed = std::stof(charSpeedString);
	characterSpeed.close();

	mSpeedX = mSpeedY = 0;

	mIndex = 0;
}

void GhostSteering::setPath(Path* myPath)
{
	mPath = *myPath;
}

void GhostSteering::resetIndex()
{
	mIndex = 0;
}


Steering * GhostSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();

	std::ifstream characterSpeed;
	characterSpeed.open("GhostSpeed.txt");
	std::string charSpeedString;
	characterSpeed >> charSpeedString;
	mSpeed = std::stof(charSpeedString);
	characterSpeed.close();

	//get the target location for each of the nodes
	if (mPath.peekNode(mIndex) != NULL)
	{
		//get the grid and set the target to arrive and face so that it can move to it
		Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(mIndex)->getId());

		setTargetLoc(target);
		//increase the index for the next node
		mIndex++;
	}

	//now do stuff
	checkDirection(pOwner);

	mPositionCentered = pOwner->getPositionComponent()->getPosition() + Vector2D(16, 16);

	//As long as we havent gotten to the end node

	float xVal = ((16 * mSpeedX) / mSpeed) + mSpeedX;
	float yVal = ((16 * mSpeedY) / mSpeed) + mSpeedY;

	mSquareIndexAtCenter = pGrid->getSquareIndexFromPixelXY((int)mPositionCentered.getX(), (int)mPositionCentered.getY());
	mSquareIndexShifted = pGrid->getSquareIndexFromPixelXY((int)mPositionCentered.getX() + (int)xVal, (int)mPositionCentered.getY() + (int)yVal);
	if (pGrid->getValueAtIndex(mSquareIndexShifted) == BLOCKING_VALUE)
	{
		pOwner->getPositionComponent()->setPosition(mTargetLoc);
	}

	if (newDir != oldDir)
	{
		centerToNode();
	}

	oldDir = newDir;

	this->mData = data;

	return this;
}

void GhostSteering::checkDirection(Unit* owner)
{
	if (isUnitLeft)
	{
		mSpeedX = -mSpeed;
		mSpeedY = 0;
		newDir = "left";
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitRight)
	{
		mSpeedX = mSpeed;
		mSpeedY = 0;
		newDir = "right";
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitUp)
	{
		mSpeedX = 0;
		mSpeedY = -mSpeed;
		newDir = "up";
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
	else if (isUnitDown)
	{
		mSpeedX = 0;
		mSpeedY = mSpeed;
		newDir = "down";
		owner->getPositionComponent()->setPosition(owner->getPositionComponent()->getPosition() + Vector2D(mSpeedX, mSpeedY));
	}
}

void GhostSteering::moveDirection(Vector2D directionX, Vector2D directionY)
{
	isUnitLeft = directionX.getX();
	isUnitRight = directionX.getY();
	isUnitUp = directionY.getX();
	isUnitDown = directionY.getY();
}

void GhostSteering::centerToNode()
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);

	mPositionCentered = pOwner->getPositionComponent()->getPosition() + Vector2D(16, 16);

	mSquareIndexAtCenter = pGrid->getSquareIndexFromPixelXY((int)mPositionCentered.getX(), (int)mPositionCentered.getY());
	if (pGrid->getValueAtIndex(mSquareIndexAtCenter) == INTERSECTION_VALUE)
	{
		pOwner->getPositionComponent()->setPosition(pGrid->getULCornerOfSquare(mSquareIndexAtCenter));
	}
}