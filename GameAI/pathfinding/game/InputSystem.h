
#ifndef INPUT_SYSTEM
#define INPUT_SYSTEM

#include <iostream>
#include <string>
#include "GameMessageManager.h"
#include "PathToMessage.h"

#include <SDL.h>
#include <math.h>
#include <Vector2D.h>

#include "Defines.h"

#include "Game.h"


class InputSystem
{
public:
	//constructor and deconstructor
	InputSystem();
	~InputSystem();

	void process();

private:
	Game pGame;

};

#endif