#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#define NOMINMAX
#include <windows.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "GameData.h"

using namespace std;


struct Achievement {
	string name;
	vector<string> description;
	vector<string> extraRequirements;
	bool unlocked = false;
	vector<int> lockedItemsIds;

	Achievement();
	Achievement(string name, vector<string> desc, vector<string> extraRequirements, vector<int> lockedItemsList);
};


class DataManager
{
public:
	static const int achievementsNumber = 20;

private:
	Achievement achievements[achievementsNumber];

	int totalDiagonalFruitsEaten = 0;

	int expectedSaveFileSize = achievementsNumber * sizeof(bool) + sizeof(int) * 10;

	void initAchievements();

public:
	// DEFAULT VALUES IN restoreDefaultData() (called in the constructor)
	int savedGamemodeIndex;
	int savedSnakeSpeedIndex;
	int savedBoardSizeIndex;
	int savedFruitQuantityIndex;
	int savedSnakeLengtheningIndex;

	int savedSnakeColorId;
	int savedGradientColorId;
	int savedTileColorId;
	int savedFruitTextureId;
	int savedMusicTrackId;

	DataManager();

	const Achievement& getAchievementData(int index);
	int getAchievementIndexByLockedItemId(int id);
	vector<int> getLockedItems();

	bool unlockAchievement(int index);
	string checkAchievementUnlocks(GameData data);

	void saveSelectedSettings(int gamemodeIndex = 0, int snakeSpeedIndex = 4, int boardSizeIndex = 2, int fruitQuantityIndex = 0, int snakeLengtheningIndex = 0,
		int snakeColorId = 102, int gradientColorId = 213, int tileColorId = 307, int fruitTextureId = 401, int musicTrackId = 501);
	void saveData(string directory = SAVE_FILE_DIRECTORY);
	void loadData(string directory = SAVE_FILE_DIRECTORY);
	bool verifyLoadedData(int gamemodesNumber, int snakeSpeedOptionsNumber, int boardSizeOptionsNumber, int fruitQuantityOptionsNumber, int snakeLengtheningOptionsNumber);
	void restoreDefualtData(string directory = SAVE_FILE_DIRECTORY);
	void eraseData(string directory = SAVE_FILE_DIRECTORY);

};

