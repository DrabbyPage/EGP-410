
#ifndef ASTAR_H
#define ASTAR_H

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;
class Heuristic;

class AStarPath : public GridPathfinder
{
public:
	AStarPath(Graph* pGraph);
	~AStarPath();

	Path* findPath(Node* start, Node* end, Heuristic* heur);//make sure to delete the path when you are done!

private:

};

struct NodeRecord
{
	NodeRecord(Node* newNode, Connection* newConnections, float newCost, float newEstiCost)
	{
		node = newNode;
		costSoFar = newCost;
		estiTotalCost = newEstiCost;
		nodeRecordConnections = newConnections;
	}

	NodeRecord()
	{
		node = nullptr;
		costSoFar = 0;
		estiTotalCost = 0;
		nodeRecordConnections = nullptr;
	}

	Node* node;
	Connection* nodeRecordConnections;
	float costSoFar;
	float estiTotalCost;
};

#endif