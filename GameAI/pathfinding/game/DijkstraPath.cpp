
#include "DijkstraPath.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "GridPathfinder.h"

DijkstraPath::DijkstraPath(Graph* pGraph):GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{

}

DijkstraPath::~DijkstraPath()
{

}

Path* DijkstraPath::findPath(Node* pFrom, Node* pTo)
{
	//make sure to delete the path when you are done!

}
