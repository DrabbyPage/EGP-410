
#ifndef INPUT_SYSTEM
#define INPUT_SYSTEM

#include <iostream>
#include <string>
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"

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

	void getInput(int x, int y);
	bool checkExit();
	bool spawnNewUnit();
	bool deleteUnit();
	char returnInput();

private:
	Game pGame;

};

#endif