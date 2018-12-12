#include "Game.h"
#include "GhostWanderState.h"
#include <iostream>

#include "GridPathfinder.h"
#include "UnitManager.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Unit.h"
#include "GhostSteering.h"
#include <stdlib.h>     
#include <time.h>       
#include "SteeringComponent.h"

using namespace std;

void GhostWanderState::onEntrance()
{
	mGhostXDist = 0;
	mGhostYDist = 0;
	mGhostXDir = Vector2D(0, 0);
	mGhostYDir = Vector2D(0, 0);
	//mID = 1;
	timer = 0;
	mGhostDir = make_pair(Vector2D(0, 0), Vector2D(0, 0));
	//cout << "\nEntering GhostWanderState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToIdle = false;
	mTransitionToFlee = false;
}

void GhostWanderState::onExit()
{
	//cout << "\nexitting GhostWanderState id:" << mID << endl;
	mTransitionToChase = false;
	mTransitionToIdle = false;
	mTransitionToFlee = false;
}

StateTransition* GhostWanderState::update()
{
	//cout << "GhostWanderState!" << endl;

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
		srand((unsigned int)time(NULL));
		/*set Ghost distances*/
		timer++;
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		GridPathfinder* pPathfinder = pGame->getPathfinder();
		int unitID = mID;

		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		
		//get the from and to index from the grid
		Vector2D GhostPosCenter = (pGame->getUnitManager()->getUnit(unitID)->getPositionComponent()->getPosition() + Vector2D(16, 16));

		vector<int> adjacentIndices;
		adjacentIndices = pGrid->getAdjacentIndices(pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY()));

		int fromIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY());

		/*if we are on a INTERSECTION and it has 2+ connections, pick one, and proceed along that direction*/

		if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE)
		{
			int directionIndex = rand() % adjacentIndices.size() / 2;
			directionIndex *= 2;

			while (pGrid->getValueAtIndex(adjacentIndices[directionIndex]) == BLOCKING_VALUE || adjacentIndices[directionIndex] == oldIndex)
			{
				directionIndex = rand() % adjacentIndices.size() / 2;
				directionIndex *= 2;
			}

			Vector2D directionPos = pGrid->getULCornerOfSquare(adjacentIndices[directionIndex]) + Vector2D(16, 16);
			Vector2D fromPos = pGrid->getULCornerOfSquare(fromIndex) + Vector2D(16, 16);

			mGhostXDist = (int)(directionPos.getX() - fromPos.getX());
			mGhostYDist = (int)(directionPos.getY() - fromPos.getY());

			int x1, x2, y1, y2;

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
		}
		else
		{
			oldIndex = fromIndex;
		}
		
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX() + mGhostXDist, (int)GhostPosCenter.getY() + mGhostYDist);
		
		//If we're going into a wall, stop where you are
		if (pGrid->getValueAtIndex(toIndex) == BLOCKING_VALUE)
		{
			toIndex = pGrid->getSquareIndexFromPixelXY((int)GhostPosCenter.getX(), (int)GhostPosCenter.getY());
		}

		Node* pFromNode = pGridGraph->getNode(fromIndex);
		Node* pToNode = pGridGraph->getNode(toIndex);

		GhostSteering* pGhostSteer = dynamic_cast<GhostSteering*>(pGame->getUnitManager()->getUnit(unitID)->getSteeringComponent()->getSteering());
		pGhostSteer->moveDirection(mGhostXDir, mGhostYDir);

		Path* newPath = pPathfinder->findPath(pFromNode, pToNode);

		//reset the index every click
		pGhostSteer->resetIndex();
		pGhostSteer->setPath(newPath);

		//when pacman is within a certain radius of ghost
		//needs to be at an intersection in order to change to chase
		
		if (pGrid->getValueAtIndex(fromIndex) == INTERSECTION_VALUE)
		{
			if (abs(GhostPosCenter.getX() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX()) < 60
				&& abs(GhostPosCenter.getY() - pGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()) < 60)
			{
				mTransitionToChase = true;
			}
		}
		
		//means the Ghost is allowed to hurt the Ghost
		
		if (pGame->getCanDestroyEnemies())
		{
			//transition back to flee
			mTransitionToFlee = true;
		}
		if (pGrid->getValueAtIndex(fromIndex) == SPAWNING_VALUE && timer > 60)
		{
			mTransitionToIdle = true;
		}
		return NULL;
	}	
}