#ifndef POSITION_COMPONENT_H_
#define POSITION_COMPONENT_H_

#include <Vector2D.h>
#include <Trackable.h>

#include "Component.h"

struct PositionData : public Trackable
{
	inline PositionData() : pos(ZERO_VECTOR2D), facing(0.0f) {};
	inline PositionData(const Vector2D& thePos, float theFacing) : pos(thePos), facing(theFacing) {};

	Vector2D pos;
	float facing;
};

const PositionData ZERO_POSITION_DATA;

class PositionComponent : public Component
{
public:
	PositionComponent(const ComponentID& id, bool shouldWrap = true) : Component(id), mWrap(shouldWrap) {};
	~PositionComponent() {};

	//getters and setters
	const Vector2D& getPosition() const { return mData.pos; };
	void setPosition(const Vector2D& pos) { mData.pos = pos; wrapCoords(); };
	void modPosition(const Vector2D& mod) { mData.pos += mod; wrapCoords(); };
	float getFacing() const { return mData.facing; };
	void setFacing(float facing) { mData.facing = facing; };
	void modFacing(float mod) { mData.facing += mod; };
	const PositionData& getData() const { return mData; };
	void setData(const PositionData& data) { mData = data; wrapCoords(); };
	void modData(const PositionData& data) { mData.pos += data.pos; mData.facing += data.facing; wrapCoords(); };

private:
	PositionData mData;

	//should we wrap around the screen?
	bool mWrap;

	void wrapCoords();

	friend class ComponentManager;
};

#endif