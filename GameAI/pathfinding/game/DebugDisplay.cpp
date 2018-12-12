#include "DebugDisplay.h"
#include "GraphicsBuffer.h"
#include "DebugContent.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "Color.h"
#include "GameApp.h"
#include "UnitManager.h"
#include "Unit.h"

#include <sstream>

using namespace std;

DebugDisplay::DebugDisplay( const Vector2D& pos, DebugContent* pContent )
	:mPos(pos)
	,mpContent(pContent)
{
}

DebugDisplay::~DebugDisplay()
{
	delete mpContent;
}

void DebugDisplay::draw( GraphicsBuffer* pBuffer )
{
	if (gpGame->getLostState())
	{
		string gameOverDisplay;
		std::stringstream gameOverText;
		string pressButtonDisplay;
		std::stringstream pressButtonText;
		gameOverText << "GAME OVER";
		gameOverDisplay = gameOverText.str();
		pressButtonText << "PRESS ESC TO EXIT";
		pressButtonDisplay = pressButtonText.str();
		gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX() + 350, mPos.getY() +300, gameOverDisplay, RED_COLOR);
		gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX() + 350, mPos.getY() + 350, pressButtonDisplay, RED_COLOR);
	}
	{
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);

		string scoreDisplay; // = mpContent->getDebugString();
		string highScoreDisplay;
		std::stringstream scoreText;
		std::stringstream highScoreText;
		string livesDisplay;
		std::stringstream livesText;

		scoreText << "Score: " << gpGame->getScore();
		highScoreText << "High Score: " << gpGame->getHighScore();
		livesText << "Lives: " << pGame->getUnitManager()->getUnit(0)->getHealth();


		scoreDisplay = scoreText.str();
		highScoreDisplay = highScoreText.str();
		livesDisplay = livesText.str();

		gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX(), mPos.getY() - 4, scoreDisplay, YELLOW_COLOR);
		gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX() + 600, mPos.getY() - 4, highScoreDisplay, YELLOW_COLOR);
		gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX() + 300, mPos.getY() - 4, livesDisplay, YELLOW_COLOR);

	}

}
