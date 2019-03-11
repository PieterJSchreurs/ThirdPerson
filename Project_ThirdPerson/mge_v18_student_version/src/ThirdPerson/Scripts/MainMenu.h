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

private:
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
	std::vector<sf::Sprite> _sprites;
	sf::Sprite _backPlane;
	sf::Texture _levelSelectTexture;
	sf::Sprite _levelSelectButton;
	sf::Texture _backPlaneTexture;
	sf::RenderWindow* _renderWindow;
	ThirdPerson* _thirdPerson;

	sf::Sprite _loadingScreen;
	sf::Texture _loadingScreenTexture;

	void HandleClick();
	void LoadLevelSelect();
	void LoadScene(std::string pString);
	void FillMainMenu();
	void DrawSprites();
	void StartLoadingScene(std::string pString);
};
#endif
