#pragma once

/* Game - base class for actual application class

	Dean Lawson
	Champlain College
	2011
*/
#include <Trackable.h>
#include <Timer.h>
#include "Defines.h"

class MemoryTracker;
class PerformanceTracker;
class GraphicsSystem;
class GraphicsBuffer;
class GraphicsBufferManager;
class SpriteManager;
class Game;
class Font;

extern PerformanceTracker* gpPerformanceTracker;
extern Game* gpGame;

const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType PAC_MAN_SPRITE_ID = 4;
const IDType BIG_PIP_SPRITE_ID = 5;
const IDType SMALL_PIP_SPRITE_ID = 6;
const IDType ENEMY_POWER_UP_SPRITE_ID = 7;
const IDType RED_GHOST_SPRITE = 8;
const IDType PINK_GHOST_SPRITE = 9;
const IDType ORANGE_GHOST_SPRITE = 10;
const IDType GREEN_GHOST_SPRITE = 11;
const IDType EDIBLE_GHOST_SPRITE = 12;

class Game:public Trackable
{
public:
	Game();
	virtual ~Game();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline Font* getFont() const { return mpFont; };
	int getScore() { return mpGameScore; };
	int getHighScore() { return mpHighScore; };

	void markForExit() { mShouldExit = true; };

	void addScore(int addValue) { mpGameScore += addValue; };

	bool getLostState() { return mGameOver; }
	void setLostState(bool newBool) { mGameOver = newBool; }
protected:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	float mLoopTargetTime;
	bool mShouldExit;

	//should be somewhere else
	Font* mpFont;

	GraphicsBufferID mBackgroundBufferID = "background";
	GraphicsBufferID mPlayerIconBufferID = "player";
	GraphicsBufferID mEnemyIconBufferID = "enemy";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mPacManID = "pacman";
	GraphicsBufferID mBigPipID = "bigpip";
	GraphicsBufferID mSmallPipID = "smallpip";
	GraphicsBufferID mEnemyPowerUpID = "enemypowerup";
	GraphicsBufferID mRedGhostID = "ghostred";
	GraphicsBufferID mGreenGhostID = "ghostgreen";
	GraphicsBufferID mOrangeGhostID = "ghostorange";
	GraphicsBufferID mPinkGhostID = "ghostpink";
	GraphicsBufferID mEdibleGhostID = "edibleghost";


	int mpGameScore;
	int mpHighScore;

	bool mGameOver;
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge
//float mapRotationToRange( float rotation, float low, float high );

//const double PI = std::atan(1.0)*4;//could proved useful!
