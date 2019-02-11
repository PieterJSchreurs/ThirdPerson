#ifndef LuaScripting_HPP
#define LuaScripting_HPP

#include "mge/core/AbstractGame.hpp"
#include "LuaScripting/Scripts/Worker.h"

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class LuaScripting : public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		LuaScripting();
		virtual ~LuaScripting();

        //override initialize so we can add a DebugHud
        virtual void initialize();

	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();

		LuaScripting(const LuaScripting&);
		LuaScripting& operator=(const LuaScripting&);

		Worker::GameplayValues _gameplayValues;
		void initializeGameplayValues();
};

#endif // LuaScripting_HPP
