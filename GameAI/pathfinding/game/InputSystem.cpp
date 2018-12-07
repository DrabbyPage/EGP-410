
#include "InputSystem.h"
#include "GameApp.h"

InputSystem::InputSystem()
{

}

InputSystem::~InputSystem()
{

}

void InputSystem::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

	SDL_PumpEvents();
	int x, y;

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new PathToMessage(lastPos, pos);
			pGame->getMessageManager()->addMessage(pMessage, 0);
			lastPos = pos;
		}
	}

	//get keyboard state
	SDL_Event newEvent;
	SDL_PollEvent(&newEvent);
	if (newEvent.type == SDL_KEYDOWN)
	{		
		if (newEvent.key.keysym.sym == SDLK_ESCAPE)
		{
			std::cout << "ending the scene" << std::endl;
			gpGame->markForExit();
			return;
		}
	}
}