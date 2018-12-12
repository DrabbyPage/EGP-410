#include "Game.h"
#include "GhostFleeState.h"
#include <iostream>
#include <Vector2D.h>
#include "GhostFleeState.h"
#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "GhostSteering.h"
#include "SteeringComponent.h"

using namespace std;

void GhostFleeState::onEntrance()
{
	cout << "\nEntering Ghost Flee State id:" << mID << endl;
	mGhostXDist = 0;
	mGhostYDist = 0;
	mGhostXDir = Vector2D(0, 0);
	mGhostYDir = Vector2D(0, 0);
	mPathIndex = 1;
	//mID = 1;
	timer = 0;
	mGhostDir = make_pair(Vector2D(0, 0), Vector2D(0, 0));
	//cout << "\nEntering GhostFleeState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToIdle = false;
	mTransitionToWander = false;
}

void GhostFleeState::onExit()
{
	//cout << "\nexitting GhostFleeState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToIdle = false;
	mTransitionToWander = false;
}

StateTransition* GhostFleeState::update()
{
	//cout << "GhostFleeState!" << endl;

	//find the right transition
	if (mTransitionToChase)
	{
		mTransitionToChase = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_CHASE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToWander)
	{
		mTransitionToWander = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_FLEE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToIdle)
	{
		mTransitionToIdle = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_IDLE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	else
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		GridPathfinder* pPathfinder = pGame->getPathfinder();
		int i = mID;
		timer++;
		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		//get the from and to index from the grid
		Vector2D GhostPosCenter = (pGame->getUnitManager()->getUnit(i)->getPositionComponent()->getPosition() + Vector2D(16, 16));

		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY());
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX(), (int)pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY());
		/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/

		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);
		mpPath = pPathfinder->findPath(pFromNode, pToNode);

		GhostSteering* pGhostSteer = dynamic_cast<GhostSteering*>(pGame->getUnitManager()->getUnit(i)->getSteeringComponent()->getSteering());

		/*find direction to next node. use that to go in*/
		if (mpPath->peekNode(mPathIndex) != NULL)
		{
			mGhostXDist = (int)pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getX() - (int)pGrid->getULCornerOfSquare(fromIndex).getX();
			mGhostYDist = (int)pGrid->getULCornerOfSquare(mpPath->peekNode(1)->getId()).getY() - (int)pGrid->getULCornerOfSquare(fromIndex).getY();
		}


		int x1, x2, y1, y2 = 0;

		x1 = mGhostXDist / -32;
		x2 = mGhostXDist / 32;
		y1 = mGhostYDist / -32;
		y2 = mGhostYDist / 32;

		if (x1 < 0)
		{
			x1 = 0;
		}
		if (x2 < 0)
		{
			x2 = 0;
		}
		if (y1 < 0)
		{
			y1 = 0;
		}
		if (y2 < 0)
		{
			y2 = 0;
		}

		mGhostDir = make_pair(Vector2D(x1, x2), Vector2D(y1, y2));
		mGhostXDir = mGhostDir.first;
		mGhostYDir = mGhostDir.second;


		pGhostSteer->moveDirection(mGhostXDir, mGhostYDir);

		pGhostSteer->resetIndex();
		pGhostSteer->setPath(mpPath);
		//reset the index every click

		if (!pGame->getCanDestroyEnemies())
		{
			mTransitionToWander = true;
		}

	}


	return NULL;//no transition
}