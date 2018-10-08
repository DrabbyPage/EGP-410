
#include "InputSystem.h"

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::getInput(int x, int y)
{
	//get input - should be moved someplace better
	SDL_PumpEvents();

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		Vector2D pos(x, y);
		GameMessage* pMessage = new PlayerMoveToMessage(pos);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
}

bool InputSystem::checkExit()
{
	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
		return true;
	else
		return false;
}

bool InputSystem::spawnNewUnit()
{
	SDL_Event newEvent;
	SDL_PollEvent(&newEvent);
	if (newEvent.type == SDL_KEYDOWN)
	{
		if (newEvent.key.keysym.sym == SDLK_a)
		{
			std::cout << "A was pressed" << std::endl;
			return true;
		}
	}
	return false;
}

bool InputSystem::deleteUnit()
{
	SDL_Event newEvent;
	SDL_PollEvent(&newEvent);
	if (newEvent.type == SDL_KEYDOWN)
	{
		if (newEvent.key.keysym.sym == SDLK_d)
		{
			std::cout << "D was pressed" << std::endl;
			return true;
		}
	}
	return false;
}

char InputSystem::returnInput()
{
	SDL_Event newEvent;
	SDL_PollEvent(&newEvent);
	if (newEvent.type == SDL_KEYDOWN)
	{
		if (newEvent.key.keysym.sym == SDLK_a)
		{
			std::cout << "A was pressed" << std::endl;
			return 'A';
		}
	}
	if (newEvent.type == SDL_KEYDOWN)
	{
		if (newEvent.key.keysym.sym == SDLK_d)
		{
			std::cout << "D was pressed" << std::endl;
			return 'D';
		}
	}
	return false;
}