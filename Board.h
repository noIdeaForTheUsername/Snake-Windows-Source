#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#define NOMINMAX
#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "SETTINGS.h"
#include "Tile.h"
#include "Snake.h"
#include "Fruit.h"
#include "TileIndicator.h"
#include "GameData.h"

using namespace std;

class Board
{
private:
	// Init
	void initTiles();

	// Drawing
	template <typename RenderObject>
	void drawTiles(RenderObject& renderObject);
	
	// PRIVATE FUNCTIONS

	// GM: Inverting Snake
	void invertSnake();
	// GM: Moving Fruits
	void checkFruitPositionsCorrect();
	void moveFruits();
	// GM: Portal
	void teleportSnakeThroughPortal(int portalNumber);
	void checkPortalRotationCorrect();
	void removePortalsOnSnakeHead();
	bool checkPortalModeFruitPositionCorrect(int x, int y, int distance);
	// GM: Time Travel
	void manageSavedBoardSnakeLengths();
	void saveBoardSnakeLengths(int presentPoint = 0);
	void reassignTimeTravelDestinationToFruit(int timeTravelNumber);
	void loadSavedBoardState(int arrayIndex = 0);
	void performTimeTravel(Fruit* fruitPtr);
	void replaceTimeTravelIndicators(int timeTravelNumber = 1);
	// GM: Wall
	void handleWallPlacing();
	bool placeWallTile(int x, int y);

	// All gamemodes
	void moveSnakeOnTiles();
	void moveFruitToRandomPosition(Fruit* fruitToMovePtr);
	void lengthenSnake(int units = 1);
	void lengthenSnakeOnTiles(int units = 1);
	void updateTileColors();
	bool checkGameOverInXMoves(int moves = 1);
	bool checkCoordinatesInBoardRange(sf::Vector2i coordinates);
	bool checkCoordinatesInBoardRange(int x, int y);
	FacingDirection getSnakeFacingDirectionByHeadTile(int tileX, int tileY, bool reversed = false);
	int getFreeSpacesAroundTile(int tileX, int tileY);
	int getTileNumberByStatus(TileStatus status);

	// Variables
	bool loadSuccessful = true;
	bool isGameOver = false;
	bool isBoardFull = false;
	float snakeMovesPerSecond = 3.f;
	Gamemode gamemode = Gamemode::Normal;
	int snakeMovementPauseUpdates = 0;
	int maxSnakeMovementPauseUpdates = 1;
	int points = 0;
	int snakeLengthening = 1;
	float width;
	float height;
	int rows;
	int columns;
	int currentGameFrameNumber = 0;
	int diagonalFruitsEaten = 0;
	int lastFruitCollectionTime = 0.f;
	sf::Color tileColor1;
	sf::Color tileColor2;
	std::vector<Fruit*> fruits;

	// Gamemode-specific variables
	std::vector<TileIndicator*> portals;
	std::vector<TileIndicator*> timeTravelIndicators;
	std::vector<TileIndicator*> wallIndicators;
	int wallToPlace = 0;
	sf::Color iconColors[5] = { sf::Color(255, 0, 0), sf::Color(0, 255, 0), sf::Color(0, 0, 255), sf::Color(0, 255, 255), sf::Color(255, 0, 255) }; // Red, Green, Blue, Cyan, Magenta
	TileIndicator* lastPortalPlaced = nullptr;
	FacingDirection lastSnakeFacingDirection;

	int moveCounter = 0;
	std::vector<int**> savedBoardSnakeLengths;  // Vector - 0 is the present or furthest future, the end of the array is the furthest past.
	int timeTravelDestinations[5];  // An array with a length of 5 (the max number of fruits in this gamemode)
	int presentPoint = -1; 
	// -1 means that the current board state is the newest future, where player has never been before, and saving it means adding a new element to the savedBoardSnakeLengths
	// smaller numbers are later in future, higher numbers are earlier in past
	bool checkTimeloopAchievement = false;
	bool unlockTimeloopAchievement = false;
	map<int, int> timepointsVisitCounters;

public:
	Board(float boardWidth = BOARD_WIDTH, float boardHeight = BOARD_HEIGHT, int boardRows = 13,
		int boardColumns = 13, sf::Color tileColor1 = sf::Color(0, 90, 255, 255), sf::Color tileColor2 = sf::Color(0, 60, 180, 255));
	~Board();


	sf::RenderTexture renderTexture;
	float tileWidth;
	float tileHeight;
	
	// Objects
	sf::Sprite sprite;
	Snake* snake = nullptr;
	Tile*** tiles; // tiles[BOARD_COLUMNS][BOARD_ROWS]

	// Functions
	void setGamemode(Gamemode gamemode, int boardUpdateFrames, sf::Font * fontPtr);
	void setCheckTimeloopAchievement(bool value = true);
	bool placeSnake(int startLength, FacingDirection facingDirection = FacingDirection::Right,
		sf::Color beginColor = SNAKE_COLOR_HEAD, sf::Color endingColor = SNAKE_COLOR_ENDING, int minGradientLength = MIN_SNAKE_COLOR_TRANSITION,
		bool rainbow = false, bool oppositeGradientColor = false, bool emergencyColors = false);
	void setSnakeColorRandomization(bool beginColor = true, bool endColor = true);
	void setSnakeLengthening(int tilesPerFruit);
	Fruit* placeNewFruit(int textureIndex = 0);
	void gameOver();

	
	void moveSnake();
	void update(int currentFrameNumber);
	void updateSnakeFacingDirection(FacingDirection newFacingDirection);

	template <typename RenderObject>
	void draw(RenderObject& renderObject);

	bool getLoadSuccessful();
	sf::Vector2f getSize();
	bool getGameOver();
	bool getBoardFull();
	int getPoints();
	int getMoveCounter();
	GameData getGameData();
};

template <typename RenderObject>
void Board::drawTiles(RenderObject& renderObject) {
	for (int i = 0; i < this->columns; i++) {
		for (int j = 0; j < this->rows; j++) {
			renderObject.draw(this->tiles[i][j]->sprite);
		}
	}
}

template <typename RenderObject>
void Board::draw(RenderObject& renderObject) {
	this->drawTiles(renderObject);

	if (this->gamemode == Gamemode::TimeTravel) {
		for (auto indicator : this->timeTravelIndicators) {
			indicator->draw(renderObject);
		}
	}
	else if (this->gamemode == Gamemode::Wall) {
		for (auto indicator : this->wallIndicators) {
			indicator->draw(renderObject);
		}
	}

	if (this->snake != nullptr) renderObject.draw(this->snake->headSprite);

	if (this->gamemode == Gamemode::Portal) {
		for (auto portal : this->portals) {
			portal->draw(renderObject);
		}
	}

	for (auto element : this->fruits) {
		element->draw(renderObject);
	}
}
