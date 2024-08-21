#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "SETTINGS.h"

enum class OriginPoint { Custom = 0, Topleft = 1, Center };

class Button
{
private:
	sf::Texture* buttonTexturePtr;
	sf::Sprite buttonSprite;
	sf::Color defaultColor;

	sf::Sprite* overlaySprite = nullptr;
	sf::Sprite* hoverIconSprite = nullptr;
	sf::Sprite* activeIconSprite = nullptr;
	sf::Sprite* disabledIconSprite = nullptr;
	
	sf::Color overlayColor;
	sf::Color hoverColor;
	sf::Color activeColor;
	sf::Color disabledColor;

	sf::Sound* hoverSoundPtr = nullptr;
	sf::Sound* clickSoundPtr = nullptr;
	sf::Sound* disabledClickSoundPtr = nullptr;
	sf::Font* fontPtr = nullptr;
	sf::Color textColor;

	bool playSoundAtHoverEnter = false;
	bool playSoundAtHoverExit = false;
	bool moveAtHover = false;
	
	bool atHover = false;
	bool isDisabled = false;

	float width;
	float height;
	OriginPoint originPoint;
	bool hasText = false;
	OriginPoint textOriginPoint;

	int hoverMovementPhase;
	int totalHoverMovementFrames;
	sf::Vector2f hoverMovementPerFrame;
	sf::Vector2f textHoverMovementPerFrame;

	void setSpriteTexture(sf::Sprite* spritePtr, sf::Texture* newTexturePtr);
	void updateColor();
	void setAtHover(bool newHover);
	void handleHoverMovement();
	bool checkMouseOver(sf::RenderWindow* window, sf::Vector2f relativeTopLeftPoint);
	void drawRenderTexture();
public:
	Button(sf::Texture* buttonTexturePtr, float posX = 0.f, float posY = 0.f, float width = 50.f, float height = 50.f,
		OriginPoint originPoint = OriginPoint::Topleft, sf::Vector2f relativeTopLeftPoint = sf::Vector2f(0, 0));
	~Button();

	sf::RenderTexture renderTexture;
	sf::Sprite sprite;
	sf::Text text;
	bool isActive = false;

	sf::Vector2f relativeTopLeftPoint;

	void setDefaultColor(sf::Color color);
	void setOverlayColor(sf::Color color);
	void setHoverColor(sf::Color color);
	void setActiveColor(sf::Color color);
	void setDisabledColor(sf::Color color);
	
	void setDefaultTexture(sf::Texture* texturePtr);
	void setOverlay(sf::Texture* texturePtr, sf::Color color = sf::Color(255, 255, 255, 255));
	void setHoverIcon(sf::Texture* texturePtr);
	void setActiveIcon(sf::Texture* texturePtr);
	void setDisabledIcon(sf::Texture* texturePtr);

	void setClickSound(sf::Sound* soundPtr);
	void setDisabledClickSound(sf::Sound* soundPtr);
	void setHoverSound(sf::Sound* soundPtr, bool playAtHoverEnter = true, bool playAtHoverExit = false);

	void setText(sf::Font* fontPtr, std::string string = "", sf::Vector2f relativePosition = sf::Vector2f(0, 0), float textSize = 50.f,
		sf::Color color = sf::Color(255, 255, 255, 255), OriginPoint originPoint = OriginPoint::Center);
	void setTextOriginPoint(OriginPoint newOriginPoint, sf::Vector2f customOriginPoint = sf::Vector2f(0.f, 0.f));

	void resetTextOriginPoint();

	void setHoverMovement(float x, float y, float time, float textX = 0.f, float textY = 0.f);

	bool checkClicked(sf::RenderWindow* window, bool ignoreDisabledParameter = false, bool playSound = true);
	void setIsActive(bool isActive);
	void setIsDisabled(bool isDisabled);

	sf::Texture getStandardTexture();
	
	void update(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window);
	void draw(sf::RenderTexture* renderTexturePtr);
};

