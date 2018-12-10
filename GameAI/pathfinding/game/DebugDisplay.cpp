#include "DebugDisplay.h"
#include "GraphicsBuffer.h"
#include "DebugContent.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "Color.h"

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
	string scoreDisplay; // = mpContent->getDebugString();
	string highScoreDisplay; 
	std::stringstream scoreText;
	std::stringstream highScoreText;

	scoreText << "Score: " << gpGame->getScore();
	highScoreText << "High Score: " << gpGame->getHighScore();

	scoreDisplay = scoreText.str();
	highScoreDisplay = highScoreText.str();

	gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX(), mPos.getY()-4, scoreDisplay, YELLOW_COLOR);
	gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX()+600, mPos.getY()-4, highScoreDisplay, YELLOW_COLOR);

}
