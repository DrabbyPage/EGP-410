
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
	Path* ReverseThePath(Path* path);

private:
	Path* dijkstraPath;
};

struct NodeRecord
{
	Node* node;
	Connection* connection;
	float costSoFar;
};

#endif // !DIJKSTRSA_H

