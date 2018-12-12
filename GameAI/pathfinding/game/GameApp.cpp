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
const Uint32 MAX_UNITS = 300;
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
	timer = 0;
	canDestroyEnemies = false;
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
	mpGraphicsBufferManager->loadBuffer(mPacManID, "pacman.png");
	mpGraphicsBufferManager->loadBuffer(mBigPipID, "BigPip.png");
	mpGraphicsBufferManager->loadBuffer(mSmallPipID, "SmallPip.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyPowerUpID, "EnemyPowerUp.png");
	mpGraphicsBufferManager->loadBuffer(mRedGhostID, "PacManGhostRed.png");
	mpGraphicsBufferManager->loadBuffer(mPinkGhostID, "PacManGhostPink.png");
	mpGraphicsBufferManager->loadBuffer(mOrangeGhostID, "PacManGhostOrange.png");
	mpGraphicsBufferManager->loadBuffer(mGreenGhostID, "PacManGhostGreen.png");
	mpGraphicsBufferManager->loadBuffer(mEdibleGhostID, "GhostEatable.png");

	// loading pathfinders
	//mpPathfinder = new DepthFirstPathfinder(mpGridGraph);
	//mpPathfinder = new DijkstraPath(mpGridGraph);
	mpPathfinder = new AStarPath(mpGridGraph);
	mpPathSmoothing = new PathSmoothing();

	createSprites();

	createUnits();

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpGameScore = 0;

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
	if (!gpGame->getLostState())
	{
		//copy to back buffer
		mpGridVisualizer->draw(*pBackBuffer);

#ifdef VISUALIZE_PATH
		//show pathfinder visualizer
		mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif

		mpUnitManager->updateAll(TARGET_ELAPSED_MS);
		mpUnitManager->checkCollisions();
		mpComponentManager->update(TARGET_ELAPSED_MS);
		mpMessageManager->processMessagesForThisframe();

		mpMessageManager->processMessagesForThisframe();

		mpUnitManager->drawAll();

		if (canDestroyEnemies)
		{
			for (int i = 1; i < 5; i++)
			{
				mpUnitManager->getUnit(i)->setSprite(*mpSpriteManager->getSprite(EDIBLE_GHOST_SPRITE));
			}
			timer += TARGET_ELAPSED_MS;

			float maxEatTime = 10.0f;

			if (timer > maxEatTime)
			{

				mpUnitManager->getUnit(1)->setSprite(*mpSpriteManager->getSprite(RED_GHOST_SPRITE));
				mpUnitManager->getUnit(2)->setSprite(*mpSpriteManager->getSprite(PINK_GHOST_SPRITE));
				mpUnitManager->getUnit(3)->setSprite(*mpSpriteManager->getSprite(ORANGE_GHOST_SPRITE));
				mpUnitManager->getUnit(4)->setSprite(*mpSpriteManager->getSprite(GREEN_GHOST_SPRITE));

				timer = 0;
				canDestroyEnemies = false;
			}
		}

	}


	mpDebugDisplay->draw(pBackBuffer);
	
	mpInputSystem->process();

	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}

void GameApp::createSprites()
{
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer(mBackgroundBufferID);
	if (pBackGroundBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight());
	}

	GraphicsBuffer* pPacManBuffer = mpGraphicsBufferManager->getBuffer(mPacManID);
	Sprite* pPacManSprite = NULL;
	if (pPacManBuffer != NULL)
	{
		pPacManSprite = mpSpriteManager->createAndManageSprite(PAC_MAN_SPRITE_ID, pPacManBuffer, (float)pPacManBuffer->getWidth() / 3, 0, (float)pPacManBuffer->getWidth() / 3, (float)pPacManBuffer->getHeight());
	}

	GraphicsBuffer* pBigPipBuffer = mpGraphicsBufferManager->getBuffer(mBigPipID);
	Sprite* pBigPipSprite = NULL;
	if (pBigPipBuffer != NULL)
	{
		pBigPipSprite = mpSpriteManager->createAndManageSprite(BIG_PIP_SPRITE_ID, pBigPipBuffer, 0, 0, (float)pBigPipBuffer->getWidth(), (float)pBigPipBuffer->getHeight());
	}

	GraphicsBuffer* pSmallPipBuffer = mpGraphicsBufferManager->getBuffer(mSmallPipID);
	Sprite* pSmallPipSprite = NULL;
	if (pSmallPipBuffer != NULL)
	{
		pSmallPipSprite = mpSpriteManager->createAndManageSprite(SMALL_PIP_SPRITE_ID, pSmallPipBuffer, 0, 0, (float)pSmallPipBuffer->getWidth(), (float)pSmallPipBuffer->getHeight());
	}

	GraphicsBuffer* pEnemyPowerBuffer = mpGraphicsBufferManager->getBuffer(mEnemyPowerUpID);
	Sprite* pEnemyPowerSprite = NULL;
	if (pEnemyPowerBuffer != NULL)
	{
		pEnemyPowerSprite = mpSpriteManager->createAndManageSprite(ENEMY_POWER_UP_SPRITE_ID, pEnemyPowerBuffer, 0, 0, (float)pEnemyPowerBuffer->getWidth(), (float)pEnemyPowerBuffer->getHeight());
	}

	GraphicsBuffer* pRedGhostBuffer = mpGraphicsBufferManager->getBuffer(mRedGhostID);
	Sprite* pRedGhostSprite = NULL;
	if (pRedGhostBuffer != NULL)
	{
		pRedGhostSprite = mpSpriteManager->createAndManageSprite(RED_GHOST_SPRITE, pRedGhostBuffer, 0, 0, (float)pRedGhostBuffer->getWidth(), (float)pRedGhostBuffer->getHeight());
	}

	GraphicsBuffer* pPinkGhostBuffer = mpGraphicsBufferManager->getBuffer(mPinkGhostID);
	Sprite* pPinkGhostSprite = NULL;
	if (pPinkGhostBuffer != NULL)
	{
		pPinkGhostSprite = mpSpriteManager->createAndManageSprite(PINK_GHOST_SPRITE, pPinkGhostBuffer, 0, 0, (float)pPinkGhostBuffer->getWidth(), (float)pPinkGhostBuffer->getHeight());
	}

	GraphicsBuffer* pOrangeGhostBuffer = mpGraphicsBufferManager->getBuffer(mOrangeGhostID);
	Sprite* pOrangeGhostSprite = NULL;
	if (pOrangeGhostBuffer != NULL)
	{
		pOrangeGhostSprite = mpSpriteManager->createAndManageSprite(ORANGE_GHOST_SPRITE, pOrangeGhostBuffer, 0, 0, (float)pOrangeGhostBuffer->getWidth(), (float)pOrangeGhostBuffer->getHeight());
	}

	GraphicsBuffer* pGreenGhostBuffer = mpGraphicsBufferManager->getBuffer(mGreenGhostID);
	Sprite* pGreenGhostSprite = NULL;
	if (pGreenGhostBuffer != NULL)
	{
		pGreenGhostSprite = mpSpriteManager->createAndManageSprite(GREEN_GHOST_SPRITE, pGreenGhostBuffer, 0, 0, (float)pGreenGhostBuffer->getWidth(), (float)pGreenGhostBuffer->getHeight());
	}

	GraphicsBuffer* pEdibleGhostBuffer = mpGraphicsBufferManager->getBuffer(mEdibleGhostID);
	Sprite* pEdibleGhostSprite = NULL;
	if (pEdibleGhostBuffer != NULL)
	{
		pEdibleGhostSprite = mpSpriteManager->createAndManageSprite(EDIBLE_GHOST_SPRITE, pEdibleGhostBuffer, 0, 0, (float)pEdibleGhostBuffer->getWidth(), (float)pEdibleGhostBuffer->getHeight());
	}
}

void GameApp::createUnits()
{
	// making pacman
	{
		Unit* pPacMan = mpUnitManager->createPlayerUnit(*mpSpriteManager->getSprite(PAC_MAN_SPRITE_ID), Unit::PAC_MAN);
		pPacMan->setSteering(Steering::PAC_MAN, Vector2D(512, 544));
		Steering* temp = pPacMan->getSteeringComponent()->getSteering();
		pPacMan->getPositionComponent()->setPosition(Vector2D(512, 544));
	}

	// making ghosts
	{
		Unit* pRedGhost = mpUnitManager->createUnit(*mpSpriteManager->getSprite(RED_GHOST_SPRITE), Unit::GHOST);
		pRedGhost->getPositionComponent()->setPosition(Vector2D(448, 448));
		pRedGhost->setSteering(Steering::GHOST, Vector2D(pRedGhost->getPositionComponent()->getPosition()));

		Unit* pPinkGhost = mpUnitManager->createUnit(*mpSpriteManager->getSprite(PINK_GHOST_SPRITE), Unit::GHOST);
		pPinkGhost->getPositionComponent()->setPosition(Vector2D(448 + (32), 448));
		pPinkGhost->setSteering(Steering::GHOST, Vector2D(pPinkGhost->getPositionComponent()->getPosition()));

		Unit* pOrangeGhost = mpUnitManager->createUnit(*mpSpriteManager->getSprite(ORANGE_GHOST_SPRITE), Unit::GHOST);
		pOrangeGhost->getPositionComponent()->setPosition(Vector2D(448 + (64), 448));
		pOrangeGhost->setSteering(Steering::GHOST, Vector2D(pOrangeGhost->getPositionComponent()->getPosition()));

		Unit* pGreenGhost = mpUnitManager->createUnit(*mpSpriteManager->getSprite(GREEN_GHOST_SPRITE), Unit::GHOST);
		pGreenGhost->getPositionComponent()->setPosition(Vector2D(448 + (64+32), 448));
		pGreenGhost->setSteering(Steering::GHOST, Vector2D(pGreenGhost->getPositionComponent()->getPosition()));
	}

	// creating enemy power up
	Unit* pEnemyPower = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(ENEMY_POWER_UP_SPRITE_ID));
	pEnemyPower->setTag(Unit::ENEMY_POWER_UP);

	// creating big pips
	{
		Unit* pBigPipUpLeft = mpUnitManager->createUnit(*mpSpriteManager->getSprite(BIG_PIP_SPRITE_ID), Unit::BIG_PIP);
		pBigPipUpLeft->getPositionComponent()->setPosition(Vector2D(96, 32));
		pBigPipUpLeft->setTag(Unit::BIG_PIP);

		Unit* pBigPipUpRight = mpUnitManager->createUnit(*mpSpriteManager->getSprite(BIG_PIP_SPRITE_ID), Unit::BIG_PIP);
		pBigPipUpRight->getPositionComponent()->setPosition(Vector2D(896, 32));
		pBigPipUpRight->setTag(Unit::BIG_PIP);

		Unit* pBigPipDownLeft = mpUnitManager->createUnit(*mpSpriteManager->getSprite(BIG_PIP_SPRITE_ID), Unit::BIG_PIP);
		pBigPipDownLeft->getPositionComponent()->setPosition(Vector2D(96, 704));
		pBigPipDownLeft->setTag(Unit::BIG_PIP);

		Unit* pBigPipDownRight = mpUnitManager->createUnit(*mpSpriteManager->getSprite(BIG_PIP_SPRITE_ID), Unit::BIG_PIP);
		pBigPipDownRight->getPositionComponent()->setPosition(Vector2D(896, 704));
		pBigPipDownRight->setTag(Unit::BIG_PIP);
	}

	// creating small pips
	for (int i = 0; i < 150; i++)
	{
		Unit* pSmallPip = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(SMALL_PIP_SPRITE_ID));
		pSmallPip->setTag(Unit::SMALL_PIP);
	}

}

