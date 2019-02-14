#include "LuaScripting/Scripts/Node.h"
#include "mge/core/World.hpp"

Node::Node(const int pColumns, const int pRows, float pTileSize) : _columns(pColumns), _rows(pRows), _tileSize(pTileSize)
{
	//initialize all tiles to walkable
	**_tileData = new bool[_columns];
	for (int i = 0; i < _columns; i++) {
		*_tileData[i] = new bool[_rows];
		for (int j = 0; j < _rows; j++) {
			*_tileData[i][j] = true; //All tiles default to walkable
		}
	}
}

int Node::width() {
	return _columns * _tileSize;
}

int Node::height() {
	return _rows * _tileSize;
}

float Node::tileSize() {
	return _tileSize;
}

int Node::columns() {
	return _columns;
}

int Node::rows() {
	return _rows;
}

void Node::SetWalkable(int pColumn, int pRow, bool pWalkable) {
	if (pColumn < 0 || pColumn >= _columns || pRow < 0 || pRow >= _rows) return;

	*_tileData[pColumn][pRow] = pWalkable;
}

bool Node::GetWalkable(int pColumn, int pRow) {
	if (pColumn < 0 || pColumn >= _columns || pRow < 0 || pRow >= _rows) return false;
	return _tileData[pColumn, pRow];
}

Node::~Node() {
}


