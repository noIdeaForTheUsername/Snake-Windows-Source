#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

enum class TileIndicatorType {Portal, TimeTravel};

class TileIndicator
{
private:
	int textureIndex;
	float tileWidth;
	float tileHeight;

	TileIndicatorType type;

	static sf::Texture portalTextures[8];
	static sf::Texture timeTravelMarkerTextures[6];
public:
	TileIndicator(int x, int y, float tileWidth, float tileHeight, sf::Color color, float rotateDegrees);
	TileIndicator(int x, int y, float tileWidth, float tileHeight, sf::Color color, float rotateDegrees, int timeTravelNumber);

	static void loadAssets();
	static bool assetsLoaded;

	int x;
	int y;

	int timeTravelNumber;

	sf::Sprite sprite;

	void update();
	void updateTexture();

	template <typename RenderObject>
	void draw(RenderObject& renderObject);

};

template <typename RenderObject>
void TileIndicator::draw(RenderObject& renderObject) {
	renderObject.draw(this->sprite);
}

