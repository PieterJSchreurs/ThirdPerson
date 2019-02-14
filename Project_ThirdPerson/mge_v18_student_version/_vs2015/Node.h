#ifndef NODE_HPP
#define NODE_HPP

#include "glm.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class Node
{
public:

	Node(const int pColumns = 5, const int pRows = 5, float pTileSize = 2.0f);
	virtual ~Node();

	int width();
	int height();

	int rows();
	int columns();
	float tileSize();

	void SetWalkable(int pColumn, int pRow, bool pWalkable);
	bool GetWalkable(int pColumn, int pRow);

private:


	const int _columns;							//the amount of columns
	const int _rows;							//the amount of rows
	float _tileSize;							//the size of one gridcell

	//Canvas _gridCanvas;						//canvas used to draw gridlines on
	//Canvas _tileCanvas;						//canvas used to show walkable tile state on

	bool* _tileData[][1];						//stores walkable flags for each cell
};

#endif // NODE_HPP
