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
#include "UnitManager.h"
#include "Unit.h"
#include "ComponentManager.h"

#include <SDL.h>
#include <fstream>
#include <vector>

#include "DijkstraPath.h"
#include "AStar.h"
#include "PathSmoothing.h"

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";
const Uint32 MAX_UNITS = 100;
const Uint32 UNIT_COUNT = 5;

GameApp::GameApp() :
	mpInputSystem(nullptr),
	mpComponentManager(nullptr),
	mpUnitManager(nullptr),
	mpMessageManager(nullptr),
	mpGrid(nullptr),
	mpPathfinder(nullptr),
	mpGridGraph(nullptr),
	mpDebugDisplay(nullptr),
	mpPathSmoothing(nullptr)
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

	// loading managers
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, "arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, "PacManGhost.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID, "target.png");
	mpGraphicsBufferManager->loadBuffer(mPacManID, "pacman.png");

	// loading pathfinders
	//mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
	//mpPathfinder = new DijkstraPath(mpGridGraph);
	mpPathfinder = new AStarPath(mpGridGraph);
	mpPathSmoothing = new PathSmoothing();


	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPlayerIconBufferID);
	Sprite* pArrowSprite = NULL;

	if (pPlayerBuffer != NULL)
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight());
	}

	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;

	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);

	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	GraphicsBuffer* pPacManBuffer = mpGraphicsBufferManager->getBuffer(mPacManID);
	Sprite* pPacManSprite = NULL;
	if (pPacManBuffer != NULL)
	{
		pPacManSprite = mpSpriteManager->createAndManageSprite(PAC_MAN_SPRITE_ID, pPacManBuffer, (float)pPacManBuffer->getWidth() / 3, 0, (float)pPacManBuffer->getWidth() / 3, (float)pPacManBuffer->getHeight());
	}

	Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(PAC_MAN_SPRITE_ID));
	pUnit->setSteering(Steering::PATH, Vector2D(pUnit->getPositionComponent()->getPosition()));

	if (pUnit == nullptr)
	{
		mpUnitManager->deleteRandomUnit();
	}

	for (int i = 0; i < UNIT_COUNT; ++i)
	{
		Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
		pUnit->setSteering(Steering::PATH, Vector2D(pUnit->getPositionComponent()->getPosition()));

		if (pUnit == nullptr)
		{
			mpUnitManager->deleteRandomUnit();
		}
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpInputSystem;
	mpInputSystem = nullptr;

	delete mpMessageManager;
	mpMessageManager = nullptr;

	delete mpUnitManager;
	mpUnitManager = nullptr;

	delete mpGrid;
	mpGrid = nullptr;

	delete mpGridVisualizer;
	mpGridVisualizer = nullptr;

	delete mpGridGraph;
	mpGridGraph = nullptr;

	delete mpDebugDisplay;
	mpDebugDisplay = nullptr;

	delete mpPathfinder;
	mpPathfinder = nullptr;

	delete mpComponentManager;
	mpComponentManager = nullptr;

	delete mpPathSmoothing;
	mpPathSmoothing = nullptr;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;

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

	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	mpMessageManager->processMessagesForThisframe();

	mpMessageManager->processMessagesForThisframe();

	mpUnitManager->drawAll();
	
	mpInputSystem->process();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}
