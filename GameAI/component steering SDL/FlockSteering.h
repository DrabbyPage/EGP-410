
#ifndef FLOCK_H
#define FLOCK_H

#include "Steering.h"
#include "BlendedSteer.h"

#include "Cohesion.h"
#include "Separation.h"
#include "Alignment.h"
#include "WanderSteering.h"
#include "ArriveAndFaceSteering.h"

class FlockSteering : public Steering
{
public:
	FlockSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID,
		bool shouldFlee);

	~FlockSteering();
	virtual Steering* getSteering();

private:
	float weight;
	BlendedSteering* flockBlendSteer;
	AlignmentSteering* flockAlignSteer;
	CohesionSteering* flockCoSteer;
	SeparationSteering* flockSepSteer;
	WanderSteering* flockWanderSteer;
};

#endif

