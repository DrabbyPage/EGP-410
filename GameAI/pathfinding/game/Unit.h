#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>
#include <chrono>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"
#include "StateMachine.h"

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;

const Uint32 DEFAULT_QUEUE_CAPACITY = 8;


//class Unit : public TransactionProcessor
class Unit : public Trackable
{
public:
	enum UnitType
	{
		INVALID_TYPE = -1,
		PAC_MAN,
		GHOST,
		BIG_PIP,
		SMALL_PIP,
		ENEMY_POWER_UP
	};

	void draw() const;
	float getFacing() const;
	void update(float elapsedTime);

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	UnitType getTag()const { return mUnitTag; };

	bool getShowTarget() { return mShowTarget; };
	bool getUnitActive() { return mUnitActive; };

	void setShowTarget(bool val) { mShowTarget = val; };
	void setUnitActive(bool newBool) { mUnitActive = newBool; };
	void setTag(UnitType newTag) { mUnitTag = newTag; };

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);

private:
	UnitType mUnitTag;
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	PositionComponent* mpPositionComponent = NULL;
	Sprite mSprite;
	float mMaxAcc;
	float mMaxSpeed;
	float mMaxRotAcc;
	float mMaxRotVel;
	bool mShowTarget;
	bool mUnitActive;

	int mUnitHealth;

	float mUnitTimer;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;

	StateMachine mUnitStateMachine;
};

