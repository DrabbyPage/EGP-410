
#ifndef HEURISTIC_H
#define HEURISTIC_H

class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;
class Node;

class Heuristic
{
public:
	// constructor, takes in the goal node for estimating
	Heuristic(Node* endNode);
	~Heuristic();

	//generates an estimated cost to reach the stored
	// goal for the given node
	float estimate(Node*);

private:
	// stores the end node that this heuristic is 
	// estimating for
	Node* heurEndNode;
};

#endif // !HEURISTIC_H

