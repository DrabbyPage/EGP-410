#include "GhostIdleState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "GhostSteering.h"
#include <stdlib.h>     
#include <time.h>       
#include "SteeringComponent.h"

void GhostIdleState::onEntrance()
{
	cout << "\nEntering Ghost idle State id:" << mID << endl;
	mGhostXDist = 0;
	mGhostYDist = -32;
	mGhostXDir = Vector2D(0, 0);
	mGhostYDir = Vector2D(1, 0);
	//mID = 1;
	timer = 0;
	mGhostDir = make_pair(Vector2D(0, 0), Vector2D(1, 0));
}

void GhostIdleState::onExit()
{
	cout << "\nExiting Ghost idle State id:" << mID << endl;
}

StateTransition * GhostIdleState::update()
{
	timer++;
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	GridPathfinder* pPathfinder = pGame->getPathfinder();
	int i = mID;

	GridGraph* pGridGraph = pGame->getGridGraph();
	Grid* pGrid = pGame->getGrid();
	//get the from and to index from the grid
	Vector2D GhostPosCenter = (pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));

	int fromIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY());
	int toIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY());
	/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/


	//check within radius of player and take damage if you are
	if (abs(GhostPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < 60
		&& abs(GhostPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < 60)
	{
		if (timer > 20)
		{
			pGame->getUnitManager()->getPlayerUnit()->hurtUnit(1);
			timer = 0;
		}

	}
	if (pGame->getCanDestroyEnemies())
	{
		//transition back to flee
	}

	//IF PLAYER IS OUTSIDE OF RADIUS
	//when pacman is within a certain radius of ghost
	//if on spawn point
	if (timer > 50 && pGrid->getValueAtIndex(fromIndex) == SPAWNING_VALUE)
	{
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_WANDER);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}


	return NULL;//no transition
}