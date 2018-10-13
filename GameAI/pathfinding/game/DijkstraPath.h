
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

class DijkstraPath : public GridPathfinder
{
public:
	DijkstraPath(Graph* pGraph);
	~DijkstraPath();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:

};

#endif // !DIJKSTRSA_H

