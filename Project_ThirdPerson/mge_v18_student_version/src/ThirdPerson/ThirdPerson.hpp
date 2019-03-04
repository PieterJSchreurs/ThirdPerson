#ifndef THIRDPERSON_HPP
#define THIRDPERSON_HPP

#include "mge/core/AbstractGame.hpp"
class GridGenerator;

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */

struct GameplayValues
{
	//Some constant gameplay variables__________________________________________
	int _gridWidth = 0;
	int _gridHeight = 0;
	float _tileSize = 0;

	int _bigShipSpeed = 0;
	int _bigShipRange = 0;
	int _bigShipDamage = 0;
	int _bigShipHealth = 0;
	int _smallShipSpeed = 0;
	int _smallShipRange = 0;
	int _smallShipDamage = 0;
	int _smallShipHealth = 0;
};
static GameplayValues _gameplayValues;

class ThirdPerson : public AbstractGame
{
    //PUBLIC FUNCTIONS
	public:
		ThirdPerson();
		virtual ~ThirdPerson();

        //override initialize so we can add a DebugHud
        virtual void initialize();

	protected:

	    //override so we can construct the actual scene
        virtual void _initializeScene();
		void loadLevel(std::string pFileName = "");
		void destroyLevel();
		GridGenerator* _myGridGenerator = nullptr;

	    //override render to render the hud as well.
	    virtual void _render();

		virtual void _update(float pStep);

	private:
		DebugHud* _hud;                   //hud display
	
        void _updateHud();

		ThirdPerson(const ThirdPerson&);
		ThirdPerson& operator=(const ThirdPerson&);

		void initializeGameplayValues();

		std::string _fileName;
};

#endif // THIRDPERSON_HPP
