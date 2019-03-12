#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "glm.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "ThirdPerson/ThirdPerson.hpp"

class MainMenu : public GameObject
{
public:
	MainMenu(ThirdPerson* pThirdPerson, sf::RenderWindow* pRenderWindow, std::vector<std::string> pFileNames,const std::string & aName, const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~MainMenu();
	virtual void update(float pStep);

protected:
	sf::RenderWindow * _window;

private:
	enum MenuStyles {
		SPLASH,
		MAIN,
		LEVELSELECT,
		CREDIT
	};

	enum ButtonStyle {
		NORMAL,
		HOVER,
		CLICK
	};

	void FillTextures();
	void BindSpritesToTextures();
	void PositionSprites();
	bool _isMainMenuInitialized;
	
	MenuStyles _currentMenuStyle = SPLASH;

	float _timer = 0;
	float _playerInputDelay = 0.5f;
	float _lastPlayerInput = 0;
	bool _levelSelectLoaded = false;
	bool _isLevelLoading = false;
	std::string _levelSelected;

	std::vector<std::string> _fileNames;
	sf::Font _font;
	sf::Text _levelText;
	std::vector<sf::Sprite> _spritesToDraw;
	std::vector<sf::Text> _texts;
	sf::Sprite _backPlane;
	sf::Texture _levelSelectTexture;
	sf::Sprite _levelSelectButton;
	sf::Texture _backPlaneTexture;
	ThirdPerson* _thirdPerson;
	
#pragma region Textures and sprites
	//Sprites menus & backgrounds
	sf::Sprite _backGroundNormalSprite;
	sf::Sprite _backGroundBlurredSprite;
	sf::Sprite _splashScreenTitleSprite;
	sf::Sprite _splashScreenAnyButtonSprite;

	//Loading Screen
	sf::Sprite _loadingScreenChapture1Sprite;
	sf::Sprite _loadingScreenChapture2Sprite;
	sf::Sprite _loadingScreenChapture3Sprite;
	sf::Sprite _loadingScreenChapture4Sprite;
	sf::Sprite _loadingScreenChapture5Sprite;
	sf::Sprite _loadingScreenChapture6Sprite;

	//Sprites buttons
	//Main Menu
	sf::Sprite _startButtonSprite;
	sf::Sprite _continueButtonSprite;
	sf::Sprite _chapterButtonSprite;
	sf::Sprite _creditsButtonSprite;
	sf::Sprite _quitButtonSprite;
	sf::Sprite _ropeSprite;

	//Textures menus & backgrounds
	sf::Texture _backGroundNormalTexture;
	sf::Texture _backGroundBlurredTexture;
	sf::Texture _splashScreenTitleTexture;
	sf::Texture _splashScreenAnyButtonTexture;

	//Loading Screen
	sf::Texture _loadingScreenChapture1Texture;
	sf::Texture _loadingScreenChapture2Texture;
	sf::Texture _loadingScreenChapture3Texture;
	sf::Texture _loadingScreenChapture4Texture;
	sf::Texture _loadingScreenChapture5Texture;
	sf::Texture _loadingScreenChapture6Texture;

	//Textures buttons
	//Main Menu
	sf::Texture _startButtonTexture;
	sf::Texture _continueButtonTexture;
	sf::Texture _chapterButtonTexture;
	sf::Texture _creditsButtonTexture;
	sf::Texture _quitButtonTexture;
	sf::Texture _ropeTexture;

#pragma endregion

	//Load Menu

	void HandleClick();
	void LoadLevelSelect();
	void LoadScene(std::string pString);
	void DrawSprites();
	void DrawMenuStyle(MenuStyles pMenuStyle);
	void StartLoadingScene(std::string pString);

	void ChangeSprite(sf::Sprite* pSprite, ButtonStyle pButtonStyle, int pIndex);
	void CheckForMouse();
};
#endif
