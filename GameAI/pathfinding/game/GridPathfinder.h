#pragma once

#include "Pathfinder.h"
#include "Path.h"
class GridGraph;
class GridVisualizer;
class GraphicsBuffer;
class PathfindingDebugContent;

class GridPathfinder:public Pathfinder
{
public:
	friend class PathfindingDebugContent;

	GridPathfinder( GridGraph* pGraph );
	virtual ~GridPathfinder();

	virtual Path* findPath( Node* pFrom, Node* pTo ) = 0;
	Path* getPath() { return mpPath; };
	void setPath(Path* newPath)
	{
		delete mpPath;
		mpPath = nullptr;
		mpPath = newPath;
	}
	
#ifdef VISUALIZE_PATH
	//just for visualization
public:
	void drawVisualization( Grid* pGrid, GraphicsBuffer* pDest );
	void drawVisualization(Grid* pGrid, GraphicsBuffer* pDest, Path pPath);

protected:
	std::vector<Node*> mVisitedNodes;
	Path* mpPath;
	GridVisualizer* mpVisualizer;
#endif

	double mTimeElapsed;
};