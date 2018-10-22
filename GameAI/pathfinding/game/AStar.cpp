
#include "AStar.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "GridPathfinder.h"
#include "Heuristic.h"

AStarPath::AStarPath(Graph* pGraph)
	: GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
	#ifdef VISUALIZE_PATH
	mpPath = NULL;
	#endif
}

AStarPath::~AStarPath()
{
	#ifdef VISUALIZE_PATH
	delete mpPath;
	#endif
}

Path* AStarPath::findPath(Node* start, Node* end)
{
	Heuristic* heur = new Heuristic(end);
	//make sure to delete the path when you are done!

	// initialize the record for the start node
	// startRecord = new StarNodeRecord()
	// startRecord.node = start
	// startRecord.connection = none
	// startRecord.costSoFar = 0
	// startRecord.estiCostSoFar = heuristic.estimate(start)
	StarNodeRecord startRecord = StarNodeRecord();
	startRecord.node = start;
	startRecord.recordConnection = nullptr;
	startRecord.costSoFar = 0;
	startRecord.estiTotalCost = heur->estimate(start);

	// initialize the open and closed list
	// open = pathfindingList()
	// open += startRecord
	// closed = pathfindingList()
	std::vector<StarNodeRecord> openList;
	std::vector<StarNodeRecord> closedList;

	openList.push_back(startRecord);

	StarNodeRecord current;

	//iterate through processing each node
	// while length(open) >0
	while (openList.size() > 0)
	{
		//find the smallest element in the open list
		// using the enstimated total cost
		// current = open.smallestElement()
		current = openList.front();
		// if it is the goal node tehn terminate
		// if current.node == goal: break
		if (current.node == end)
		{
			std::cout << "Reached the end of A_Star" << std::endl;
			break;
		}
		// otherwise get its outgoing connections
		else
		{
			// connections= graph.getConnections(current)
			std::vector<Connection*> connections;
			connections = mpGraph->getConnections(current.node->getId());

			// loop through each connection in turn
			// for connection in connections:
			for (int i = 0; i < connections.size(); i++)
			{
				// get the cost estimate for the end node
				// endNode = connection.getToNode()
				// endNodeCost = current.costSoFar + connection.GetCost()
				Node* endNode = connections[i]->getToNode();
				float endNodeCost = current.costSoFar + connections[i]->getCost();

				StarNodeRecord endNodeRecord;
				float endNodeHeuristic;

				bool inClosedList = false;
				bool inOpenList = false;

				// if the node is closed we may have to skip or remove it from the close list
				// if closed.contains(endNode):
				for (auto nodeRecord = closedList.begin(); nodeRecord != closedList.end(); nodeRecord++)
				{
					// here we find the record in the closed list corresponding to the end node
					// endNodeRecord = closedNode.find(endNode)
					if (nodeRecord->node == endNode)
					{
						inClosedList = true;
						endNodeRecord.node = nodeRecord->node;
					}

					// if we didnt find a shorter route, skip
					// if(endNodeRecord.costSoFar <= endNodeCost:
					if (endNodeRecord.costSoFar <= endNodeCost)
					{
						// continue;
						break;
					}
					// otherwise remove it from the closed list;
					else
					{
						// closed -= endNodeRecord
						closedList.erase(nodeRecord);

						// we can use the node's old cost values to calculate its heuristic without
						// calling the possibly expensive heuristic function
						// endNodeHeuristic = endNodeRecord.estiTotalCost - endNodeRecord.costSoFar
						endNodeHeuristic = endNodeRecord.estiTotalCost - endNodeRecord.costSoFar;
					}


				}
				if (!inClosedList)
				{
					// skip if the node is open and we've not found a better route
					// else if open.contains(endNode):
					for (auto record = openList.begin(); record != openList.end(); record++)
					{
						if (record->node == endNode)
						{
							break;
						}
						else
						{
							// here we find the record in the open list corresponding to the endNode
							// endNodeRecord = open.find(endNode)
							endNodeRecord.node = record->node;

							// if our route is no better, then skip
							// if(endNodeRecord.costSoFar <= endNodeCost
							if (endNodeRecord.costSoFar <= endNodeCost)
							{
								// continue
								continue;
							}
							else
							{

								// we can use the node's old cost values to calculate its heuristic without
								// calling the possibly expensive heuristic function
								// endNodeHeuristic = endNodeRecord.cost - endNodeRecord.costSoFar
								endNodeHeuristic = endNodeRecord.recordConnection->getCost() - endNodeRecord.costSoFar;
							}
						}
					}
				}
				// otherwise we know we've got an unvisited node so make a record for it
				// else:
				else if (!inClosedList && !inOpenList)
				{
					// endNodeRecord = new StarNodeRecord()
					// endNodeRecord.node = endNode
					endNodeRecord = StarNodeRecord();
					endNodeRecord.node = endNode;

					// we'll need to calculate the heuristic value using the function, since
					// we dont have an existing record to use
					// endNodeHeuristic = heuristic.estimate(endNode)
					endNodeHeuristic = heur->estimate(endNode);

					// we're here if we need to update the node
					// update the cost, estimate, and connection
					// endNodeRecord.cost = endNodeCost
					// endNodeRecord.connection = connection
					// endNodeRecord.estimatedTotalCost = endNodeCost + endNodeHeuristic
					endNodeRecord.costSoFar = endNodeCost;
					endNodeRecord.recordConnection = connections[i];
					endNodeRecord.estiTotalCost = endNodeCost + endNodeHeuristic;

					// and add it to the openList
					// if not open.contains(endNode): // but by this point we know that it isnt in the open list
					if (!inClosedList)
					{
						bool inOpen = false;
						for (auto record = openList.begin(); record < openList.end(); record++)
						{
							if (record->node == endNodeRecord.node)
							{
								inOpen = true;
								break;
							}
						}

						if (!inOpen)
						{
							openList.push_back(endNodeRecord);
						}
					}
					
				}

			}
			// we've finished looking at the connections for the current node so add it to the closed 
			// list and remove it from the open list
			// open -= current
			// closed += current
			openList.erase(openList.begin());
			closedList.push_back(current);
			mVisitedNodes.push_back(current.node);
		}
	}

	// we're here if we found a goal or if we have no more nodes to search find which one
	// if(current.node != goal):
	if(current.node != end)
	{
		// we've run out of nodes without finding the goal, so theres no solution
		// return none
		return nullptr;
	}
	// else:
	else
	{
		// compile the list of connections in the path
		// path = []
		Path* a_Star_Path = new Path();

		// work back along the path, accumulating connections
		// while(current.node != start)
		while(current.node != start)
		{
			// path+= current.connection
			// current = current.connection.getFromNode()
			// FYI Update the current's connection as well
			a_Star_Path->addNode(current.node);
			current.node = current.recordConnection->getFromNode();
			for (auto node = closedList.begin(); node < closedList.end(); node++)
			{
				if (node->node == current.node)
				{
					current.recordConnection = node->recordConnection;
				}
			}
		}

		// reverse the path and return it

		// reverse the path, and return it
		Path* tempPath = new Path();

		for (int i = 0; i < a_Star_Path->getNumNodes(); i++)
		{
			Node* newNode;
			int lastNodeIndex;

			lastNodeIndex = a_Star_Path->getNumNodes() - (i + 1);

			newNode = a_Star_Path->peekNode(lastNodeIndex);

			tempPath->addNode(newNode);
		}
		delete heur;
		delete a_Star_Path;
		a_Star_Path = tempPath;

		#ifdef VISUALIZE_PATH
		mpPath = a_Star_Path;
		#endif
		return a_Star_Path;
		// return reverse path
	}
}

