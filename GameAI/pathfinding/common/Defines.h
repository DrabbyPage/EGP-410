
#pragma once

#include <string>
#include <deanlibdefines.h>

typedef int IDType;
typedef std::string IDDescriptor;
typedef std::string GraphicsBufferID;

const IDType INVALID_ID = -1;

const IDType HIGHEST_ID = 9999;

#define MESSAGE_MANAGER gpGame->getMessageManager()
#define GRAPHICS_SYSTEM gpGame->getGraphicsSystem()

typedef Uint32 UnitID;

const UnitID INVALID_UNIT_ID = UINT_MAX;

//Circle conversion constants
const float PI = 3.14159f;
const float TAU = 2.0f * PI;

const float PI_DEG = 180.0f;
const float TAU_DEG = 360.0f;

const float DEG_TO_RAD = PI / PI_DEG;
const float RAD_TO_DEG = PI_DEG / PI;
