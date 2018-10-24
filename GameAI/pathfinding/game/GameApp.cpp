#include "Game.h"
#include "GameApp.h"
#include "GameMessageManager.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "InputSystem.h"

#include <SDL.h>
#include <fstream>
#include <vector>

#include "DijkstraPath.h"
#include "AStar.h"

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";

GameApp::GameApp()
:mpMessageManager(NULL)
,mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}

	mpMessageManager = new GameMessageManager();

	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();

	//mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
	//mpPathfinder = new DijkstraPath(mpGridGraph);
	mpPathfinder = new AStarPath(mpGridGraph);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;

	delete mpPathfinder;
	mpPathfinder = NULL;

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	mpGridVisualizer->draw( *pBackBuffer );

	#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
	#endif

	mpDebugDisplay->draw( pBackBuffer );

	mpMessageManager->processMessagesForThisframe();
	
	mpInputSystem->process();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::LookForPathChange(char charInput)
{
	switch (charInput)
	{
		PathfindingDebugContent* pContent;
	case 'A':

	case 'a':
		std::cout << "changing to a*" << std::endl;
		delete mpPathfinder;
		delete mpDebugDisplay;

		mpPathfinder = new AStarPath(mpGridGraph);

		//debug display
		pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

		break;

	case 'D':

	case 'd':
		std::cout << "changing to depth first search" << std::endl;
		delete mpPathfinder;
		delete mpDebugDisplay;

		mpPathfinder = new DepthFirstPathfinder(mpGridGraph);

		//debug display
		pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

		break;

	case 'F':

	case 'f':
		std::cout << "changing to dijkstra" << std::endl;
		delete mpPathfinder;
		delete mpDebugDisplay;

		mpPathfinder = new DijkstraPath(mpGridGraph);

		//debug display
		pContent = new PathfindingDebugContent(mpPathfinder);
		mpDebugDisplay = new DebugDisplay(Vector2D(0, 12), pContent);

		break;

	default:
		break;
	}
}
