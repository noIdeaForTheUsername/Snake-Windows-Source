#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "SETTINGS.h"

enum class FacingDirection {Up = 1, Right, Down, Left};

class Snake
{
private:

	bool loadSuccessful = true;
	float tileWidth;
	float tileHeight;
	bool canChangeDirection = true;

	sf::Texture headTexture;
	sf::Texture pausedHeadTexture;
	sf::Color finalEndingColor;
	sf::Color* endingColors;

	FacingDirection getOppositeFacingDirection(FacingDirection facingDirection);
	void updateEndingColor();
	sf::Color getRandomColor(int minAlpha = 255);

	int minGradientLength;
	bool gradientEnabled = false;
	bool randomizeHeadColor = false;
	bool randomizeEndingColor = false;
	bool oppositeGradientColor = false;

	void updateRainbowColors();
	void updateEmergencyColors(int currentFrameNumber);

	sf::Color emergencyColor1 = sf::Color(220, 16, 16);
	sf::Color emergencyColor2 = sf::Color(15, 20, 150);

public:
	Snake(sf::Vector2f headSize, int startX, int startY,
 int startLength,
		FacingDirection startFacingDirection = FacingDirection::Right,
 int minGradientLength = MIN_SNAKE_COLOR_TRANSITION,
		bool gradientEnabled = true);

	sf::Sprite headSprite;

	int headX;
	int headY;
	int length = 2;

	bool rainbow = false;
	bool emergencyColors = false;
	
	sf::Color headColor;
	sf::Color endingColor;
	
	FacingDirection facingDirection;
	FacingDirection queuedFacingDirection;

	void setColors(sf::Color beginColor, sf::Color finalEndingColor);
	void setRandomColors();
	void setRandomizeColors(bool headColor, bool endingColor);
	void setEmergencyColors();
	void setRainbow();
	void setOppositeGradientColor();
	bool getLoadSuccessful();

	void updateFacingDirection(FacingDirection newFacingDirection);
	void changeLength(int units = 1);
	sf::Vector2i move(int tiles = 1);
	void setPausedTexture(bool paused = true);
	void teleportHead(int newX, int newY, FacingDirection newFacingDirection);
	void update(int currentFrameNumber);
};

