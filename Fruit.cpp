#include "Fruit.h"

sf::Texture Fruit::textures[9];
sf::Texture Fruit::overlayTextures[2];

sf::SoundBuffer Fruit::eatingSoundBuffer;
sf::Sound Fruit::eatingSound;
bool Fruit::assetsLoaded = false;

Fruit::Fruit(float tileWidth, float tileHeight, int textureIndex) : moveVector(0, 0)
{
	this->x = 0;
	this->y = 0;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->textureIndex = textureIndex;

	this->sprite.setTexture(Fruit::textures[this->textureIndex]);
	sf::Vector2u textureSize = this->sprite.getTexture()->getSize();
	this->sprite.setScale(tileWidth / textureSize.x, tileHeight / textureSize.y);
	this->sprite.setPosition(0, 0);
}

Fruit::~Fruit() {
	delete this->overlayTexturePtr;
}

// STATIC
void Fruit::loadAssets() {
	if (!Fruit::assetsLoaded) {
		Fruit::assetsLoaded = Fruit::textures[0].loadFromFile("Textures/apple.png")
			&& Fruit::textures[1].loadFromFile("Textures/banana.png")
			&& Fruit::textures[2].loadFromFile("Textures/orange.png")
			&& Fruit::textures[3].loadFromFile("Textures/melon.png")
			&& Fruit::textures[4].loadFromFile("Textures/cherries.png")
			&& Fruit::textures[5].loadFromFile("Textures/blueberries.png")
			&& Fruit::textures[6].loadFromFile("Textures/pineapple.png")
			&& Fruit::textures[7].loadFromFile("Textures/chocolate.png")
			&& Fruit::textures[8].loadFromFile("Textures/brokenFruit.png")
			&& Fruit::overlayTextures[0].loadFromFile("Textures/portalIcon.png")
			&& Fruit::overlayTextures[1].loadFromFile("Textures/clockIcon.png")
			&& Fruit::eatingSoundBuffer.loadFromFile("Audio/eating.mp3");
		Fruit::eatingSound.setBuffer(Fruit::eatingSoundBuffer);
		Fruit::eatingSound.setVolume(DEFAULT_VOLUME);
	}
}


std::vector<sf::Texture*> Fruit::getTextures()
{
	std::vector<sf::Texture*> toReturn;

	for (auto& texture : textures) {
		toReturn.push_back(&texture);
	}

	return toReturn;
}
// STATIC END


sf::Vector2i Fruit::moveToRandomPosition(int maxX, int maxY)
{
	this->x = std::rand() % (maxX + 1);
	this->y = std::rand() % (maxY + 1);
	this->sprite.setPosition(this->x * tileWidth, this->y * tileHeight);
	this->overlaySprite.setPosition(this->x * tileWidth, this->y * tileHeight);
	return sf::Vector2i(this->x, this->y);
}

void Fruit::playEatingSound() {
	Fruit::eatingSound.play();
}

void Fruit::setRandomMoveVector() {
	bool diagonalMovement = std::rand() % 10 == 0;  // 1 per 10 fruits moves diagonally
	int movementType;  // 1, 2, 3, 4 - Up, Right, Down, Left |   5, 6, 7, 8 - Right-up, Right-Down, Left-Down, Left-Up, 
	movementType = std::rand() % 4 + (diagonalMovement ? 5 : 1);
	switch (movementType)
	{
	default:
	case 1:
		this->moveVector = sf::Vector2i(0, -1);
		break;
	case 2:
		this->moveVector = sf::Vector2i(1, 0);
		break;
	case 3:
		this->moveVector = sf::Vector2i(0, 1);
		break;
	case 4:
		this->moveVector = sf::Vector2i(-1, 0);
		break;
	case 5:
		this->moveVector = sf::Vector2i(1, -1);
		break;
	case 6:
		this->moveVector = sf::Vector2i(1, 1);
		break;
	case 7:
		this->moveVector = sf::Vector2i(-1, 1);
		break;
	case 8:
		this->moveVector = sf::Vector2i(-1, -1);
		break;
	}
}


void Fruit::setPortalNumber(int portalNumber, sf::Color iconColor)
{
	this->hasOverlay = true;
	this->portalNumber = portalNumber;
	this->overlaySprite.setTexture(Fruit::overlayTextures[0]);
	sf::Vector2u textureSize = this->overlaySprite.getTexture()->getSize();
	this->overlaySprite.setScale(this->tileWidth / textureSize.x, this->tileHeight / textureSize.y);
	this->overlaySprite.setColor(iconColor);
	this->overlaySprite.setPosition(this->x * tileWidth, this->y * tileHeight);
}

void Fruit::setTimeTravelNumber(int timeTravelNumber, sf::Color iconColor, sf::Font* fontPtr) {
	this->hasOverlay = true;
	this->timeTravelNumber = timeTravelNumber;
	this->overlaySprite.setTexture(Fruit::overlayTextures[1]);
	sf::Vector2u textureSize = this->overlaySprite.getTexture()->getSize();
	this->overlaySprite.setScale(this->tileWidth / textureSize.x, this->tileHeight / textureSize.y);
	this->overlaySprite.setColor(iconColor);
	this->overlaySprite.setPosition(this->x * tileWidth, this->y * tileHeight);

	this->hasText = true;
	this->overlayText = new sf::Text();
	this->overlayText->setFont(*fontPtr);
	this->overlayText->setString("-0.0s");
	this->overlayText->setCharacterSize(15.f);
	this->overlayText->setFillColor(this->negativeTimeColor);
	this->overlayText->setOrigin(this->overlayText->getLocalBounds().width / 2, this->overlayText->getLocalBounds().height / 2);
	this->overlayText->setPosition(this->sprite.getPosition().x + this->tileWidth / 2 + this->sprite.getLocalBounds().width / 2,
		this->sprite.getPosition().y + this->tileHeight / 3 + this->sprite.getLocalBounds().height / 2 - 5.f);
}

void Fruit::updateTimeTravelText(int presentPoint, int destinationPoint, float snakeMovesPerSecond)
{
	bool teleportionIntoFuture = destinationPoint <= presentPoint;
	int movesDifference = abs(destinationPoint - presentPoint);
	float timeDifference = (float)movesDifference / snakeMovesPerSecond;
	int secondsDifference = floor(timeDifference);
	float decimalFloat = floor(timeDifference / 0.2f) * 0.2f - secondsDifference;
	if (secondsDifference == 0 && decimalFloat == 0.f) decimalFloat = 0.2f;

	std::string symbol = teleportionIntoFuture ? "+" : "-";
	std::string decimalString = std::to_string(decimalFloat);
	decimalString = decimalString.substr(1, 2);
	this->overlayText->setString(symbol + std::to_string(secondsDifference) + decimalString + "s");
	this->overlayText->setFillColor(teleportionIntoFuture ? positiveTimeColor : negativeTimeColor);
	this->overlayText->setOrigin(this->overlayText->getLocalBounds().width / 2, this->overlayText->getLocalBounds().height / 2);
	this->overlayText->setPosition(this->sprite.getPosition().x + this->tileWidth / 2 + this->sprite.getLocalBounds().width / 2,
		this->sprite.getPosition().y + this->tileHeight / 3 + this->sprite.getLocalBounds().height / 2 - 5.f);
}

