#include <cassert>

#include "PacManSteering.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"
#include "GameApp.h"
#include "GridPathfinder.h"
#include "GraphicsSystem.h"
#include "Grid.h"
#include "Node.h"
#include "PathSmoothing.h"

PacManSteering::PacManSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID) :
	Steering(Steering::PAC_MAN, ownerID, targetLoc, targetID),
	mKinArriveFaceSteer(KinematicArriveFaceSteering(ownerID, targetLoc, targetID)),
	targetPathIndex(0)
{

}

PacManSteering::~PacManSteering()
{
}

Steering* PacManSteering::getSteering()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	Unit* pOwner = pGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	GridPathfinder* pPathfinder = pGame->getPathfinder();

	if (mPath.getNumNodes() > 0)
	{
		pGame->getPathfinder()->drawVisualization(pGame->getGrid(), pGame->getGraphicsSystem()->getBackBuffer(), mPath);

		Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
		float targetRadiusSquared = 60.0f;

		if (direction.getLengthSquared() < targetRadiusSquared && mPath.peekNode(targetPathIndex) != nullptr)
		{
			Vector2D target = pGrid->getULCornerOfSquare(mPath.peekNode(targetPathIndex)->getId());
			setTargetLoc(target);
			mKinArriveFaceSteer.setTargetLoc(target);
			++targetPathIndex;
		}
	}

	Steering* steering = mKinArriveFaceSteer.getSteering();

	data.acc = steering->getData().acc;
	data.vel = steering->getData().vel;
	data.rotVel = steering->getData().rotVel;

	this->mData = data;
	return this;
}
