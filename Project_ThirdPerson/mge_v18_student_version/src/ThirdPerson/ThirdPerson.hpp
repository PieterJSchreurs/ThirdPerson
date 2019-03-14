#ifndef THIRDPERSON_HPP
#define THIRDPERSON_HPP

#include "mge/core/AbstractGame.hpp"
class GridGenerator;

class DebugHud;
class UIHandler;
class HudHandler;

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
		void loadLevel(std::string pFileName = "");
		void RestartLevel();
		void GoToMainMenu();
        //override initialize so we can add a DebugHud
        virtual void initialize();

	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();
		void destroyLevel();
		GridGenerator* _myGridGenerator = nullptr;

	    //override render to render the hud as well.
	    virtual void _render();

		virtual void _update(float pStep);

	private:
		DebugHud* _hud;                   //hud display
		UIHandler* _myUIHandler;
		HudHandler* _myHudHandler = nullptr;

        void _updateHud();
		void InitializeMainMenu();
		
		ThirdPerson(const ThirdPerson&);
		ThirdPerson& operator=(const ThirdPerson&);

		void initializeGameplayValues();

		std::string _fileName;
		std::string _tutorialLevel = "lvl1(lvl4)";

		const int _audioFileAmount = 42;
		std::string _allAudioFiles[43] =
		{
			"Hover.wav",
			"Click.wav",
			"Seagul1.wav",
			"Seagul2.wav",
			"Seagul3.wav",
			"Waves.wav",
			"Wind.wav",
			"CannonSplash.wav",
			"EnemyShipCannon.wav",
			"SmallShipCannon.wav",
			"BigShipCannon.wav",
			"Movement1.wav",
			"Movement2.wav",
			"Movement3.wav",
			"Movement4.wav",
			"Movement5.wav",
			"StartPlayer.wav",
			"StartEnemy.wav",
			"StartLevel.wav",
			"Treasure.wav",
			"Harbour.wav",
			"PlayerDetected.wav",
			"Victory.wav",
			"Defeat.wav",
			"AbandonShipEnemy.wav",
			"CannonVoiceEnemy.wav",
			"CannonVoicePlayer1.wav",
			"CannonVoicePlayer2.wav",
			"CannonVoicePlayer3.wav",
			"CannonVoicePlayer4.wav",
			"CannonVoicePlayer5.wav",
			"CannonVoicePlayer6.wav",
			"CannonVoicePlayer7.wav",
			"CannonVoicePlayer8.wav",
			"CannonVoicePlayer9.wav",
			"CannonVoicePlayer10.wav",
			"CannonVoicePlayer11.wav",
			"ImpactObstacles.wav",
			"ImpactSand.wav",
			"SinkingShipCannon.wav",
			"SinkingShipKraken.wav",
			"Kraken.wav",
			"Story.wav"
		};

		const int _meshFileAmount = 45;
		std::string _allMeshFiles[45] = 
		{
			"cube_flat.obj", 
			"Board.obj", 
			"Candle.obj",
			"coin1.obj",
			"coin2.obj" , 
			"coin3.obj",
			"Knife.obj",
			"Skull.obj",
			"actionCoin.obj",
			"Sand_Tile_1.obj",
			"Sand_Tile_2.obj",
			"Sand_Tile_3.obj",
			"Sand_Tile_Inverse_Corner_1.obj",
			"Sand_Tile_Inverse_Corner_2.obj",
			"Sand_Tile_Inverse_Corner_3.obj",
			"Sand_Tile_Straight_1.obj",
			"Sand_Tile_Straight_2.obj",
			"Sand_Tile_Straight_3.obj",
			"Sand_Tile_Corner_1.obj",
			"Sand_Tile_Corner_2.obj",
			"Sand_Tile_Corner_3.obj",
			"Harbour_Bottom_Left.obj",
			"Harbour_Middle_Left.obj",
			"Harbour_Top_Left.obj",
			"Harbour_Top_Middle.obj",
			"Harbour_Top_Right.obj",
			"Harbour_Middle_Right.obj",
			"Harbour_Bottom_Right.obj",
			"Obstacle_1.obj",
			"Obstacle_2.obj",
			"Obstacle_3.obj",
			"Obstacle_4.obj",
			"Obstacle_5.obj",
			"Obstacle_6.obj",
			"Obstacle_7.obj",
			"Obstacle_8.obj",
			"Obstacle_9.obj",
			"Treasure_Island.obj",
			"plane_8192.obj",
			"sphere_smooth.obj",
			"Main_Ship.obj",
			"Small_Ship.obj",
			"Enemy_Ship.obj",
			"Enemy_Ship_DMG.obj",
			"Kraken_Arm.obj"
		};
};

#endif // THIRDPERSON_HPP
