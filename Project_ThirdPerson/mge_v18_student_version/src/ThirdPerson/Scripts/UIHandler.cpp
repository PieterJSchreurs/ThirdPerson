#include "UIHandler.h"

UIHandler::UIHandler(sf::RenderWindow * pWindow, const std::string& aName, const glm::vec3& aPosition) :GameObject(aName, aPosition), _window(pWindow)
{

}

//void UIHandler::createSprite(sf::Sprite* pSprite, float xCoordinate, float yCoordinate)
//{
//
//}



UIHandler::~UIHandler()
{
}