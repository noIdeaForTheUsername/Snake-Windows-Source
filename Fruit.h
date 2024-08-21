#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "SETTINGS.h"

class Fruit
{
private:
	float tileWidth;
	float tileHeight;

	sf::Texture* overlayTexturePtr = nullptr;
	sf::Text* overlayText = nullptr;
	sf::Color positiveTimeColor = sf::Color(153, 255, 255);
	sf::Color negativeTimeColor = sf::Color(255, 187, 153);

	static sf::Texture textures[];
	static sf::Texture overlayTextures[];
	static sf::SoundBuffer eatingSoundBuffer;
	static sf::Sound eatingSound;

	bool hasOverlay = false;
	bool hasText = false;
public:
	Fruit(float tileWidth, float tileHeight, int textureIndex = 0);
	~Fruit();

	int x;
	int y;
	int textureIndex = 0;
	int portalNumber = 0;
	int timeTravelNumber = 0;
	sf::Sprite sprite;
	sf::Sprite overlaySprite;

	sf::Vector2i moveVector;
	
	void playEatingSound();
	sf::Vector2i moveToRandomPosition(int maxX, int maxY);

	void setRandomMoveVector();
	void setPortalNumber(int portalNumber, sf::Color iconColor);
	void setTimeTravelNumber(int timeTravelNumber, sf::Color iconColor, sf::Font* fontPtr);
	void updateTimeTravelText(int presentPoint, int destinationPoint, float snakeMovesPerSecond);

	template <typename RenderObject>
	void draw(RenderObject& renderObject);

	static bool assetsLoaded;
	static std::vector<sf::Texture*> getTextures();
	static void loadAssets();
};

template <typename RenderObject>
void Fruit::draw(RenderObject& renderObject) {
	this->sprite.setPosition(this->x * tileWidth, this->y * tileHeight);
	this->overlaySprite.setPosition(this->x * tileWidth, this->y * tileHeight);
	if (this->hasText) this->overlayText->setPosition(this->sprite.getPosition().x + this->tileWidth / 2 + this->sprite.getLocalBounds().width / 2,
		this->sprite.getPosition().y + this->tileHeight / 3 + this->sprite.getLocalBounds().height / 2 - 5.f);
	renderObject.draw(this->sprite);
	if (this->hasOverlay) renderObject.draw(this->overlaySprite);
	if (this->hasText) renderObject.draw(*this->overlayText);
}

