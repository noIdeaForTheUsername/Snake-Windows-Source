#pragma once
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <cmath>

#include "Fruit.h"
#include "SETTINGS.h"

enum class TileStatus { Empty = 0, Snake, Fruit, Wall };

class Tile
{
private:
	sf::Color emptyColor;
	static sf::Texture texture;
	static sf::Texture wallTexture;

	void updateSpriteTexture(sf::Texture* newTexture, float width = -1.f, float height = -1.f);
	void placeWall();
public:
	Tile(float posX, float posY, float width, float height, sf::Color tileColor);

	// Variables
	sf::Sprite sprite;
	TileStatus status;
	Fruit* fruitPtr = nullptr;
	int snakeRemaining;
	bool markedForWallPlacement = false;
	
	// Functions
	TileStatus placeSnakeTile(int snakeLengthOnTile, int totalSnakeLength, sf::Color beginColor = SNAKE_COLOR_HEAD, sf::Color endColor = SNAKE_COLOR_ENDING);
	bool placeFruitTile(Fruit* fruitPtr);
	void removeFruit(bool playSound = true);
	bool markForWallPlacement();
	
	void lengthenSnakeOnTile(int units = 1);
	void moveSnakeOnTile(int snakeLength, sf::Color beginColor, sf::Color endColor);
	void updateColor(int snakeLength, sf::Color beginColor, sf::Color endColor);


	// Static elements
	static bool assetsLoaded;
	static sf::SoundBuffer wallSoundBuffer;
	static sf::Sound wallSound;
	static void loadAssets();

	static bool invisibleSnake;
	static int tileInvisibilityBegin;
	static void setSnakeInvisibility(bool invisibility = true, int tilesVisible = 3);
};
