#ifndef DEBUGHUD_HPP
#define DEBUGHUD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

/**
 * Very simple class capable of rendering a simple piece of text through SFML.
 */
class DebugHud
{
	public:
		DebugHud( sf::RenderWindow * aWindow );
		virtual ~DebugHud();
		virtual void draw();

		void setDebugInfo (std::string pInfo, int xPos = 10, int yPos = 10);

	protected:
		sf::RenderWindow * _window;

	private:
        std::string _debugInfo;

        sf::Font _font;
		sf::Image _testimage;
		sf::Texture _texture;

        sf::Text _debugText;

        void _createDebugHud();

        DebugHud(const DebugHud&);
        DebugHud& operator=(const DebugHud&);

};

#endif // DEBUGHUD_HPP
