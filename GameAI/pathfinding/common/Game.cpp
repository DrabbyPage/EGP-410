
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Font.h"
#include <Timer.h>
#include "Defines.h"

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mShouldExit(false)
	,mpFont(NULL)
	,mLoopTargetTime(0.0f)
{
	mGameOver = false;
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	srand((UINT)time(0));//seed random number generator

	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;


	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init(WIDTH, HEIGHT);
	if (!goodGraphics)
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();

	
	//load background
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");

	//load Font
	mpFont = new Font("cour.ttf", 24);

	// game score is 0 at start
	mpGameScore = 0;

	// loading the high score
	std::ifstream inputFile;
	std::string highScoreString;

	inputFile.open("HighScore.txt");
	inputFile >> highScoreString;
	inputFile.close();

	int newHighScore = std::stoi(highScoreString);
	mpHighScore = newHighScore;

	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	//cleanup font
	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
}

void Game::beginLoop()
{
	mpLoopTimer->start();

	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	mpGraphicsSystem->draw(*pBackgroundSprite, 0.0f, 0.0f);
}

void Game::processLoop()
{
	mpGraphicsSystem->swap();
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( mLoopTargetTime );
	if (mShouldExit == true)
	{
		if (mpGameScore > mpHighScore)
		{
			mpHighScore = mpGameScore;
			std::ofstream outputFile;
			outputFile.open("HighScore.txt");
			outputFile << mpHighScore;
			outputFile.close();
		}
	}
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}
