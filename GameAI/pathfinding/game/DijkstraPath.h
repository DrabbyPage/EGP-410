
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;

class DijkstraPath : public GridPathfinder
{
public:
	DijkstraPath(Graph* pGraph);
	~DijkstraPath();

	Path* findPath(Node* start, Node* end);//make sure to delete the path when you are done!

private:

};

struct NodeRecord
{
	NodeRecord(Node* newNode, Connection* newConnections, float newCost)
	{
		node = newNode;
		costSoFar = newCost;

		nodeRecordConnections = newConnections;

	}

	NodeRecord()
	{
		node = nullptr;
		costSoFar = 0;
		nodeRecordConnections = nullptr;
	}

	Node* node;
	Connection* nodeRecordConnections;
	float costSoFar;
};

#endif // !DIJKSTRSA_H

