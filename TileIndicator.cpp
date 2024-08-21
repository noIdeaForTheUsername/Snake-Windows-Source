#include "TileIndicator.h"

sf::Texture TileIndicator::portalTextures[8];
sf::Texture TileIndicator::timeTravelMarkerTextures[6];
bool TileIndicator::assetsLoaded = false;

TileIndicator::TileIndicator(int x, int y, float tileWidth, float tileHeight, sf::Color color, float rotateDegrees)
{
	this->x = x;
	this->y = y;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->textureIndex = 0;

	this->type = TileIndicatorType::Portal;
	this->sprite.setTexture(TileIndicator::portalTextures[0]);

	sf::Vector2u textureSize = this->sprite.getTexture()->getSize();
	this->sprite.setScale(tileWidth / textureSize.x, tileHeight / textureSize.y);
	this->sprite.setRotation(rotateDegrees);
	this->sprite.setColor(color);
	this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	this->sprite.setPosition((this->x * this->tileWidth) + this->tileWidth * 0.5f, (this->y * this->tileHeight) + this->tileHeight * 0.5f);
}
TileIndicator::TileIndicator(int x, int y, float tileWidth, float tileHeight, sf::Color color, float rotateDegrees, int timeTravelNumber)
{
	this->x = x;
	this->y = y;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;

	this->timeTravelNumber = timeTravelNumber;
	this->type = TileIndicatorType::TimeTravel;
	this->sprite.setTexture(TileIndicator::timeTravelMarkerTextures[timeTravelNumber - 1]);

	sf::Vector2u textureSize = this->sprite.getTexture()->getSize();
	this->sprite.setScale(tileWidth / textureSize.x, tileHeight / textureSize.y);
	this->sprite.setColor(color);
	this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	this->sprite.setPosition((this->x * this->tileWidth) + this->tileWidth * 0.5f, (this->y * this->tileHeight) + this->tileHeight * 0.5f);
	this->sprite.setRotation(rotateDegrees);
}

// STATIC
void TileIndicator::loadAssets() {
	TileIndicator::assetsLoaded = true;
	for (int i = 0; i < 8; i++) {
		TileIndicator::assetsLoaded = TileIndicator::assetsLoaded && TileIndicator::portalTextures[i].loadFromFile("Textures/Portals/portal" + std::to_string(i + 1) + ".png");
	}
	for (int i = 0; i < 5; i++) {
		TileIndicator::assetsLoaded = TileIndicator::assetsLoaded && TileIndicator::timeTravelMarkerTextures[i].loadFromFile("Textures/TimeTravelMarkers/marker" + std::to_string(i + 1) + ".png");
	}
	TileIndicator::assetsLoaded = TileIndicator::assetsLoaded && TileIndicator::timeTravelMarkerTextures[5].loadFromFile("Textures/snakeHead.png");
}

void TileIndicator::update() {
	this->updateTexture();
}

void TileIndicator::updateTexture() {
	if (this->type == TileIndicatorType::Portal)
	{
		this->textureIndex = (this->textureIndex + 1) % 8;
		this->sprite.setTexture(TileIndicator::portalTextures[this->textureIndex]);
	}	
}
