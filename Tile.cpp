#include "Tile.h"

sf::Texture Tile::texture;
sf::Texture Tile::wallTexture;
bool Tile::assetsLoaded = false;

sf::SoundBuffer Tile::wallSoundBuffer;
sf::Sound Tile::wallSound;

bool Tile::invisibleSnake = false;
int Tile::tileInvisibilityBegin = 3;

Tile::Tile(float posX, float posY, float width, float height, sf::Color tileColor)
{
	this->status = TileStatus::Empty;
	this->snakeRemaining = 0;
	this->fruitPtr = nullptr;

	this->emptyColor = tileColor;
	
	if (!Tile::assetsLoaded) Tile::loadAssets();

	this->sprite.setPosition(posX, posY);
	this->updateSpriteTexture(&Tile::texture, sf::Vector2f(width, height));
	this->sprite.setColor(this->emptyColor);
}

// Static function
void Tile::loadAssets()
{
	Tile::assetsLoaded = Tile::texture.loadFromFile("Textures/tile.png")
		&& Tile::wallTexture.loadFromFile("Textures/wall.png")
		&& Tile::wallSoundBuffer.loadFromFile("Audio/wall.mp3");
	Tile::wallSound.setBuffer(Tile::wallSoundBuffer);
	Tile::wallSound.setVolume(DEFAULT_VOLUME);
}

void Tile::updateSpriteTexture(sf::Texture* newTexture, sf::Vector2f newSize)
{
	if (newSize == sf::Vector2f(-1, -1)) {
		// The default function parameter is sf::Vector2f(-1, -1). If no other size is given, use the old sprite size.
		newSize = sf::Vector2f(this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height);
	}
	this->sprite.setTexture(*newTexture);
	sf::FloatRect textureSize = this->sprite.getLocalBounds();
	this->sprite.setScale(newSize.x / textureSize.width, newSize.y / textureSize.height);
}

TileStatus Tile::placeSnakeTile(int snakeLength, int totalSnakeLength = 2, sf::Color beginColor, sf::Color endColor) {
	TileStatus previousStatus = this->status;
	this->status = TileStatus::Snake;
	this->snakeRemaining = snakeLength;
	this->updateColor(totalSnakeLength, beginColor, endColor);
	return previousStatus;
}

void Tile::updateColor(int snakeLength, sf::Color beginColor, sf::Color endColor) {
	if (this->status == TileStatus::Snake) {
		float multiplier;
		if (Tile::invisibleSnake) {
			endColor = beginColor;
			endColor.a = 0;
			if (snakeLength - this->snakeRemaining < Tile::tileInvisibilityBegin) {
				multiplier = (float)(snakeLength - this->snakeRemaining) / (float)Tile::tileInvisibilityBegin;
			}
			else {
				multiplier = 1.f;
			}
		} else {
			multiplier = (float)(snakeLength - this->snakeRemaining) / (float)(snakeLength - 1);
		}
		float rDiff = endColor.r - beginColor.r;
		float gDiff = endColor.g - beginColor.g;
		float bDiff = endColor.b - beginColor.b;
		float aDiff = endColor.a - beginColor.a;
		sf::Color snakeColor = sf::Color(beginColor.r + round(rDiff * multiplier),
			beginColor.g + round(gDiff * multiplier),
			beginColor.b + round(bDiff * multiplier),
			beginColor.a + round(aDiff * multiplier));
		// Color blending
		if (snakeColor.a != 255) {
			snakeColor.r = round(snakeColor.r * ((float)snakeColor.a / 255) + this->emptyColor.r * (1 - ((float)snakeColor.a / 255)));
			snakeColor.g = round(snakeColor.g * ((float)snakeColor.a / 255) + this->emptyColor.g * (1 - ((float)snakeColor.a / 255)));
			snakeColor.b = round(snakeColor.b * ((float)snakeColor.a / 255) + this->emptyColor.b * (1 - ((float)snakeColor.a / 255)));
			snakeColor.a = 255;
		}

		if (this->sprite.getColor() != snakeColor) this->sprite.setColor(snakeColor);
		if (this->sprite.getTexture() != &Tile::texture) this->updateSpriteTexture(&Tile::texture);
	}
	else if (this->status == TileStatus::Wall) {
		if (this->sprite.getColor() != sf::Color(255, 255, 255, 255)) this->sprite.setColor(sf::Color(255, 255, 255, 255));
		if (this->sprite.getTexture() != &Tile::wallTexture) this->updateSpriteTexture(&Tile::wallTexture);
	}
	else if (this->status == TileStatus::Empty || this->status == TileStatus::Fruit) {
		if (this->sprite.getColor() != this->emptyColor) this->sprite.setColor(this->emptyColor);
		if (this->sprite.getTexture() != &Tile::texture) this->updateSpriteTexture(&Tile::texture);
	}
}

bool Tile::placeFruitTile(Fruit* fruitPtr)
{
	if (this->status == TileStatus::Empty) {
		this->fruitPtr = fruitPtr;
		this->status = TileStatus::Fruit;
		return true;
	}
	else {
		return false;
	}
}

void Tile::removeFruit(bool playSound)
{
	if (playSound) this->fruitPtr->playEatingSound();
	this->fruitPtr = nullptr;
	if (this->status == TileStatus::Fruit) {
		if (this->markedForWallPlacement) {
			this->placeWall();
		}
		else {
			this->status = TileStatus::Empty;
		}
	} 
}


bool Tile::markForWallPlacement()
{
	if (this->status == TileStatus::Empty) {
		this->markedForWallPlacement = true;
		this->placeWall();
		return true;
	} else if (this->status == TileStatus::Snake || this->status == TileStatus::Fruit) {
		this->markedForWallPlacement = true;
		return true;
	}
	else {
		return false;
	}
}
void Tile::placeWall() {
	this->markedForWallPlacement = true;
	this->status = TileStatus::Wall;
	this->updateColor(2, sf::Color::White, sf::Color::White);
	Tile::wallSound.stop();
	Tile::wallSound.setPitch(0.50f + ((float)(std::rand() % 60) + 1.f) / 60.f);
	Tile::wallSound.play();
}

void Tile::setSnakeInvisibility(bool invisibility, int tilesVisible)
{
	Tile::invisibleSnake = invisibility;
	Tile::tileInvisibilityBegin = tilesVisible;
}

void Tile::lengthenSnakeOnTile(int units)
{
	if (this->status == TileStatus::Snake) {
		this->snakeRemaining += units;
		if (this->snakeRemaining <= 0) {
			this->snakeRemaining = 0;
			if (this->markedForWallPlacement)
				this->placeWall();
			else
				this->status = TileStatus::Empty;
		}
	}
	else if (this->status == TileStatus::Empty) {
		this->snakeRemaining = 0;
	}
}

void Tile::moveSnakeOnTile(int snakeLength, sf::Color beginColor, sf::Color endColor)
{
	this->lengthenSnakeOnTile(-1);
	this->updateColor(snakeLength, beginColor, endColor);
}

