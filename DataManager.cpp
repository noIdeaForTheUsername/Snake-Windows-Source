#include "DataManager.h"

Achievement::Achievement() { }
Achievement::Achievement(string name, vector<string> desc, vector<string> extraRequirements, vector<int> lockedItemsList)
	: name(name), description(desc), extraRequirements(extraRequirements), unlocked(false), lockedItemsIds(lockedItemsList) { }


DataManager::DataManager() {
	this->initAchievements();
	this->restoreDefualtData();
}

void DataManager::initAchievements() {
	this->achievements[0] = Achievement("First Steps", { "Get 15 points." }, {}, {105, 305});
	this->achievements[1] = Achievement("100", { "Get 100 points." }, {"Snake Speed 4-9", "Board Size: 11-17", "Fruits: 1-3"}, {405, 505});
	this->achievements[2] = Achievement("Fighting Hunger", { "Make the Snake long enough to cover", "20% of the total space on the board." },
		{"Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1"}, {106, 306, 503, 404});
	this->achievements[3] = Achievement("Gaining Weight", { "Make the Snake long enough to cover", "35% of the total space on the board." },
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 201, 202, 203, 204, 205, 310 });
	this->achievements[4] = Achievement("Expansion", { "Make the Snake long enough to cover", "50% of the total space on the board." },
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 206, 207, 208, 209, 111, 211, 311 });
	this->achievements[5] = Achievement("Megaexpansion", { "Make the Snake long enough to cover", "70% of the total space on the board." },
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 112, 212, 312 });
	this->achievements[6] = Achievement("Chillout", { "Get 35 points with the", "Snake speed at 1." }, {"Fruits: 1-3"}, {303});
	this->achievements[7] = Achievement("Energized", { "Make the Snake long enough to cover", "25% of the board with the", "Snake speed at 9" },
		{ "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1"}, {408});
	this->achievements[8] = Achievement("Ghost", { "Make the Snake long enough to cover", "30% of the board in the Invisible mode." },
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 215, 308 });
	this->achievements[9] = Achievement("Inversion", { "Make the Snake long enough to cover", "30% of the board in the Inverting mode." },
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 216, 317 });
	this->achievements[10] = Achievement("Builder", { "Make the Snake long enough to cover", "(along with the wall) 35% of the", "board in the Wall mode."},
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, {110, 210, 314});
	this->achievements[11] = Achievement("For Science", { "Make the Snake long enough to cover", "25% of the board in the Portal mode." },
		{ "Snake Speed 3-9", "Board Size: 13-19", "Snake Len.: 1" }, { 313, 504 });
	this->achievements[12] = Achievement("Marty McFly", { "Make the Snake long enough to", "cover 30% of the board", "in the Time Travel mode."},
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 113 });
	this->achievements[13] = Achievement("Timeloop", { "In the Time Travel mode, visit the same", "point in time " + to_string(TIMELOOP_ACHIEVEMENT_VISITS_REQUIRED) + " times (the moment", "you are currently in is shown by", "\"The Present\" display)."},
		{"Snake Speed 4-9"}, { 217 });
	this->achievements[14] = Achievement("Fruit Chaser", { "Make the Snake long enough to", "cover 35% of the board", "in the Moving Fruits mode."},
		{ "Snake Speed 4-9", "Board Size: 13-19", "Fruits: 1-3", "Snake Len.: 1" }, { 406 });
	this->achievements[15] = Achievement("Fruit Catcher", { "Eat in total 20 diagonally moving Fruits", "(saved between sessions - you may", "leave the game and keep progress)." },
		{ "Snake Speed 4-9", "Board Size: 11-19", "Fruits: 1-3" }, { 407 });
	this->achievements[16] = Achievement("Emergency", { "[SECRET] We need help. Call 112 now!" }, {}, { 114 });
	this->achievements[17] = Achievement("Training", { "[SECRET] It's time for some training!", "You don't need any special equipment", "- even running is enough. Just",
		"remember about a strict diet!"}, {}, { 316 });
	this->achievements[18] = Achievement("Sweet Treat", { "[SECRET] Get a free candy." }, {}, { 315 });
	this->achievements[19] = Achievement("Critical Error", { "[SECRET] Beat the game. Somehow..." }, {"Fruits: 1-5"}, {115, 318, 409});
}



const Achievement& DataManager::getAchievementData(int index) {
	return this->achievements[index];
}

int DataManager::getAchievementIndexByLockedItemId(int id) {
	for (int i = 0; i < achievementsNumber; i++) {
		for (int idToCheck : this->achievements[i].lockedItemsIds) {
			if (idToCheck == id) {
				return i;
			}
		}
	}
	return -1;
}

vector<int> DataManager::getLockedItems()
{
	vector<int> toReturn;
	for (int i = 0; i < achievementsNumber; i++) {
		if (!this->achievements[i].unlocked) {
			for (int id : this->achievements[i].lockedItemsIds) {
				toReturn.push_back(id);
			}
		}
	}

	return toReturn;
}



bool DataManager::unlockAchievement(int index)
{
	if (!this->achievements[index].unlocked) {
		this->achievements[index].unlocked = true;
		this->saveData(SAVE_FILE_DIRECTORY);
		return true;
	}
	else {
		return false;
	}

}

string DataManager::checkAchievementUnlocks(GameData data) {

	// int snakeSpeeds[9] = { 15, 12, 10, 8, 7, 6, 5, 4, 3 };
	// boardSizes[6] = { 9, 11, 13, 15, 17, 19 };
	// fruitQuantities[6] = { 1, 2, 3, 4, 5, 10 };
	// snakeLengtheningValues[4] = { 1, 2, 3, 5 };

	if (data.gamemode == Gamemode::MovingFruits && !this->achievements[15].unlocked && data.boardUpdateFrames <= 8 && data.boardSize >= 11 && data.fruitQuantity <= 3) {
		this->totalDiagonalFruitsEaten += data.diagonalFruitsEaten;
		this->saveData(SAVE_FILE_DIRECTORY);
	}


	// ACHIEVEMENT UNLOCKING CONDITIONS

	if (!this->achievements[0].unlocked  // First steps
		&& data.points >= 15) {
		this->unlockAchievement(0);
		return this->achievements[0].name;
	}
	else if (!this->achievements[1].unlocked  // 100
		&& data.points >= 100 && data.boardUpdateFrames <= 8 && data.boardSize <= 17 && data.fruitQuantity <= 3) {
		this->unlockAchievement(1);
		return this->achievements[1].name;
	}
	else if (!this->achievements[2].unlocked  // Fighting Hunger
		&& data.occupiedTiles >= (data.boardSize * data.boardSize * 0.20f) && data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(2);
		return this->achievements[2].name;
	}
	else if (!this->achievements[3].unlocked  // Gaining Weight
		&& data.occupiedTiles >= (data.boardSize * data.boardSize * 0.35f) && data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(3);
		return this->achievements[3].name;
	}
	else if (!this->achievements[4].unlocked  // Expansion
		&& data.occupiedTiles >= (data.boardSize * data.boardSize * 0.50f) && data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(4);
		return this->achievements[4].name;
	}
	else if (!this->achievements[5].unlocked  // Megaexpansion
		&& data.occupiedTiles >= (data.boardSize * data.boardSize * 0.70f) && data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(5);
		return this->achievements[5].name;
	}
	else if (!this->achievements[6].unlocked  // Chillout
		&& data.points >= 35 && data.boardUpdateFrames >= 15 && data.fruitQuantity <= 3) {
		this->unlockAchievement(6);
		return this->achievements[6].name;
	}
	else if (!this->achievements[7].unlocked  // Energized
		&& data.occupiedTiles >= (data.boardSize * data.boardSize * 0.25f) && data.boardUpdateFrames <= 3 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(7);
		return this->achievements[7].name;
	}
	else if (!this->achievements[8].unlocked  // Ghost
		&& data.gamemode == Gamemode::Invisible && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.30f)
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(8);
		return this->achievements[8].name;
	}
	else if (!this->achievements[9].unlocked  // Inversion
		&& data.gamemode == Gamemode::Inverting && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.30f)
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 11 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(9);
		return this->achievements[9].name;
	}
	else if (!this->achievements[10].unlocked  // Builder
		&& data.gamemode == Gamemode::Wall && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.35f)
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(10);
		return this->achievements[10].name;
	}
	else if (!this->achievements[11].unlocked  // For Science
		&& data.gamemode == Gamemode::Portal && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.25f)
		&& data.boardUpdateFrames <= 10 && data.boardSize >= 13 && data.snakeLengthening == 1) {
		this->unlockAchievement(11);
		return this->achievements[11].name;
	}
	else if (!this->achievements[12].unlocked  // Marty McFly
		&& data.gamemode == Gamemode::TimeTravel && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.30f)
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(12);
		return this->achievements[12].name;
	}
	else if (!this->achievements[13].unlocked  // Timeloop
		&& data.gamemode == Gamemode::TimeTravel && data.unlockTimeloopAchievement
		&& data.boardUpdateFrames <= 8) {
		this->unlockAchievement(13);
		return this->achievements[13].name;
	}
	else if (!this->achievements[14].unlocked  // Fruit Chaser
		&& data.gamemode == Gamemode::MovingFruits && data.occupiedTiles >= (data.boardSize * data.boardSize * 0.35f)
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 13 && data.fruitQuantity <= 3 && data.snakeLengthening == 1) {
		this->unlockAchievement(14);
		return this->achievements[14].name;
	}
	else if (!this->achievements[15].unlocked  // Fruit Catcher
		&& data.gamemode == Gamemode::MovingFruits && this->totalDiagonalFruitsEaten >= 20
		&& data.boardUpdateFrames <= 8 && data.boardSize >= 11 && data.fruitQuantity <= 3) {
		this->unlockAchievement(15);
		return this->achievements[15].name;
	}
	else if (!this->achievements[16].unlocked  // Emergency (die while having 112 points)
		&& data.gameOver && data.points == 112) {
		this->unlockAchievement(16);
		return this->achievements[16].name;
	}
	else if (!this->achievements[17].unlocked  // Training (don't eat for a minute)
		&& data.lastFruitCollectionTime <= data.timer - 60.f) {
		this->unlockAchievement(17);
		return this->achievements[17].name;
	}
	// Free candy achievement is unlocked using the unlockAchievement() function called from the Menu class
	else if (!this->achievements[19].unlocked  // Error (beat the game by having every single tile on the board occupied by either Snake or wall (if beating the game in the Wall mode is even possible))
		&& data.unlockErrorAchievement && data.fruitQuantity <= 5) {
		this->unlockAchievement(19);
		return this->achievements[19].name;
	}
	else {
		return "";
	}
}



void DataManager::saveSelectedSettings(int gamemodeIndex, int snakeSpeedIndex, int boardSizeIndex, int fruitQuantityIndex, int snakeLengtheningIndex,
	int snakeColorId, int gradientColorId, int tileColorId, int fruitTextureId, int musicTrackId)
{
	this->savedGamemodeIndex = gamemodeIndex;
	this->savedSnakeSpeedIndex = snakeSpeedIndex;
	this->savedBoardSizeIndex = boardSizeIndex;
	this->savedFruitQuantityIndex = fruitQuantityIndex;
	this->savedSnakeColorId = snakeColorId;
	this->savedGradientColorId = gradientColorId;
	this->savedTileColorId = tileColorId;
	this->savedFruitTextureId = fruitTextureId;
	this->savedMusicTrackId = musicTrackId;
}



void DataManager::saveData(string directory)
{
	std::ofstream outFile(directory, std::ios::binary);

	if (outFile) {
		for (int i = 0; i < achievementsNumber; ++i) {
			outFile.write(reinterpret_cast<const char*>(&achievements[i].unlocked), sizeof(bool));
		}
		outFile.write(reinterpret_cast<const char*>(&this->totalDiagonalFruitsEaten), sizeof(int));

		outFile.write(reinterpret_cast<const char*>(&this->savedGamemodeIndex), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedSnakeSpeedIndex), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedBoardSizeIndex), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedFruitQuantityIndex), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedSnakeLengtheningIndex), sizeof(int));

		outFile.write(reinterpret_cast<const char*>(&this->savedSnakeColorId), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedGradientColorId), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedTileColorId), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedFruitTextureId), sizeof(int));
		outFile.write(reinterpret_cast<const char*>(&this->savedMusicTrackId), sizeof(int));
	}
	else {
		std::string errorMessage = "Failed to save data: Error opening save file (directory: " + SAVE_FILE_DIRECTORY
			+ "). Check if the file is already in use by external programs.";
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR | MB_OK);
	}
}

void DataManager::loadData(string directory) {
	std::ifstream inFile(directory, std::ios::binary | std::ios::ate); // Open the file at the end
	bool loadingSuccessful = true;


	if (inFile) {
		std::streampos fileSize = inFile.tellg(); // Check file size
		inFile.seekg(0, std::ios::beg); // Get back to the begin

		// If all achievements are saved, but settings aren't, load achievements and rewrite file with default settings
		bool loadSettingsData = true;
		if (fileSize < this->expectedSaveFileSize) {
			loadSettingsData = false;
			// If not even achievements are saved, close the reading mode and write the file with default values
			if (fileSize < achievementsNumber * sizeof(bool) + sizeof(int)) {
				inFile.close();
				this->restoreDefualtData();
				this->saveData(directory);
				return;
			}
		}

		for (int i = 0; i < achievementsNumber; ++i) {
			loadingSuccessful = loadingSuccessful && inFile.read(reinterpret_cast<char*>(&this->achievements[i].unlocked), sizeof(bool));
		}

		loadingSuccessful = loadingSuccessful && inFile.read(reinterpret_cast<char*>(&this->totalDiagonalFruitsEaten), sizeof(int));

		if (loadSettingsData && loadingSuccessful) {
			loadingSuccessful = loadingSuccessful && inFile.read(reinterpret_cast<char*>(&this->savedGamemodeIndex), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedSnakeSpeedIndex), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedBoardSizeIndex), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedFruitQuantityIndex), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedSnakeLengtheningIndex), sizeof(int))

				&& inFile.read(reinterpret_cast<char*>(&this->savedSnakeColorId), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedGradientColorId), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedTileColorId), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedFruitTextureId), sizeof(int))
				&& inFile.read(reinterpret_cast<char*>(&this->savedMusicTrackId), sizeof(int));
		}
		else {
			this->saveData(directory);
		}

		if (!loadingSuccessful) {
			std::string errorMessage = "Error loading data. The most likely reason is file corrupt. Press OK to recreate the file with default data.";
			MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR | MB_OK);
			this->restoreDefualtData(directory);
			this->saveData(directory);
		}
	}
	else {
		this->restoreDefualtData(directory);
		this->saveData(directory);
	}
}

void DataManager::restoreDefualtData(string directory)
{
	for (int i = 0; i < this->achievementsNumber; i++) {
		this->achievements[i].unlocked = false;
	}

	this->savedGamemodeIndex = 0;
	this->savedSnakeSpeedIndex = 5;
	this->savedBoardSizeIndex = 2;
	this->savedFruitQuantityIndex = 0;
	this->savedSnakeLengtheningIndex = 0;

	this->totalDiagonalFruitsEaten = 0;

	this->savedSnakeColorId = 102;
	this->savedGradientColorId = 213;
	this->savedTileColorId = 307;
	this->savedFruitTextureId = 401;
	this->savedMusicTrackId = 501;
}

bool DataManager::verifyLoadedData(int gamemodesNumber, int snakeSpeedOptionsNumber, int boardSizeOptionsNumber, int fruitQuantityOptionsNumber, int snakeLengtheningOptionsNumber)
{
	bool dataCorrect = this->savedSnakeSpeedIndex >= 0 && this->savedSnakeSpeedIndex < snakeSpeedOptionsNumber
		&& this->savedBoardSizeIndex >= 0 && this->savedBoardSizeIndex < boardSizeOptionsNumber
		&& this->savedFruitQuantityIndex >= 0 && this->savedFruitQuantityIndex < fruitQuantityOptionsNumber
		&& this->savedSnakeLengtheningIndex >= 0 && this->savedSnakeLengtheningIndex < snakeLengtheningOptionsNumber
		&& this->savedGamemodeIndex >= 0 && this->savedGamemodeIndex < gamemodesNumber
		&& this->savedSnakeColorId >= 100 && this->savedSnakeColorId < 200
		&& this->savedGradientColorId >= 200 && this->savedGradientColorId < 300
		&& this->savedTileColorId >= 300 && this->savedSnakeColorId < 400
		&& this->savedFruitTextureId >= 400 && this->savedFruitTextureId < 500
		&& this->savedMusicTrackId >= 500 && this->savedMusicTrackId < 600;

	return dataCorrect;
}

void DataManager::eraseData(string directory) {
	std::ofstream outFile(directory, std::ios::binary);
	outFile.close();
}
