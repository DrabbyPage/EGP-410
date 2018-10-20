
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
	//dijkstraPath = nullptr;
}

DijkstraPath::~DijkstraPath()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
	/*
	if (dijkstraPath != nullptr)
	{
		delete dijkstraPath;
	}*/
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

	startRecord.node = start;
	//startRecord.nodeRecordConnections = mpGraph->getConnections(start->getId());
	startRecord.costSoFar = 0;


	// initialize the open and closed lists
	// open = PathfindingList()
	// open += startRecord
	// closed = PathfindingList()
	std::vector<NodeRecord> openNodes;
	std::vector<NodeRecord> closedNodes;
	openNodes.push_back(startRecord);
	
	NodeRecord current = NodeRecord();

	std::vector<Connection*> connections;
	connections = mpGraph->getConnections(openNodes.front().node->getId());

	// iterate through processing each node
	// while length(open>0)
	while (openNodes.size() > 0)
	{	
		NodeRecord endNodeRecord = NodeRecord();

		// find the smallest element in the open list
		// current = open.smallestElement()
		current = openNodes.front();

		// if it is the goal node then terminate
		// if( current.node == goal: break
		if (current.node == end)
		{
			std::cout << "at the end" << std::endl;
			break;
		}
		// otherwise get this outgoing connections
		else
		{
			// connections = mpGraph.getConnections(current)
			connections = mpGraph->getConnections(current.node->getId());
			
			// loop through each connection in turn 
			// for connection in connections:
			for (unsigned int i = 0; i < connections.size(); i++)
			{
				// get the cost estimate for the end node
				// end node = connection.getNode()
				// endNodeCost = current.costSoFar + connection.getCost()
				Node* endNode = connections[i]->getToNode();
				Connection* endConnect = connections[i];
				float endNodeCost = current.costSoFar + connections[i]->getCost();
				
				bool inClosedList = false;
				bool inOpenList = false;
				
				// skip if the node is closed
				// if(closed.contains(endNode): continue
				for (auto record = closedNodes.begin(); record != closedNodes.end(); record++)
				{
					if (record->node == endNode)
					{
						inClosedList = true;
						break;
					}
				}
				// else if open.contains(endNode):
				if (!inClosedList)
				{
					for (auto record = openNodes.begin(); record < openNodes.end(); record++)
					{
						if (record->node == endNode)
						{
							// here we find the record in the open list corresponding to the endNode
							// endNodeRecord = open.find(endNode)
							endNodeRecord.node = record->node;
							inOpenList = true;
							// if(endNodeRecord.cost <= endNodeCost)
							// continue
							if (endNodeRecord.costSoFar <= endNodeCost)
							{
								continue;
							}
							break;
						}
					}
				}
				// otherwise we know we've got an unvisited node so make a record of it
				if(!inClosedList && !inOpenList)
				{
					// endNodeRecord = new NodeRecord()
					// endNodeRecord.node = endNode
					endNodeRecord.node = endNode;

					// we're here if we need to update the node
					// update the cost and connection
					endNodeRecord.costSoFar = endNodeCost;
					endNodeRecord.nodeRecordConnections = endConnect;
				}

				// and add it to the open list
				// if NOT open.contains(endNode)
				if (!inClosedList)
				{
					bool inOpen = false;
					for (auto record = openNodes.begin(); record < openNodes.end(); record++)
					{
						if (record->node == endNodeRecord.node)
						{
							inOpen = true;
							break;
						}
					}

					if (!inOpen)
					{
						openNodes.push_back(endNodeRecord);
					}
				}
			}
			
			// we've finished looking at the connections for
			// the current node, so add it to the closed list
			// and remove it form the open list:

		}
		openNodes.erase(openNodes.begin());
		closedNodes.push_back(current);
		mVisitedNodes.push_back(current.node);
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
		/*
		if (dijkstraPath != nullptr)
		{
			delete dijkstraPath;
		}*/

		Path* dijkstraPath = new Path();

		// work pack along the path, accumulating connections
		// while(current.node != start):
		// path += current.connection
		// current = current.connection.getFromNode()
		while (current.node != start)
		{
			dijkstraPath->addNode(current.node);
			current.node = current.nodeRecordConnections->getFromNode();
			
			for (auto node = closedNodes.begin(); node < closedNodes.end(); node++)
			{
				if (node->node == current.node)
				{
					current.nodeRecordConnections = node->nodeRecordConnections;
				}
			}

		}

		// reverse the path, and return it
		// return reverse(path)
		//dijkstraPath = ReverseThePath(dijkstraPath);
		Path* tempPath = new Path();
		
		std::cout << "reversing the path" << std::endl;
		for (int i = 0; i < dijkstraPath->getNumNodes(); i++)
		{
			Node* newNode;
			int lastNodeIndex;

			lastNodeIndex = dijkstraPath->getNumNodes() - (i + 1);

			newNode = dijkstraPath->peekNode(lastNodeIndex);

			tempPath->addNode(newNode);
		}
		dijkstraPath = tempPath;
		//delete tempPath;

#ifdef VISUALIZE_PATH
		mpPath = dijkstraPath;
#endif

		return dijkstraPath;
	}

}

