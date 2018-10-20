
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

DijkstraPath::DijkstraPath(Graph* pGraph) 
	: GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
	dijkstraPath = nullptr;
}

DijkstraPath::~DijkstraPath()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif

	if (dijkstraPath != nullptr)
	{
		delete dijkstraPath;
	}
}

Path* DijkstraPath::findPath(Node* start, Node* end)
{
	//make sure to delete the path when you are done!

	//initialize the record for the start nodes
	// startRecord = new NodeRecord()
	// startRecord.node = startNode;
	// startRecord.connection = none;
	// startRecord.costSoFar = 0;

	NodeRecord startRecord = NodeRecord();
	NodeRecord endNodeRecord = NodeRecord();

	startRecord.node = start;
	startRecord.connection = nullptr;
	startRecord.costSoFar = 0;


	// initialize the open and closed lists
	// open = PathfindingList()
	// open += startRecord
	// closed = PathfindingList()
	std::vector<Node*> openNodes;
	std::vector<Node*> closedNodes;
	openNodes.push_back(startRecord.node);
	
	NodeRecord current = NodeRecord();
	current.node = openNodes.front();

	std::vector<Connection*> connections;
	connections = mpGraph->getConnections(current.node->getId());

	// iterate through processing each node
	// while length(open>0)
	while (openNodes.size() > 0)
	{
		// find the smallest element in the open list
		// current = open.smallestElement()
		current.node = openNodes.front();

		// if it is the goal node then terminate
		// if( current.node == goal: break
		if (current.node == end)
		{
			break;
		}
		else
		{
			// otherwise get this outgoing connections
			// connections = mpGraph.getConnections(current)
			connections = mpGraph->getConnections(current.node->getId());

			// loop through each connection in turn 
			// for connection in connections:
			for (int i = 0; i < connections.size(); i++)
			{
				// get the cost estimate for the end node
				// end node = connection.getNode()
				// endNodeCost = current.costSoFar + connection.getCost()
				Node* endNode = connections[i]->getToNode();
				float endNodeCost = startRecord.costSoFar + connections[i]->getCost();

				// skip if the node is closed
				// if(closed.contains(endNode): continue
				if (std::find(closedNodes.begin(), closedNodes.end(), endNode) != closedNodes.end())
				{
					continue;
				}
				if (std::find(openNodes.begin(), openNodes.end(), endNode) != openNodes.end())
				{
					// else if open.contains(endNode):

					// here we find the record in the open list
					// corresponding to the endNode
					// endNodeRecord = open.find(endNode)
					endNodeRecord.node = endNode;

					// if(endNodeRecord.cost <= endNodeCost)
					// continue
					if (endNodeRecord.costSoFar <= endNodeCost)
					{
						continue;
					}
				}
				else
				{
					// otherwise we know we've got an unvisited node
					// so make a record of it
					// else:
					// endNodeRecord = newNodeRecord()
					// endNodeRecord = endNode
					endNodeRecord.costSoFar = endNodeCost;
					endNodeRecord.node = endNode;
				}


				// we're here if we need to update the node
				// update the cost and connection
				// endNodeRecord.cost = endNodeCost
				// endNodeRecord.connection = connection
				endNodeRecord.costSoFar = endNodeCost;
				endNodeRecord.connection = connections[i];

				// and add it to the open list
				// if not open.contains(endNode)
				// open+= endNodeRecord
				if (std::find(openNodes.begin(), openNodes.end(), endNode) != openNodes.end())
				{
					openNodes.push_back(endNodeRecord.node);
				}

			}

			// we've finished looking at the connections for
			// the current node, so add it to the closed list
			// and remove it form the open list:
			// open -= current
			// closed += current
			for (int i = 0; i < openNodes.size(); i++)
			{
				if (openNodes[i] == current.node)
				{
					openNodes.erase(openNodes.begin() + i);
					closedNodes.push_back(current.node);
					break;
				}
			}

		}

	}

	// we're here if we've either found the goal, or 
	// if we've no more nodes to search, find which:
	// if (current.node != goal)
	if (current.node != end)
	{
		// we've run out of nodes without finding the goal, so theres no solution:
		return nullptr;
	}
	else
	{
		// else:

		// compile the list of connections in the path
		// path = []
		if (dijkstraPath != nullptr)
		{
			delete dijkstraPath;
		}

		dijkstraPath = new Path();

		// work pack along the path, accumulating connections
		// while(current.node != start):
		// path += current.connection
		// current = current.connection.getFromNode()
		while (current.node != start)
		{
			dijkstraPath->addNode(current.node);
			current.node = current.connection->getFromNode();
		}

		// reverse the path, and return it
		// return reverse(path)
		dijkstraPath = ReverseThePath(dijkstraPath);

#ifdef VISUALIZE_PATH
		mpPath = dijkstraPath;
#endif

		return dijkstraPath;
	}

}

Path* DijkstraPath::ReverseThePath(Path* path)
{
	Path* newPath = new Path();

	for (int i = 0; i < path->getNumNodes(); i++)
	{
		Node* newNode;
		int lastNodeIndex;

		lastNodeIndex = path->getNumNodes() - (i + 1);

		newNode = path->peekNode(lastNodeIndex);

		newPath->addNode(newNode);
	}

	if (newPath->getNumNodes() > 0)
	{
		return newPath;
	}
	else
	{
		return nullptr;
	}
}
