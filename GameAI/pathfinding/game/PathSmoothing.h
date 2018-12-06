#ifndef PATH_SMOOTHING_H_
#define PATH_SMOOTHING_H_

#include <Trackable.h>

class Path;
class Node;

class PathSmoothing : public Trackable
{
public:
	inline PathSmoothing() {};
	inline ~PathSmoothing() {};

	Path* smoothPath(Path* inputPath);
	bool clearedRaycast(Node* outputNode, Node* inputNode);
};

#endif