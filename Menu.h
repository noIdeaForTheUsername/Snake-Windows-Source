#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Board.h"
#include "Fruit.h"
	
#include "SETTINGS.h"
#include "Button.h"
#include "DataManager.h"
#include "AchievementInfoPage.h"
#include "GameData.h"

using namespace std;

enum class MenuPage {Main = 1, Gamemodes, Settings, Customize1, Customize2, Achievements, AchievementInfo, GameInfo, Credits, TTModeInfo1, TTModeInfo2, DataErase};

struct GameSettings {
	Gamemode gamemode = Gamemode::Normal;
	int boardUpdateFrames = 6;
	int boardColumns = 13;
	int boardRows = 13;
	int fruitQuantity = 1;
	int snakeLengthening = FRUIT_COLLECTION_LENGTHENING;
	sf::Color snakeColorBegin = sf::Color(220, 45, 0, 255);
	sf::Color snakeColorEnd = sf::Color(255, 200, 190, 255);
	bool randomizeSnakeBeginColor = false;
	bool randomizeSnakeEndingColor = false;
	int minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION;
	bool emergencySnake = false;
	bool rainbowSnake = false;
	bool oppositeGradientColor = false;
	bool checkTimeloopAchievement = false;
	sf::Color tileColor1 = sf::Color(0, 90, 255, 255);
	sf::Color tileColor2 = sf::Color(0, 60, 180, 255);
	int fruitTextureIndex = 0;
	int musicIndex = 0;
};

struct SnakeColor {
	int id = 100;
	sf::Color color;
	sf::Color defualtGradient;
	string name;
	Button* buttonPtr;
};

struct GradientColor {
	int id = 200;
	sf::Color color;
	string name;
	Button* buttonPtr;
};

struct TileColor {
	int id = 300;
	sf::Color color1;
	sf::Color color2;
	string name;
	Button* buttonPtr;
};

struct FruitTexture {
	int id = 400;
	string name = "fruit_name";
	int externalArrayIndex = 0;
	Button* buttonPtr;
};

struct MusicTrack {
	int id = 500;
	string name;
	float previewPlayingBeginOffset = 0.f;
	float previewPlayingEndOffset = 10.f;
	int externalArrayIndex = 0;
	Button* buttonPtr;
};

struct TTModeInfoPicture {
	sf::Texture texture;
	sf::Sprite sprite;
};

class Menu
{
private:
	bool loadSuccessful;

	bool playButtonActivated = false;
	DataManager dataManager;
	MenuPage currentPage;
	GameSettings gameSettingsList;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Font* fontPtr;

	int menuBoardRows;
	int menuBoardColumns;
	float tileWidth;
	float tileHeight;

	// FUNCTIONS
	void initVariables(int gameMusicTracksNumber = 1);
	void initAssetsAndSprites(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr,
		sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr,
		sf::Sound* buttonHoverSoundPtr, sf::Sound* buttonClickSoundPtr, sf::Sound* buttonDisabledClickSoundPtr = nullptr);
	void initMenuPages();
	void initMainPage();
	void initGamemodesPage();
	void initSettingsPage();
	void initCustomizePage();
	void initAchievementsPage();
	void initGameInfoPage();
	void initCreditsPage();
	void initTtModeInfoPages();
	void initDataErasePage();
	void initPageHeaderText(sf::Text* textObjectPtr, std::string string);
	sf::Text* createTtModeInfoBackgroundText(std::string string, sf::Vector2f pos, sf::Color color);
	void checkSettingsButtonArrayClicks(Button** buttonArrayPtr, int buttonArrayLength, sf::RenderWindow* window);
	void deactivateButtonArray(Button** buttonArrayPtr, int buttonArrayLength);
	int getActiveButtonIndex(Button** buttonArrayPtr, int buttonArrayLength);
	void resetActivatedButtons();

	template <typename T, std::size_t N>
	T* checkCustomizationButtonGroupClicks(T(&itemArray)[N], sf::RenderWindow* window, sf::Mouse::Button mouseButton);
	template <typename T, std::size_t N>
	void deactivateButtonGroup(T(&itemArray)[N]);
	template <typename T, std::size_t N>
	T& getSelectedItem(T(&itemArray)[N]);
	template <typename T, std::size_t N>
	T& getItemById(int id, T(&itemArray)[N]);

	void drawRenderTexture();


	// MULTIPLE PAGES
	sf::Color optionButtonsTextColor = sf::Color(0, 0, 255);
	sf::Color optionNameTextColor = sf::Color(255, 255, 255);
	sf::Color headerTextColor = sf::Color(255, 255, 0);
	float headerTextSize = 70.f;

	Button* backButtonPtr;
	sf::Texture backButtonTexture;

	sf::Texture activeButtonIndicator;
	sf::Texture activeButtonIndicator2;

	sf::Sound* buttonHoverSoundPtr;
	sf::Sound* buttonClickSoundPtr;
	sf::Sound* buttonDisabledClickSoundPtr;

	// MAIN PAGE
	sf::Text titleText;

	sf::Texture playButtonTexture;
	Button* playButtonPtr;
	sf::Texture settingsButtonTexture;
	Button* settingsButtonPtr;
	sf::Texture gamemodesButtonTexture;
	Button* gamemodesButtonPtr;
	sf::Texture customizeButtonTexture;
	Button* customizeButtonPtr;
	sf::Texture achievementsButtonTexture;
	Button* achievementsButtonPtr;
	sf::Texture infoButtonTexture;
	Button* infoButtonPtr;



	sf::SoundBuffer brakeSoundBuffer;
	sf::Sound brakeSound;
	sf::SoundBuffer candySoundBuffer;
	sf::Sound candySound;
	sf::Texture vanTexture;
	sf::Sprite vanSprite;
	sf::Texture candyTexture;
	sf::Sprite candySprite;
	sf::Vector2f defaultCandySpriteScale;
	Button* titleButtonPtr;
	Button* vanButtonPtr;
	int titleButtonClicks = 0;
	int vanAnimationFrames = -1;
	int vanAnimationPhaseFrames = 0;
	void handleVanAnimation();


	// GAMEMODES PAGE
	sf::Text gamemodesHeaderText;

	sf::Texture gamemodeButtonTexture;

	std::string gamemodeNames[7] = { "Normal", "Invisible", "Inverting", "Wall", "Moving Fruits", "Portal", "Time Travel" };
	std::string gamemodeDescriptions[7][2] = {
		{"The standard gamemode.", ""},
		{"The Snake becomes invisible.", "You can only see its head."},
		{"The Snake's head swaps with its tail", "every time the Snake eats."},
		{"Eating makes the Snake place", "extra wall tiles on the ground."},
		{"Fruits move every time", " the Snake moves."},
		{"Fruits work like portals that", " you can pass through."},
		{"Fruits teleport you in time when eaten", "(more instructions in the Info menu)."}
	};

	sf::Text gamemodePageInfoText;
	sf::Text gamemodeDescriptionText;
	sf::Text gamemodeDescriptionText2;
	float gamemodeDescriptionTextSize = 27.f;
	sf::Color gamemodeDescriptionTextColor = sf::Color(255, 255, 255);
	Button** gamemodeButtons;
	const int gamemodeButtonsNumber = 7;

	void updateGamemodeDescriptionText(int selectedGamemodeIndex = 0);


	// SETTINGS PAGE
	sf::Text settingsHeaderText;
	sf::Text snakeSpeedText;
	sf::Text boardSizeText;
	sf::Text fruitAmountText;
	sf::Text snakeLengthingText;

	sf::Texture snakeSpeedButtonTexture;
	sf::Texture boardSizeButtonTexture;
	sf::Texture fruitQuantityButtonTexture;
	sf::Texture snakeLengtheningButtonTexture;

	Button** snakeSpeedButtons;
	const int snakeSpeedButtonsNumber = 9;
	int snakeSpeeds[9] = { 15, 12, 10, 8, 7, 6, 5, 4, 3 };
	Button** boardSizeButtons;
	const int boardSizeButtonsNumber = 6;
	int boardSizes[6] = { 9, 11, 13, 15, 17, 19 };
	Button** fruitQuantityButtons;
	const int fruitQuantityButtonsNumber = 6;
	int fruitQuantities[6] = { 1, 2, 3, 4, 5, 10 };
	Button** snakeLengtheningButtons;
	const int snakeLengtheningButtonsNumber = 4;
	int snakeLengtheningValues[4] = { 1, 2, 3, 5 };

	// CUSTOMIZATION PAGE(S)
	sf::Text customizeHeaderText;
	sf::Text customizeRightClickText;
	int customizeRightClickTextPulse = 0;
	int customizeRightClickTextPulseMaxValue = FPS * 1.0f;
	sf::Color customizeRightClickTextDefaultColor = sf::Color::White;
	sf::Color customizeRightClickTextPulseColor = sf::Color(255, 0, 0);
	sf::Text snakeColorText;
	sf::Text gradientColorText;
	sf::Text tileColorText;
	sf::Text fruitTextureText;
	sf::Text musicText;
	sf::Color customizationTabColor1;
	sf::Color customizationTabColor2;
	Button* customizationTab1ButtonPtr;
	Button* customizationTab2ButtonPtr;

	sf::Sprite customizationTabSprite;
	sf::Texture customizationTabTexture;
	sf::Texture tabButtonTexture;
	sf::Texture colorButtonTexture;
	sf::Texture rainbowColorButtonTexture;
	sf::Texture randomColorButtonTexture;
	sf::Texture emergencyColorButtonTexture;
	sf::Texture musicButtonTexture;
	sf::Texture colorOverlayTexture;
	sf::Texture alphaOverlayTexture;
	sf::Texture gradientOffOverlayTexture;
	sf::Texture disabledIconTexture;

	const int snakeColorsNumber = 15;
	SnakeColor snakeColors[15] = {
		{101, sf::Color(150, 0, 0), sf::Color(255, 190, 190), "Red", nullptr},
		{102, sf::Color(220, 45, 0), sf::Color(255, 200, 190), "Orange", nullptr},
		{103, sf::Color(210, 190, 0), sf::Color(255, 240, 190), "Yellow", nullptr},
		{104, sf::Color(0, 230, 0), sf::Color(232, 255, 239), "Lime", nullptr},
		{105, sf::Color(0, 85, 20), sf::Color(110, 155, 120), "Dark green", nullptr},
		{106, sf::Color(0, 225, 225), sf::Color(210, 255, 255), "Cyan", nullptr},
		{107, sf::Color(7, 20, 150), sf::Color(90, 95, 205), "Dark blue", nullptr},
		{108, sf::Color(110, 0, 170), sf::Color(230, 190, 255), "Purple", nullptr},
		{109, sf::Color(195, 0, 90), sf::Color(255, 155, 200), "Fuchsia", nullptr},
		{110, sf::Color(120, 120, 120), sf::Color(200, 200, 200), "Gray", nullptr},
		{111, sf::Color(255, 255, 255), sf::Color(170, 170, 170), "White", nullptr},
		{112, sf::Color(6, 6, 6), sf::Color(90, 90, 90), "Black", nullptr},

		{113, sf::Color(150, 0, 0), sf::Color(200, 200, 200), "Randomized Color", nullptr},
		{114, sf::Color(150, 0, 0), sf::Color(200, 200, 200), "Emergency siren", nullptr},
		{115, sf::Color(150, 0, 0), sf::Color(200, 200, 200), "Rainbow Snake", nullptr},
	};

	const int gradientColorsNumber = 17;
	GradientColor gradientColors[17] = {
		{201, sf::Color(150, 0, 0), "Red Gradient", nullptr},
		{202, sf::Color(220, 45, 0), "Orange Gradient", nullptr},
		{203, sf::Color(210, 190, 0), "Yellow Gradient", nullptr},
		{204, sf::Color(0, 230, 0), "Lime Gradient", nullptr},
		{205, sf::Color(0, 85, 20), "Dark green Gradient", nullptr},
		{206, sf::Color(0, 225, 225), "Cyan Gradient", nullptr},
		{207, sf::Color(7, 20, 150), "Dark blue Gradient", nullptr},
		{208, sf::Color(110, 0, 170), "Purple Gradient", nullptr},
		{209, sf::Color(195, 0, 90), "Fuchsia Gradient", nullptr},
		{210, sf::Color(120, 120, 120), "Gray Gradient", nullptr},
		{211, sf::Color(255, 255, 255), "White Gradient", nullptr},
		{212, sf::Color(6, 6, 6), "Black Gradient", nullptr},

		{213, sf::Color(150, 0, 0), "Default Gradient", nullptr},
		{214, sf::Color(150, 0, 0), "No Gradient", nullptr},
		{215, sf::Color(150, 0, 0), "Transparent Gradient", nullptr},
		{216, sf::Color(150, 0, 0), "Opposite color Gradient", nullptr},
		{217, sf::Color(150, 0, 0), "Randomized Gradient", nullptr}
	};

	const int tileColorButtonsNumber = 18;
	TileColor tileColors[18] = {
		{301, sf::Color(165, 0, 0), sf::Color(115, 0, 0), "Red"},  // Red
		{302, sf::Color(255, 115, 25), sf::Color(204, 102, 30), "Orange"},  // Orange
		{303, sf::Color(255, 226, 165), sf::Color(237, 189, 119), "Beach"},  // Beach
		{304, sf::Color(0, 204, 50), sf::Color(0, 143, 35), "Green"},  // Green
		{305, sf::Color(0, 102, 25), sf::Color(0, 71, 18), "Dark green"},  // Dark green
		{306, sf::Color(0, 180, 175), sf::Color(0, 125, 123), "Cyan"},  // Cyan
		{307, sf::Color(0, 90, 255), sf::Color(0, 60, 180), "Blue"},  // Blue (default)
		{308, sf::Color(10, 10, 10), sf::Color(40, 0, 65), "Shadow"},  // Shadow
		{309, sf::Color(128, 0, 94), sf::Color(90, 0, 66), "Dark Fuchsia"},  // Fuchsia
		
		{310, sf::Color(100, 100, 100), sf::Color(70, 70, 70), "Gray"},  // Gray
		{311, sf::Color(240, 240, 240), sf::Color(195, 195, 195), "White"},  // White
		{312, sf::Color(26, 26, 26), sf::Color(13, 13, 13), "Black"},  // Black

		{313, sf::Color(0, 220, 220), sf::Color(200, 200, 200), "Sci-fi"}, // Portal (sci-fi, gray-cyan)
		{314, sf::Color(13, 13, 13), sf::Color(220, 230, 0), "Hazard"}, // Construction (yellow-black)
		{315, sf::Color(178, 255, 255), sf::Color(255, 128, 255), "Candy"},  // Candy (cyan-pink)
		{316, sf::Color(124, 101, 70), sf::Color(65, 87, 43), "Military"},  // Military (brown-green)
		{317, sf::Color(240, 240, 240), sf::Color(13, 13, 13), "Black and White"}, // Black-white
		{318, sf::Color(0, 0, 0), sf::Color(0, 0, 0), "Void"}  // Void (black)

		//{sf::Color(110, 65, 30), sf::Color(90, 50, 15)},  // Dirt
		//{sf::Color(204, 0, 204), sf::Color(143, 0, 143)},  // Magenta
	};

	int fruitTextureButtonsNumber = 9;
	FruitTexture fruitTextures[9] = {
		{401, "Apple"},
		{402, "Banana"},
		{403, "Orange"},
		{404, "Watermelon"},
		{405, "Cherries"},
		{406, "Blueberries"},
		{407, "Pineapple"},
		{408, "Chocolate"},
		{409}
	};

	int musicButtonsNumber;
	MusicTrack musicTracks[5] = {
		{501, "Music track 1", 18.4f, 47.2f},
		{502, "Music track 2", 16.0f, 39.6f},
		{503, "Music track 3", 19.9f, 42.1f},
		{504, "Music track 4", 13.1f, 40.7f},
		{505, "Music track 5", 23.1f, 56.0f}
	};

	void updateGradientColorButtonsColors();
	void handleCustomizeRightClickTextPulseEffect();


	// ACHIEVEMENTS PAGE + ACHIEVEMENT INFO PAGES

	sf::Text achievementsHeaderText;
	std::vector<sf::Text*> achievementNameLabels;

	Button** achievementButtons;

	sf::Texture unlockedAchievementButtonTexture;
	sf::Texture lockedAchievementButtonTexture;

	sf::Texture* unlockedAchievementLabelTexturePtr;
	sf::Texture* lockedAchievementLabelTexturePtr;

	AchievementInfoPage* achievementInfoPages[DataManager::achievementsNumber];

	int currentMusicPreviewIndex = -1;

	int currentAchievementInfoPageIndex = 0;
	bool showItemInfoOnAchievementInfoPage = false;
	bool returnToCustomizationTab2 = false;


	// GAME INFO PAGES
	// Main
	sf::Text gameInfoHeaderText;

	sf::Texture gameInfoPageOptionTexture;

	Button* creditsButtonPtr;
	Button* ttModeInfoButtonPtr;
	Button* eraseDataButtonPtr;

	// Credits
	sf::Text creditsHeaderText;

	sf::Text* creditsTexts;
	const int creditsTextsNumber = 9;
	std::string creditsTextLines[9] = {
		"Game creator (GitHub username):",
		"noIdeaForTheUsername",
		"Made with SFML",
		"and VS Community 2019",
		"Font: dafont.com/04b-30.font",
		"Sound effects: zapsplat.com",
		"Game music: created with suno.com",
		"Menu music: fesliyanstudios.com",
		"- 8 Bit Adventure (slower ver.)"
	};

	// Time travel mode info pages (2)
	sf::Text ttModeInfoHeaderText;

	sf::Texture ttModeInfoPageButtonTexture;
	Button* ttModeInfoPage1ButtonPtr;
	Button* ttModeInfoPage2ButtonPtr;

	const int ttModeInfoPicturesNumber = 4;
	TTModeInfoPicture ttModeInfoPictures[4];

	std::vector<sf::Text*> ttModeInfoBackgroundTexts1;
	sf::Text* ttModeInfoTexts1;
	std::vector<std::string> ttModeInfoTextLines1 = {
		"1. Traveling back in time will teleport",
		"your Snake to a position it had earlier,",
		"in the Past. For example, let's say we",
		"travel 15s back in time.",
		"",
		"",
		"",
		"",
		"",
		"2. After traveling to the Past, you may",
		"get back to the position your Snake had",
		"before traveling - by traveling to the",
		"Future. If after the previous travel",
		"(from example in 1.) we travel again after",
		"2s, and we travel 5s forward in time, the",
		"Snake will end up in a position it had",
		"exactly 8s before the first time travel."
	};

	std::vector<sf::Text*> ttModeInfoBackgroundTexts2;
	sf::Text* ttModeInfoTexts2;
	std::vector<std::string> ttModeInfoTextLines2 = {
		"3. Traveling to the Past will erase the",
		"currently saved Future. You won't be",
		"able to travel there anymore, but all",
		"timestamps between the Present and your",
		"destination point in the Past will still",
		"be available (0:14-0:21). You also can't",
		"travel to timestamps you haven't visited.",
		"",
		"",
		"",
		"",
		"",
		"4. Fruits show how far in the Past or",
		"the Future they're going to teleport you.",
		"However, you mostly have to care about",
		"the position you will end up with -",
		"marked with square indicators. It is",
		"not recommended to play with more than",
		"3 Fruits in this gamemode - and max is 5."
	};


	// Data erase
	sf::Text dataEraseHeaderText;
	sf::Text* dataEraseInfoTexts;
	const int dataEraseInfoTextsNumber = 4;
	std::string dataEraseInfoTextLines[4] = {
		"This includes: achievements, unlocked",
		"customization items and saved game",
		"settings. This action also cannot be",
		"undone. Are you sure?",
	};

	Button* dataEraseCancelButton;
	Button* dataEraseConfirmButton;

	sf::Texture dataEraseIconTexture;
	sf::Sprite dataEraseIconSprite;

public:
	Menu(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr,
		sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr, int gameMusicTracksNumber = 1,
		sf::Sound* buttonHoverSoundPtr = nullptr, sf::Sound* buttonClickSoundPtr = nullptr, sf::Sound* buttonDisabledClickSoundPtr = nullptr);
	~Menu();

	sf::Sprite sprite;
	sf::RenderTexture renderTexture;

	bool getLoadSuccessful();
	void update(sf::RenderWindow* window, sf::Vector2f relativeTopLeftPoint = sf::Vector2f(0, 0));

	string checkAchievementUnlocks(GameData data);
	void reloadAchievementStatus();
	void checkButtonClicks(sf::RenderWindow* window, sf::Mouse::Button mouseButton);
	void setCurrentPage(MenuPage page);
	void beginVanAnimation(float time);

	void loadData(string directory = SAVE_FILE_DIRECTORY);
	void saveData(string directory = SAVE_FILE_DIRECTORY);
	void saveSelectedSettings();

	bool getPlayGame();
	int getCurrentMusicPreviewIndex();
	float getCurrentMusicPreviewBeginOffset();
	float getCurrentMusicPreviewEndOffset();
	GameSettings getGameSettings();
	
	template <typename RenderObject>
	void draw(RenderObject& renderObject);
};

// TEMPLATE FUNCTIONS

template <typename T, std::size_t N>
T* Menu::checkCustomizationButtonGroupClicks(T(&itemArray)[N], sf::RenderWindow* window, sf::Mouse::Button mouseButton) {

	if (mouseButton == sf::Mouse::Button::Left) {
		for (int i = 0; i < N; i++) {
			if (itemArray[i].buttonPtr->checkClicked(window)) {
				this->deactivateButtonGroup(itemArray);
				itemArray[i].buttonPtr->setIsActive(true);
				return &itemArray[i];
			}
			else if (itemArray[i].buttonPtr->checkClicked(window, true, false)) {
				this->customizeRightClickTextPulse = this->customizeRightClickTextPulseMaxValue;
				return &itemArray[i];
			}
		}
		return nullptr;
	}
	else if (mouseButton == sf::Mouse::Button::Right) {
		for (int i = 0; i < N; i++) {
			if (itemArray[i].buttonPtr->checkClicked(window, true, true)) {
				this->currentAchievementInfoPageIndex = this->dataManager.getAchievementIndexByLockedItemId(itemArray[i].id);
				if (this->currentAchievementInfoPageIndex == -1) {
					this->achievementInfoPages[0]->setDisplayedUnlockableItem(itemArray[i].name, itemArray[i].buttonPtr->getStandardTexture());
				}
				else {
					this->achievementInfoPages[currentAchievementInfoPageIndex]->setDisplayedUnlockableItem(itemArray[i].name, itemArray[i].buttonPtr->getStandardTexture());
				}
				this->showItemInfoOnAchievementInfoPage = true;
				this->returnToCustomizationTab2 = this->currentPage == MenuPage::Customize2;
				this->setCurrentPage(MenuPage::AchievementInfo);
				return &itemArray[i];
			}
		}
		return nullptr;
	}

	return nullptr;
}

template <typename T, std::size_t N>
void Menu::deactivateButtonGroup(T(&itemArray)[N])
{
	for (int i = 0; i < N; i++) {
		itemArray[i].buttonPtr->setIsActive(false);
	}
}

template<typename T, std::size_t N>
T& Menu::getSelectedItem(T(&itemArray)[N])
{
	for (auto& item : itemArray) {
		if (item.buttonPtr->isActive) return item;
	}
	std::cerr << "Warning! getSelectedItem(): no selected item found in the given array! Returning zeroth index element." << std::endl;
	return itemArray[0];
}

template<typename T, std::size_t N>
T& Menu::getItemById(int id, T(&itemArray)[N])
{
	for (auto& item : itemArray) {
		if (item.id == id) {
			return item;
		}
	}
	std::string errorMessage = "Menu::getItemById(): Couldn't find an item with the given ID (" + std::to_string(id) + "). Given ID out of range. "
		"This error should only appear if the source code had been modified and a mistake was made.";
	throw std::out_of_range(errorMessage);
}

template <typename RenderObject>
void Menu::draw(RenderObject& renderObject) {
	this->drawRenderTexture();
	this->sprite.setTexture(this->renderTexture.getTexture());
	renderObject.draw(this->sprite);
}
