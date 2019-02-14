#include "ThirdPerson/Scripts/TileWorld.h"
#include "mge/core/World.hpp"

TileWorld::TileWorld(const int pColumns, const int pRows, float pTileSize, const std::string& aName, const glm::vec3& aPosition) : GameObject(aName, aPosition), _columns(glm::min(glm::max(pColumns, 7), 100)), _rows(glm::min(glm::max(pRows, 7), 100)), _tileSize(pTileSize)
{
	setLocalPosition(glm::vec3(-_columns * _tileSize + (_tileSize / 1.5f), 0, -_rows * _tileSize + (_tileSize / 1.5f)));

	//initialize all tiles to walkable
	//**_tileData = new bool[_columns];
	for (int i = 0; i < _columns; i++) {
		//*_tileData[i] = new bool[_rows];
		for (int j = 0; j < _rows; j++) {
			_tileData[i][j] = &trueHolder; //All tiles default to walkable
		}
	}
}

float TileWorld::width() {
	return _columns * _tileSize;
}

float TileWorld::height() {
	return _rows * _tileSize;
}

float TileWorld::tileSize() {
	return _tileSize;
}

int TileWorld::columns() {
	return _columns;
}

int TileWorld::rows() {
	return _rows;
}

void TileWorld::SetWalkable(int pColumn, int pRow, bool pWalkable) {
	if (pColumn < 0 || pColumn >= _columns || pRow < 0 || pRow >= _rows) {
		return;
	}

	if (pWalkable)
	{
		_tileData[pColumn][pRow] = &trueHolder;
	}
	else {
		_tileData[pColumn][pRow] = &falseHolder;
	}
}

bool TileWorld::GetWalkable(int pColumn, int pRow) {
	if (pColumn < 0 || pColumn >= _columns || pRow < 0 || pRow >= _rows) {
		return false;
	}

	return *_tileData[pColumn][pRow];
}

TileWorld::~TileWorld() {
}


