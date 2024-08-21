#pragma once

//#include <cstdint>
#include <iostream>
#include <cmath>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Board.h"
#include "Menu.h"
#include "SETTINGS.h"
#include "BottomBar.h"
#include "GameData.h"


enum class GameState{Loading = -1, Menu = 0, Paused, Active, Over};

class Game
{
public:
	Game();
	~Game();

	void processGame();
private:
	// Window and events
	sf::RenderWindow* window;
	sf::Image windowIcon;
	sf::VideoMode videoMode;
	sf::Event ev;


	// Variables
	GameState gameState = GameState::Loading;
	GameSettings gameSettingsList;
	bool loadSuccessful = true;
	bool paused = false;
	bool endingSequenceActive = false;
	int gameCompletions = 0;
	int endingSequenceFramesCount = 0;
	int maxEndingSequenceFrames = round(FPS * ENDING_SEQUENCE_DURATION);
	int bsodWaitingFrames = 0;
	int maxBsodWaitingFrames = round(FPS * ENDING_RESTART_PERIOD_DURATION);
	int gameActiveFramesCount = 0;
	int pauseFramesCount = 0;
	int menuFramesCount = 0;
	int musicPreviewFramesCount = 0;
	int minPauseFrames = 1;
	int musicFadeOutFrames = 1;
	int musicFadeInFrames = 1;
	int points = 0;
	float timer = 0.f;

	// Objects and assets
	BottomBar* bottomBar;
	Board* board;
	Menu* menuObject = nullptr;
	sf::Text* startText;
	sf::Text* gameOverText;
	sf::Font defaultFont;

	sf::Texture bsodTexture;
	sf::Sprite bsodSprite;

	sf::Texture controlsInfoTexture;
	sf::Sprite controlsInfoSprite;

	sf::Music menuMusic;
	sf::Music* gameMusicPtr;
	const int gameMusicTracksNumber = 5;
	std::string musicDirectories[5] = {
		"Audio/Music1.mp3",
		"Audio/Music2.mp3",
		"Audio/Music3.mp3",
		"Audio/Music4.mp3",
		"Audio/Music5.mp3"
	};
	sf::SoundBuffer buttonHoverSoundBuffer;
	sf::Sound buttonHoverSound;
	sf::SoundBuffer buttonClickSoundBuffer;
	sf::Sound buttonClickSound;
	sf::SoundBuffer buttonDisabledClickSoundBuffer;
	sf::Sound buttonDisabledClickSound;
	sf::SoundBuffer achievementSoundBuffer;
	sf::Sound achievementSound;
	sf::SoundBuffer secretAchievementSoundBuffer;
	sf::Sound secretAchievementSound;
	sf::Texture unlockedAchievementLabelTexture;
	sf::Texture lockedAchievementLabelTexture;
	

	// Init functions
	void initVariables();
	void initAssets();
	void initWindow();
	void initMenu();
	void initBoard();
	void initBottomBar();
	void initText();
	bool getAssetsLoaded();
	void reloadGameMusic(int index = 0);

	// Functions ran every frame
	void handleEvents();
	void handleKeyPressed(sf::Event::KeyEvent);
	void update();
	void drawWindow();
	void handleMusic();
	void handleFrameCounters();

	// Other functions
	void enterMenu();
	void getSettingsList();
	void exitMenu();
	void gameOver();
	void resetGame();
	void startGame();
	void stopGame();
};
