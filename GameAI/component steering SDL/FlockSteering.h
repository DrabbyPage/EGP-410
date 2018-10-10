
#ifndef FLOCK_H
#define FLOCK_H

#include <fstream>

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

	void setNewWeight(std::string, float newWeight);

private:
	float alignWeight = 0.6f, // weight default = 0.6
		cohesionWeight = 0.06f,  // weight default = 0.06
		separWeight = 2.0f, // weight default = 2.0
		wanderWeight = 0.03f; // weight default = 0.03

	std::ifstream mFile;
	std::ofstream mOutFile;
	std::string fileName;

	BlendedSteering* flockBlendSteer;
	AlignmentSteering* flockAlignSteer;
	CohesionSteering* flockCoSteer;
	SeparationSteering* flockSepSteer;
	WanderSteering* flockWanderSteer;
};

#endif

