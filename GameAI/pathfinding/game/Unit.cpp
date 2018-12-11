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


Unit::Unit(const Sprite& sprite) :
	mSprite(sprite),
	mUnitTag(INVALID_TYPE),
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
	}

	if (unitHealthFileName.size() > 0)
	{
		std::ifstream unitHealthFile;
		std::string unitHealthString;
		unitHealthFile.open(unitHealthFileName);
		unitHealthFile >> unitHealthString;
		mUnitHealth = std::stoi(unitHealthString);
	}

	mUnitTimer = 0;
	mUnitActive = true;
}

Unit::~Unit()
{
	mUnitStateMachine.~StateMachine();
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
			mUnitStateMachine.update();
		}
	}
	else if (mUnitTag == PAC_MAN)
	{
		if (!mUnitActive)
		{
			// reset the position
			mpPositionComponent->setPosition(Vector2D(480, 448));

			GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
			pGame->getUnitManager()->resetAllUnitPos();

			mUnitActive = true;
		}

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