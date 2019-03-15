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
		CREDIT, 
		QUIT
	};

	enum ButtonStyle {
		NORMAL,
		HOVER,
		CLICK, 
		LOCKED
	};

	void FillTextures();
	void BindSpritesToTextures();
	void PositionSprites();
	bool _isMainMenuInitialized;
	bool _changeScene = false;
	void CheckForNarrative();
	bool _isNarrativeActive = false;
	
	MenuStyles _currentMenuStyle = SPLASH;

	float _timer = 0;
	float _timer2 = 0;
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
	std::vector<sf::Sprite> _levelList;
	std::vector<sf::Sprite> _loadingScreens;
	std::vector<bool> _levelUnlocked;
	bool trueHolder = true;
	bool falseHolder = false;
	int _indexNarrative = 0;

	sf::Sprite _backgroundNarrativeSprite;
	sf::Texture _backgroundNarrativeTexture;
	sf::Sprite _narrativeTextSprite;
	sf::Texture _narrativeTextTexture;
	
#pragma region Textures and sprites
	//Sprites menus & backgrounds
	sf::Sprite _backGroundNormalSprite;
	sf::Sprite _backGroundBlurredSprite;
	sf::Sprite _splashScreenTitleSprite;
	sf::Sprite _splashScreenAnyButtonSprite;
	sf::Sprite _levelSelectTemplateSprite;

	//Loading Screen
	sf::Sprite _loadingScreenChapture1Sprite;
	sf::Sprite _loadingScreenChapture2Sprite;
	sf::Sprite _loadingScreenChapture3Sprite;
	sf::Sprite _loadingScreenChapture4Sprite;
	sf::Sprite _loadingScreenChapture5Sprite;
	sf::Sprite _loadingScreenChapture6Sprite;

	//Credits
	sf::Sprite _creditScreenSprite;

	//Level Select
	sf::Sprite _chapter1Sprite;
	sf::Sprite _chapter2Sprite;
	sf::Sprite _chapter3Sprite;
	sf::Sprite _chapter4Sprite;
	sf::Sprite _chapter5Sprite;
	sf::Sprite _chapter6Sprite;
	sf::Sprite _chapterIndicatorTextSprite;

	sf::Sprite _chapterArrowLeftSprite;
	sf::Sprite _chapterArrowRightSprite;
	//Sprites buttons
	//Main Menu
	sf::Sprite _startButtonSprite;
	sf::Sprite _continueButtonSprite;
	sf::Sprite _chapterButtonSprite;
	sf::Sprite _creditsButtonSprite;
	sf::Sprite _quitButtonSprite;
	sf::Sprite _ropeSprite;
	sf::Sprite _backButtonSprite;

	//Textures menus & backgrounds
	sf::Texture _backGroundNormalTexture;
	sf::Texture _backGroundBlurredTexture;
	sf::Texture _splashScreenTitleTexture;
	sf::Texture _splashScreenAnyButtonTexture;
	sf::Texture _creditsScreenTexture;
	sf::Texture _levelSelectTemplateTexture;

	//Loading Screen
	sf::Texture _loadingScreenChapture1Texture;
	sf::Texture _loadingScreenChapture2Texture;
	sf::Texture _loadingScreenChapture3Texture;
	sf::Texture _loadingScreenChapture4Texture;
	sf::Texture _loadingScreenChapture5Texture;
	sf::Texture _loadingScreenChapture6Texture;

	//Level Select
	sf::Texture _chapter1Texture;
	sf::Texture _chapter2Texture;
	sf::Texture _chapter3Texture;
	sf::Texture _chapter4Texture;
	sf::Texture _chapter5Texture;
	sf::Texture _chapter6Texture;
	sf::Texture _chapterIndicatorTextTexture;

	sf::Texture _chapterArrowLeftTexture;
	sf::Texture _chapterArrowRightTexture;

	//Textures buttons
	//Main Menu
	sf::Texture _startButtonTexture;
	sf::Texture _continueButtonTexture;
	sf::Texture _chapterButtonTexture;
	sf::Texture _creditsButtonTexture;
	sf::Texture _quitButtonTexture;
	sf::Texture _ropeTexture;
	sf::Texture _backButtonTexture;



#pragma endregion

	//Load Menu

	void HandleClick();
	void LoadLevelSelect();
	void LoadScene(std::string pString);
	void DrawSprites();
	void DrawMenuStyle(MenuStyles pMenuStyle);
	void StartLoadingScene(std::string pString);

	void ChangeSprite(sf::Sprite* pSprite, ButtonStyle pButtonStyle, int pIndex);
	std::string GetLevelBySprite(sf::Sprite pSprite);
	bool IsLevelUnLocked(sf::Sprite pSprite);
	void UnlockLevel(int pIndex, bool pToggle);
	void CheckForMouse();
};
#endif
