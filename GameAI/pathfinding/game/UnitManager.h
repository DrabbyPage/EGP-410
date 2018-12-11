#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;

class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager(){};

	Unit* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA, const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = ZERO_POSITION_DATA, const PhysicsData& physicsData = ZERO_PHYSICS_DATA);
	Unit* createRandomUnit(const Sprite& sprite);

	Unit* getUnit(const UnitID& id) const;
	inline const std::map<UnitID, Unit*>& getUnitMap() const { return mUnitMap; };
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	void checkCollisions();
	bool collision(Unit* obj1, Unit* obj2);
	bool collisionDistBased(Unit* obj1, Unit* obj2);

	void resetAllUnitPos();

private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	std::map<UnitID, Unit*> mUnitMap;
};

