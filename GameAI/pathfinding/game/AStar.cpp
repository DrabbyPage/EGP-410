
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

Path* AStarPath::findPath(Node* start, Node* end, Heuristic* heur)
{
	//make sure to delete the path when you are done!

	// initialize the record for the start node
	// startRecord = new NodeRecord()
	// startRecord.node = start
	// startRecord.connection = none
	// startRecord.costSoFar = 0
	// startRecord.estiCostSoFar = heuristic.estimate(start)

	// initialize the open and closed list
	// open = pathfindingList()
	// open += startRecord
	// closed = pathfindingList()

	//iterate through processing each node
	// while length(open) >0
	{
		//find the smallest element in the open list
		// using the enstimated total cost
		// current = open.smallestElement()

		// if it is the goal node tehn terminate
		// if current.node == goal: break
		{

		}

		// otherwise get its outgoing connections
		{
			// connections= graph.getConnections(current)
			
			// loop through each connection in turn
			// for connection in connections:
			{
				// get the cost estimate for the end node
				// endNode = connection.getToNode()
				// endNodeCost = current.costSoFar + connection.GetCost()

				// if the node is closed we may have to skip or remoce it from teh close list
				// if closed.contains(endNode):
				{
					// here we find the record in the closed list corresponding to the end node
					// endNodeRecord = closedNode.find(endNode)

					// if we didnt find a shorter route, skip
					// if(endNodeRecord.costSoFar <= endNodeCost:
					// continue;

					// otherwise remove it from the closed list;
					// closed -= endNodeRecord

					// we can use the node's old cost values to calculate its heuristic without
					// calling the possibly expensive heuristic function
					// endNodeHeuristic = endNodeRecord.estiTotalCost - endNodeRecord.costSoFar

				}

				// skip if the node is open and we've not found a better route
				// else if open.contains(endNode):
				{
					// here we find the record in the open list corresponding to the endNode
					// endNodeRecord = open.find(endNode)

					// if our route is no better, then skip
					// if(endNodeRecord.costSoFar <= endNodeCost
					{
						// continue
					}

					// we can use the node's old cost values to calculate its heuristic without
					// calling the possibly expensive heuristic function
					// endNodeHeuristic = endNodeRecord.cost - endNodeRecord.costSoFar

					// otherwise we know we've got an unvisited node so make a record for it
					// else:
					{
						// endNodeRecord = new NodeRecord()
						// endNodeRecord.node = endNode

						// we'll need to calculate the heuristic value using the function, since
						// we dont have an existing record to use
						// endNodeHeuristic = heuristic.estimate(endNode)
					}

					// we're here if we need to update the node
					// update the cost, estimate, and connection
					// endNodeRecord.cost = endNodeCost
					// endNodeRecord.connection = connection
					// endNodeRecord.estimatedTotalCost = endNodeCost + endNodeHeuristic

					// and add it to the openList
					// if not open.contains(endNode):
					{
						// open += endNodeRecord
					}

				}
			}
			
		}

		// we've finished looking at the connections for the current node so add it to the closed 
		// list and remove it from the open list
		// open -= current
		// closed += current

	}

	// we're here if we found a goal or if we have no more nodes to search find which one
	// if(current.node != goal):
	{
		// we've run out of nodes without finding the goal, so theres no solution
		// return none
	}
	// else:
	{
		// compile the list of connections in the path
		// path = []

		// work back along the path, accumulating connections
		// while(current.node != start)
		{
			// path+= current.connection
			// current = current.connection.getFromNode()
			// FYI Update the current's connection as well
		}

		// reverse the path and return it
		// return reverse path
	}
}

