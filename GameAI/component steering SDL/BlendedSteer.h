
#ifndef BLENDED_H
#define BLENDED_H

#include <iostream>
#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include <vector>

#include "Steering.h"
#include "Behavior.h"

class BlendedSteering : public Steering
{
public:
	BlendedSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee);
	~BlendedSteering();
	virtual Steering* getSteering();

	void AddBehavior(Steering* steer, float weight);

private:
	std::vector<Behavior*> behaviorList;
	float weight;
};

#endif