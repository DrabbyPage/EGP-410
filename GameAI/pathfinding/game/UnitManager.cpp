#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "GameApp.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"
#include "Grid.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize) :
	mPool(maxSize, sizeof(Unit))
{

}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = pGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData, shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;
	}

	return pUnit;
}

Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}

Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	int posX;
	int posY;

	do
	{
		posX = (rand() % pGame->getGrid()->getGridWidth()) * pGame->getGrid()->getSquareSize();
		posY = (rand() % pGame->getGrid()->getGridHeight()) * pGame->getGrid()->getSquareSize();

	} while (pGame->getGrid()->getValueAtPixelXY((int)posX, (int)posY) == BLOCKING_VALUE);

	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX, posY), 0));

	if (pUnit != NULL)
	{
		pUnit->setSteering(Steering::PATH, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
	}

	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);

	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	auto it = mUnitMap.find(id);

	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

								 //remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = pGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		//if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		//{
		//	target = 1;
		//}

		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

void UnitManager::checkCollisions()
{
	if (getPlayerUnit() != nullptr)
	{
		for (unsigned int i = 1; i < mUnitMap.size(); i++)
		{
			if (collision(mUnitMap[0], mUnitMap[i]))
			{
				// if collision is an enemy kill obj

				// if obj is pip then add score
				std::cout << "there was a collision" << std::endl;
			}
		}
	}
}

bool UnitManager::collision(Unit* obj1, Unit* obj2)
{
	Vector2D obj1Pos = obj1->getPositionComponent()->getPosition();
	Vector2D obj2Pos = obj2->getPositionComponent()->getPosition();
	//If the leftmost or rightmost point of the first sprite lies somewhere inside the second, continue.
	if ((obj1Pos.getX() >= obj2Pos.getX()
		&& obj1Pos.getX() <= (obj2Pos.getX() + obj2->mSprite.getWidth()))
		|| ((obj1Pos.getX() + obj1->mSprite.getWidth()) >= obj2Pos.getX()
		&& (obj1Pos.getX() + obj1->mSprite.getWidth()) <= (obj2Pos.getX() + obj2->mSprite.getWidth())))
	{
		//Now we look at the y axis:
		if ((obj1Pos.getY() >= obj2Pos.getY()
			&& obj1Pos.getY() <= (obj2Pos.getY() + obj2->mSprite.getHeight()))
			|| ((obj1Pos.getY() + obj1->mSprite.getHeight()) >= obj2Pos.getY()
			&& (obj1Pos.getY() + obj1->mSprite.getHeight()) <= (obj2Pos.getY() + obj2->mSprite.getHeight())))
		{
			//The sprites appear to overlap.
			return true;
		}
	}
	//The sprites do not overlap:
	return false;
}