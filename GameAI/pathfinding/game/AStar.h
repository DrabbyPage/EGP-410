
#ifndef ASTAR_H
#define ASTAR_H

#include "GridPathfinder.h"
#include <vector>

#include"Path.h"
#include "Graph.h"
#include "Grid.h"
#include "Connection.h"
#include "Heuristic.h"

class AStarPath : public GridPathfinder
{
public:
	AStarPath(Graph* pGraph);
	~AStarPath();

	Path* findPath(Node* start, Node* end);//make sure to delete the path when you are done!

private:

};

struct StarNodeRecord
{
	StarNodeRecord()
	{
		node = nullptr;
		costSoFar = 0;
		estiTotalCost = 0;
		recordConnection = nullptr;
	}

	Node* node;
	Connection* recordConnection;
	float costSoFar;
	float estiTotalCost;
};

#endif