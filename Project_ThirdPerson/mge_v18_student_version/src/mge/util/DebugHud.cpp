#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <SFML/Graphics/Text.hpp>
#include "mge/util/DebugHud.hpp"
#include "mge/config.hpp"

DebugHud::DebugHud(sf::RenderWindow * aWindow) : _window(aWindow), _debugInfo(), _font(), _debugText()
{
	assert(_window != NULL);

	if (!_font.loadFromFile(config::MGE_FONT_PATH + "piratesfont.otf")) {
		std::cout << "Could not load font, exiting..." << std::endl;
		return;
	}
	
	_createDebugHud();
}

DebugHud::~DebugHud()
{
	//dtor
}

void DebugHud::_createDebugHud() {
	_debugText.setString("");
	_debugText.setFont(_font);
	_debugText.setCharacterSize(40);
	_debugText.setFillColor(sf::Color::White);
}

void DebugHud::setDebugInfo(std::string pInfo, int xPos, int yPos) {
	_debugText.setString(pInfo);
	_debugText.setPosition(xPos, yPos);
}

void DebugHud::draw()
{
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	_window->draw(_debugText);
	_window->popGLStates();
}
