#ifndef TILEWORLD_HPP
#define TILEWORLD_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class TileWorld : public GameObject
{
public:

	TileWorld(const int pColumns = 5, const int pRows = 5, float pTileSize = 2.0f, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~TileWorld();

	void SetTileWorldValues(TileWorld* pOther);

	float width();
	float height();

	int rows();
	int columns();
	float tileSize();

	void SetWalkable(int pColumn, int pRow, bool pWalkable);
	bool GetWalkable(int pColumn, int pRow);

private:

	bool falseHolder = false;
	bool trueHolder = true;

	int _columns;							//the amount of columns
	int _rows;							//the amount of rows
	float _tileSize;							//the size of one gridcell

	//Canvas _gridCanvas;						//canvas used to draw gridlines on
	//Canvas _tileCanvas;						//canvas used to show walkable tile state on

	bool* _tileData[100][100];						//stores walkable flags for each cell
	//bool** _tileData[10];
};

#endif // TILEWORLD_HPP
