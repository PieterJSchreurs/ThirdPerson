#ifndef GRIDGENERATOR_HPP
#define GRIDGENERATOR_HPP

#include "glm.hpp"
#include "ThirdPerson/Scripts/Ship.h"
#include "ThirdPerson/Scripts/Node.h"
#include "ThirdPerson/Scripts/NodeWorld.h"
#include "ThirdPerson/Scripts/TileWorld.h"
#include "mge/core/Mesh.hpp"

/**
* Exercise for the student: implement the Light class...
* Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
*/
class GridGenerator : public GameObject
{
public:
	GridGenerator(TileWorld& pTileWorld, const std::string& pFileName, const std::string& aName = "", const glm::vec3& aPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual ~GridGenerator();

	void GenerateNodeGraph();
	void DestroyNodeGraph();

	Node* GetNodeAtTile(int pColumn, int pRow);

	std::vector<Node*> GetAllNodesOfType(Node::TerrainTypes type);

	std::vector<Node*> GetAllNodes();

	int getGridWidth();
	int getGridHeight();

	std::vector<Ship*> GetPlayerShips();
	std::vector<Ship*> GetAIShips();

private:
	enum tileTypes {
		fullTile = 0,
		straightTile = 1,
		cornerTile = 2,
		cornerInverseTile = 3,
		single = 4,
	};
	struct neighbourTiles {
		bool _north = false;
		bool _northEast = false;
		bool _east = false;
		bool _southEast = false;
		bool _south = false;
		bool _southWest = false;
		bool _west = false;
		bool _northWest = false;

		bool _tiles[8] = { _north, _northEast, _east, _southEast, _south, _southWest, _west, _northWest };

		glm::vec2 _direction = glm::vec2(0, 0);
		int _directionIndex = 0;
		tileTypes _type = tileTypes::single;

		void UpdateTilesArray() {
			_tiles[0] = _north;
			_tiles[1] = _northEast;
			_tiles[2] = _east;
			_tiles[3] = _southEast;
			_tiles[4] = _south;
			_tiles[5] = _southWest;
			_tiles[6] = _west;
			_tiles[7] = _northWest;
		}
		void SetDirection(int pDir) {
			_directionIndex = pDir % 8;
			if (_directionIndex == 0) //North
			{
				_direction = glm::vec2(0, -1);
			}
			else if (_directionIndex == 1) //North-east
			{
				_direction = glm::vec2(1, -1);
			}
			else if (_directionIndex == 2) //East
			{
				_direction = glm::vec2(1, 0);
			}
			else if (_directionIndex == 3) //South-east
			{
				_direction = glm::vec2(1, 1);
			}
			else if (_directionIndex == 4) //South
			{
				_direction = glm::vec2(0, 1);
			}
			else if (_directionIndex == 5) //South-west
			{
				_direction = glm::vec2(-1, 1);
			}
			else if (_directionIndex == 6) //West
			{
				_direction = glm::vec2(-1, 0);
			}
			else if (_directionIndex == 7) //North-west
			{
				_direction = glm::vec2(-1, -1);
			}
		}

		bool GetTilesInARow(int pAmount, int pCornerAmount) {
			UpdateTilesArray();
			int counter = 0;
			int cornerCounter = 0;
			bool fullCheck = false;
			for (int i = 0; i < 8; i++)//Check all the tiles in clockwise order, starting at north.
			{
				if (_tiles[i]) //If the tile is of the desired type
				{
					counter++; //Add to the counter
					if (i % 2 == 1)
					{
						cornerCounter++;
					}
					if ((counter == pAmount && cornerCounter == pCornerAmount) || counter > pAmount) //If there are pAmount tiles connected in a row
					{
						SetDirection(i - (glm::floor(pAmount / 2.0f)));
						return true;//Return true
					}
					else if (i >= 7 && !fullCheck)//If there are no pAmount tiles in a row yet after having checked all the tiles, but there is a chain going, recheck north to see if it connects counter clockwise.
					{
						fullCheck = true;
						for (int j = 0; j < 8; j++)
						{
							if (_tiles[j]) //If the tile is of the desired type
							{
								counter++; //Add to the counter
								if (j % 2 == 1)
								{
									cornerCounter++;
								}
								if ((counter == pAmount && cornerCounter == pCornerAmount) || counter > pAmount) //If there are pAmount tiles connected in a row
								{
									SetDirection(((i + j + 1) - (int)(glm::floor(pAmount / 2.0f))) % 8);
									return true;//Return true
								}
							}
							else //If the chain is broken, there are no pAmount tiles in a row.
							{
								return false;
							}
						}
					}
				}
				else 
				{
					counter = 0;
					cornerCounter = 0;
				}
			}
			return false;
		}
	};

	neighbourTiles getNeighbourTiles(int pNodeX, int pNodeY, std::vector<int> pAllNodes, Node::TerrainTypes pType, Node::TerrainTypes pType2 = Node::TerrainTypes::empty);

	Mesh* _cubeMeshDefault;
	Mesh* _cubeFullMeshDefault;
	Mesh* _cubeInvertedCornerMeshDefault;
	Mesh* _cubeStraightMeshDefault;
	Mesh* _cubeCornerMeshDefault;
	Mesh* _obstacleMeshDefault;
	Mesh* _planeMeshDefault;
	Mesh* _suzannaMeshDefault;
	Mesh* _teapotMeshDefault;
	Mesh* _sphereMeshDefault;

	void PlaceCorrectIslandNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes);
	void PlaceCorrectHarborNode(Node* pNode, int pColumn, int pRow, std::vector<int> pAllNodes);

	TileWorld& _tileWorld;		//the tileworld to inspect
	NodeWorld* _nodeWorld;		//the nodeworld to generate

	Node* _nodeCache[100][100];	//The map can not be bigger than 100 by 100!
	int _gridWidth;
	int _gridHeight;
	float _tileGap = 0.0f;

	std::vector<Ship*> _playerShips;
	std::vector<Ship*> _AIShips;

	std::string _fileName;
};

#endif // GRIDGENERATOR_HPP
