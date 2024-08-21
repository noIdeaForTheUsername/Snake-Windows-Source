#pragma once

#include "SETTINGS.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


// struct used to save game data that is required to check if an achievement can be unlocked

struct GameData {
	// Game settings list part
	Gamemode gamemode;
	int boardUpdateFrames;
	int boardSize;
	int fruitQuantity;
	int snakeLengthening;

	// Board part
	int points;
	int occupiedTiles;  // Snake and Wall tiles
	int movesDone = 0;
	float timer; // *Timer is set in Game, between getting the data from Board and sending it to Menu (and then DataManager)
	float lastFruitCollectionTime;
	bool gameOver = false;
	int diagonalFruitsEaten = 0;
	bool unlockTimeloopAchievement = false;
	bool unlockErrorAchievement = false;
};
