#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "SETTINGS.h"
#include "Button.h"

using namespace std;

class BottomBar
{
private:
	bool loadSuccessful = true;
	sf::Font* fontPtr;
	sf::RenderTexture renderTexture;

	sf::Text presentText;
	sf::Text pointText;
	sf::Text timerText;

	float pointTextDefaultY;
	float presentTextDefaultY;
	sf::Color presentTextDefaultColor;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Texture menuButtonTexture;

	sf::Sound* buttonClickSoundPtr;
	sf::Sound* buttonHoverSoundPtr;
	sf::Sound* achievementSoundPtr;
	
	Button* menuButtonPtr;

	sf::Texture timerTexture;
	sf::Sprite timerSprite;

	sf::Sprite unlockedAchievementSprite;
	sf::Sprite lockedAchievementSprite;

	int achievementDisplayCurrentFrames = -1;
	int achievementDisplayPhaseFrames = 10;
	float achievementNotificationHeight;
	sf::Text achievementUnlockedText;
	sf::Text achievementNameText;

	int pointsDisplayed = 0;

	bool showTimeTravelModeText = false;
	
	void initText();
	void initSprite(sf::Sprite* spritePtr, sf::Texture* texturePtr, float posX = 0.f, float posY = 0.f, float width = 0.f, float height = 0.f);
	void handleAchievementNotification();
public:
	BottomBar(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr, sf::Texture* unlockedAchievementTexturePtr, sf::Texture* lockedAchievementTexturePtr,
		sf::Sound* achievementSound = nullptr);
	~BottomBar();

	sf::Sprite sprite;

	bool getLoadSuccessful();
	void setButtonSounds(sf::Sound* hoverSoundPtr = nullptr, sf::Sound* clickSoundPtr = nullptr);
	void setColor(sf::Color color);
	void setTimeTravelModeTextVisibility(bool visible = true);

	void drawRenderTexture();
	void draw(sf::RenderWindow* renderWindowPtr);
	void draw(sf::RenderTexture* renderTexturePtr);
	bool getMenuButtonClicked(sf::RenderWindow* window);
	void update(sf::RenderWindow* window, int points = 0);
	void updateTimer(float timeInSeconds);
	void updateTimeTravelText(int boardUpdateFrames, int moveCounter);
	void displayAchievementNotification(string achievementName, float notificationVisibilityTime = ACHIEVEMENT_NOTIFCATION_DISPLAY_TIME);
	void resetAchievementNotification();
};

