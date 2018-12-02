#include "PathSmoothing.h"
#include "Path.h"
#include "Node.h"
#include "GameApp.h"
#include "Vector2D.h"
#include "Grid.h"

Path PathSmoothing::smoothPath(Path inputPath)
{
	// if the path is only two nodes long then we can't smooth it so return
	if (inputPath.getNumNodes() <= 2)
	{
		std::cout << "Path is less than two units" << std::endl;
		return inputPath;
	}

	// compile an output path
	Path outputPath;
	outputPath.addNode(inputPath.peekNode(0));
	
	// keep track of where we are in the inputPath we start at 2, because
	// we assume two adjacent nodes will pass the raycast
	int inputIndex = 2;
	int amountOfOutput = 1;

	// loop until we find the last item in the input
	while(inputIndex < inputPath.getNumNodes() - 1)
	{
		amountOfOutput = outputPath.getNumNodes();

		// check if we can "see" the next node (there is no wall/ blocking value in the way)
		bool rayClear = clearedRaycast(outputPath.peekNode(amountOfOutput - 1), inputPath.peekNode(inputIndex));

		if(!rayClear)
		{
			// the ray text failed, add the last node that passed to the output list
			outputPath.addNode(inputPath.peekNode(inputIndex - 1));
		}
		
		// consider the next node
		inputIndex = inputIndex + 1;
	}

	// we've reached the end of the input path, add the end node to the output and return it
	outputPath.addNode(inputPath.peekNode(inputPath.getNumNodes() - 1));

	// return outputPath
	return outputPath;
}

bool PathSmoothing::clearedRaycast(Node* outputNode, Node* inputNode)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);


	Grid* pGrid = pGame->getGrid();

	Vector2D fromGridPos = pGrid->getULCornerOfSquare(outputNode->getId());
	Vector2D toGridPos = pGrid->getULCornerOfSquare(inputNode->getId());

	Vector2D diff;

	// getting the direction of the input node from the output
	diff = toGridPos - fromGridPos;

	// making a copy of the info to make teh following code smaller
	float diffX = diff.getX();
	float diffY = diff.getY();

	// this variable is the number of checks between the output node and the input node
	int amountOfChecks = 50;//(int)diff.getLength(); // the best one i found was 50 // need to do some other kind of dist for it

	// making a copy of the info to make teh following code smaller
	float posX = fromGridPos.getX();
	float posY = fromGridPos.getY();

	// will check in between the two given nodes for the wall/ or the blocking, value
	for (int i = 0; i < amountOfChecks; i++)
	{
		// checks of there is a wall value between the nodes
		bool pathBlocked = (pGame->getGrid()->getValueAtPixelXY((int)posX, (int)posY) == BLOCKING_VALUE); // checking if the pos is a wall


		if (pathBlocked)
		{
			// we learned there is a wall in the way and therefore we cannot see the input node
			return false;
		}

		// the position in check increments until it hits the node in question
		posX = posX + (diffX / amountOfChecks);
		posY = posY + (diffY / amountOfChecks);
	}

	// by this time we have learned there is no wall in the way
	return true;

}