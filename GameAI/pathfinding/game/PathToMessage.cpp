#include "PathToMessage.h"
#include "Game.h"
#include "GameApp.h"
#include "GridPathfinder.h"
#include "Grid.h"
#include "GridGraph.h"
#include "PathSmoothing.h"
#include "PathSteering.h"
#include "Unit.h"
#include "UnitManager.h"
#include "SteeringComponent.h"
#include "PacManSteering.h"

PathToMessage::PathToMessage( const Vector2D& from, const Vector2D& to )
:GameMessage(PATH_TO_MESSAGE)
,mFrom(from)
,mTo(to)
{
}

PathToMessage::~PathToMessage()
{
}

void PathToMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	if (pGame != NULL)
	{
		GridPathfinder* pPathfinder = pGame->getPathfinder();
		GridGraph* pGridGraph = pGame->getGridGraph();
		Grid* pGrid = pGame->getGrid();
		int toIndex = pGrid->getSquareIndexFromPixelXY((int)mTo.getX(), (int)mTo.getY());
		Node* pToNode = pGridGraph->getNode(toIndex);

		Unit* player = pGame->getUnitManager()->getPlayerUnit();
		Vector2D unitPosition = player->getPositionComponent()->getPosition();
		int fromIndex = pGrid->getSquareIndexFromPixelXY(static_cast<int>(unitPosition.getX()), static_cast<int>(unitPosition.getY()));
		Node* pFromNode = pGridGraph->getNode(fromIndex);
		pPathfinder->findPath(pFromNode, pToNode);

		PacManSteering* pathSteering = static_cast<PacManSteering*>(player->getSteeringComponent()->getSteering());
		PathSmoothing* pPathSmoothing = pGame->getPathSmoothing();


		Path* path = pPathfinder->getPath();

		if (!path)
		{
			path = pPathfinder->findPath(pFromNode, pToNode);
		}

		if (path)
		{
			Path unitPath = *pPathfinder->getPath();//*pPathSmoothing->smoothPath(path);// uncomment to smooth path
			pathSteering->setPath(unitPath);
		}
	}
}
