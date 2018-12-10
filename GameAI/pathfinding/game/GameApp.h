#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"

//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class GameMessageManager;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;

class InputSystem;

class UnitManager;
class ComponentManager;
class PathSmoothing;
//class AStarPath;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame


class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	void createSprites();
	void createUnits();

	//accessors
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };

	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };

	inline PathSmoothing* getPathSmoothing() { return mpPathSmoothing; };

private:
	GameMessageManager* mpMessageManager;
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	DebugDisplay* mpDebugDisplay;

	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;

	PathSmoothing* mpPathSmoothing;
	GridPathfinder* mpPathfinder;

	InputSystem* mpInputSystem;
};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge
//float mapRotationToRange(float rotation, float low, float high);


