#include "Game.h"
#include "GhostChaseState.h"
#include <iostream>
#include "GameApp.h"
#include "Grid.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "Path.h"
#include "GhostSteering.h"
#include "Steering.h"
#include "SteeringComponent.h"

using namespace std;

void GhostChaseState::onEntrance()
{
	cout << "\nEntering Ghost Chase State id:" << mID << endl;
	mGhostXDist = 0;
	mGhostYDist = 0;
	mGhostXDir = Vector2D(0, 0);
	mGhostYDir = Vector2D(0, 0);
	mPathIndex = 1;
	//mID = 1;
	timer = 0;
	mGhostDir = make_pair(Vector2D(0, 0), Vector2D(0, 0));

	//cout << "\nEntering GhostChaseState id:" << mID << endl;
	mTransitionToWander = false;
	mTransitionToIdle = false;
	mTransitionToFlee = false;
}

void GhostChaseState::onExit()
{
	//cout << "\nexitting GhostChaseState id:" << mID << endl;
	mTransitionToWander = false;
	mTransitionToIdle = false;
	mTransitionToFlee = false;
}

StateTransition* GhostChaseState::update()
{
	//cout << "GhostChaseState!" << endl;

	//find the right transition
	if (mTransitionToWander)
	{
		mTransitionToWander = false;
		map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_CHASE);
		if (iter != mTransitions.end())//found?
		{
			StateTransition* pTransition = iter->second;
			return pTransition;
		}
	}
	if (mTransitionToFlee)
	{
		mTransitionToFlee = false;
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

		if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE || mpPath == nullptr)
		{
			/*THIS NEEDS TO pathfind peemans location ONLY at intersections*/
		}

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
		//needs to be at an intersection in order to change to chase

		if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE)
		{
			if (abs(GhostPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) >= 60
				&& abs(GhostPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) >= 60)
			{
				map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_WANDER);
				if (iter != mTransitions.end())//found?
				{
					StateTransition* pTransition = iter->second;
					return pTransition;
				}
			}
		}

		if (pGrid->getValueAtIndex(fromIndex) == SPAWNING_VALUE && timer > 60)
		{
			map<TransitionType, StateTransition*>::iterator iter = mTransitions.find(GHOST_IDLE);
			if (iter != mTransitions.end())//found?
			{
				StateTransition* pTransition = iter->second;
				return pTransition;
			}
		}

		return NULL;//no transition
	}


	return NULL;//no transition
}