
#include <iostream>
#include <string>

#include <SDL.h>
#include <math.h>
#include <Vector2D.h>

#include "Defines.h"

#include "Heuristic.h"
#include "GameApp.h"
#include "Game.h"
#include "Node.h"
#include "Grid.h"

// constructor, takes in the goal node for estimating
Heuristic::Heuristic(Node* endNode)
{
	heurEndNode = endNode;
}

Heuristic::~Heuristic()
{

}

//generates an estimated cost to reach the stored
// goal for the given node
float Heuristic::estimate(Node* currentNode)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	
	Vector2D nodePos = pGame->getGrid()->getULCornerOfSquare(currentNode->getId());
	Vector2D endNodePos = pGame->getGrid()->getULCornerOfSquare(heurEndNode->getId());

	Vector2D diff = nodePos - endNodePos;
	float distance = diff.getLength();
	//float distance = sqrt(pow(diff.getX(),2) + pow(diff.getY(),2));

	return distance;
}