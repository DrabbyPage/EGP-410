#include "Unit.h"
#include <assert.h>
#include <chrono>
#include <fstream>

#include "GameApp.h"
#include "GraphicsSystem.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"
#include "ComponentManager.h"
#include "SpriteManager.h"
#include "UnitManager.h"

#include "GhostChaseState.h"
#include "GhostWanderState.h"
#include "GhostFleeState.h"
#include "GhostIdleState.h"

Unit::Unit(const Sprite& sprite) :
	mSprite(sprite),
	mUnitTag(INVALID_TYPE),
	mPositionComponentID(INVALID_COMPONENT_ID),
	mPhysicsComponentID(INVALID_COMPONENT_ID),
	mSteeringComponentID(INVALID_COMPONENT_ID),
	mShowTarget(false)
{
	mUnitTimer = 0;
	mMultiplierTimer = 0;
	mUnitActive = true;
}


Unit::Unit(const Sprite& sprite, UnitType tag) :
	mSprite(sprite),
	mUnitTag(tag),
	mPositionComponentID(INVALID_COMPONENT_ID),
	mPhysicsComponentID(INVALID_COMPONENT_ID),
	mSteeringComponentID(INVALID_COMPONENT_ID),
	mShowTarget(false)
{
	std::string unitHealthFileName;

	if (mUnitTag == PAC_MAN)
	{
		unitHealthFileName = "PacManHealth.txt";
	}
	else if (mUnitTag == GHOST)
	{
		unitHealthFileName = "GhostHealth.txt";
		createGhostStates();
	}

	// if it is a ghost or pacman give the unit a health
	if (unitHealthFileName.size() > 0)
	{
		std::ifstream unitHealthFile;
		std::string unitHealthString;
		unitHealthFile.open(unitHealthFileName);
		unitHealthFile >> unitHealthString;
		mUnitHealth = std::stoi(unitHealthString);
	}

	mUnitTimer = 0;
	mMultiplierTimer = 0;
	mUnitActive = true;
}

Unit::~Unit()
{
	if (mUnitTag == GHOST)
	{
		delete mpWanderTransition;
		delete mpFleeTransition;
		delete mpChaseTransition;
		delete mpIdleTransition;

		delete mUnitStateMachine;
	}

}

void Unit::update(float elapsedTime)
{
	// updating pip
	if (mUnitTag == BIG_PIP || mUnitTag == SMALL_PIP)
	{
		// if the unit is not active then start the timer for it to respawn
		if (!mUnitActive)
		{
			// add onto the timer
			mUnitTimer += elapsedTime;

			// grab the respawn time from the file
			std::ifstream inputFile;
			inputFile.open("PipRespawnTime.txt");
			std::string respawnTimeString;
			inputFile >> respawnTimeString;
			float respawnTimeFloat = 0;
			respawnTimeFloat = std::stof(respawnTimeString);

			// if the time is more than the respawn time then reset
			if (mUnitTimer > respawnTimeFloat)
			{
				mUnitTimer = 0;
				mUnitActive = true;
			}
		}
	}
	else if (mUnitTag == GHOST)
	{
		GhostWanderState* pWanderState = dynamic_cast<GhostWanderState*>(mUnitStateMachine->getStateList().at(0));
		pWanderState->setId(mID);
		GhostFleeState* pFleeState = dynamic_cast<GhostFleeState*>(mUnitStateMachine->getStateList().at(1));
		pFleeState->setId(mID);
		GhostChaseState* pChaseState = dynamic_cast<GhostChaseState*>(mUnitStateMachine->getStateList().at(2));
		pChaseState->setId(mID);
		GhostIdleState* pIdleState = dynamic_cast<GhostIdleState*>(mUnitStateMachine->getStateList().at(3));
		pIdleState->setId(mID);

		// if the unit had died
		if (!mUnitActive)
		{
			// reset the position
			mpPositionComponent->setPosition(Vector2D(480, 448));

			// add onto the timer
			mUnitTimer += elapsedTime;

			// grab the respawn time from the file
			std::ifstream inputFile;
			inputFile.open("GhostRespawnTime.txt");
			std::string respawnTimeString;
			inputFile >> respawnTimeString;
			float respawnTimeFloat = 0;
			respawnTimeFloat = std::stof(respawnTimeString);

			// if the time is more than the respawn time then reset
			if (mUnitTimer > respawnTimeFloat)
			{
				mUnitTimer = 0;
				mUnitActive = true;
			}
		}
		else
		{
			mUnitStateMachine->update();
		}
	}

	// update teh speed multiplier  //3.0f is the max amount of time
	if (mMultiplierTimer < 3.0f && mSpeedMultiplier > 1.0f)
	{
		mMultiplierTimer += elapsedTime;
	}
	else if (mMultiplierTimer >= 3.0f)
	{
		mMultiplierTimer = 0;
		mSpeedMultiplier = 1.0f;
	}
}

void Unit::draw() const
{
	if (this != nullptr)
	{
		PositionComponent* pPosition = getPositionComponent();
		assert(pPosition != NULL);
		const Vector2D& pos = pPosition->getPosition();
		if(mUnitActive)
		{
			gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());
		}

		if (mShowTarget)
		{
			SteeringComponent* pSteering = getSteeringComponent();
			assert(pSteering != NULL);
			const Vector2D& targetLoc = pSteering->getTargetLoc();

			if (&targetLoc != &ZERO_VECTOR2D)
			{
				Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
				assert(pTargetSprite != NULL);
				gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
			}
		}
	}

}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
}

PositionComponent* Unit::getPositionComponent() const
{
	return mpPositionComponent;
}

PhysicsComponent* Unit::getPhysicsComponent() const
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	PhysicsComponent* pComponent = pGame->getComponentManager()->getPhysicsComponent(mPhysicsComponentID);
	return pComponent;
}

SteeringComponent* Unit::getSteeringComponent() const
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	SteeringComponent* pComponent = pGame->getComponentManager()->getSteeringComponent(mSteeringComponentID);
	return pComponent;
}

void Unit::setSteering(Steering::SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(SteeringData(type, targetLoc, mID, targetUnitID));
	}
}

void Unit::speedUpUnit(float multiplier)
{
	mSpeedMultiplier = multiplier;
}

void Unit::createGhostStates()
{
	GhostWanderState* pWanderState = new GhostWanderState(0);
	GhostFleeState* pFleeState = new GhostFleeState(1);
	GhostChaseState* pChaseState = new GhostChaseState(2);
	GhostIdleState* pIdleState = new GhostIdleState(3);

	mpWanderTransition = new StateTransition(GHOST_WANDER, 0);
	mpFleeTransition = new StateTransition(GHOST_FLEE, 1);
	mpChaseTransition = new StateTransition(GHOST_CHASE, 2);
	mpIdleTransition = new StateTransition(GHOST_IDLE, 3);

	pWanderState->setId(mID);
	pWanderState->addTransition(mpFleeTransition);
	pWanderState->addTransition(mpChaseTransition);
	pWanderState->addTransition(mpIdleTransition);

	pFleeState->setId(mID);
	pFleeState->addTransition(mpWanderTransition);
	pFleeState->addTransition(mpChaseTransition);
	pFleeState->addTransition(mpIdleTransition);

	pChaseState->setId(mID);
	pChaseState->addTransition(mpFleeTransition);
	pChaseState->addTransition(mpWanderTransition);
	pChaseState->addTransition(mpIdleTransition);

	pIdleState->setId(mID);
	pIdleState->addTransition(mpFleeTransition);
	pIdleState->addTransition(mpChaseTransition);
	pIdleState->addTransition(mpWanderTransition);

	mUnitStateMachine = new StateMachine();

	mUnitStateMachine->addState(pWanderState);
	mUnitStateMachine->addState(pFleeState);
	mUnitStateMachine->addState(pChaseState);
	mUnitStateMachine->addState(pIdleState);

	mUnitStateMachine->setInitialStateID(0);

	mUnitStateMachine->getCurrentState();
}
