#include "Snake.h"


Snake::Snake(sf::Vector2f headSize, int startX, int startY, int startLength, FacingDirection startFacingDirection, int minGradientLength, bool gradientEnabled)
	: headSprite()
{

	this->loadSuccessful = true;
	this->headX = startX;
	this->headY = startY;
	this->tileWidth = headSize.x;
	this->tileHeight = headSize.y;
	this->length = startLength;
	this->facingDirection = startFacingDirection;
	this->queuedFacingDirection = this->facingDirection;
	this->minGradientLength = minGradientLength;
	this->gradientEnabled = gradientEnabled;
	this->setColors(SNAKE_COLOR_HEAD, SNAKE_COLOR_ENDING);

	this->loadSuccessful = this->loadSuccessful && this->headTexture.loadFromFile("Textures/snakeHead.png")
		&& this->pausedHeadTexture.loadFromFile("Textures/snakeHeadPaused.png");
	this->headSprite.setTexture(this->headTexture);
	sf::Vector2u headTextureSize = this->headTexture.getSize();
	this->headSprite.setScale(tileWidth / headTextureSize.x, tileHeight / headTextureSize.y);
	this->headSprite.setOrigin(sf::Vector2f(this->headSprite.getLocalBounds().width / 2, this->headSprite.getLocalBounds().height / 2));
	int rotateAngle = ((int)this->facingDirection - 1) * 90;
	this->headSprite.setRotation(rotateAngle);
	this->headSprite.setPosition(this->headX * this->tileWidth + this->tileWidth * 0.5, this->headY * this->tileHeight + this->tileHeight * 0.5);
}

bool Snake::getLoadSuccessful() {
	return this->loadSuccessful;
}

void Snake::setColors(sf::Color beginColor, sf::Color finalEndingColor) {
	delete this->endingColors;
	this->endingColors = new sf::Color[this->minGradientLength];
	this->headColor = beginColor;
	if (this->gradientEnabled) {
		this->finalEndingColor = finalEndingColor;
		//if (this->oppositeGradientColor)
		//	this->finalEndingColor = sf::Color(255 - this->headColor.r, 255 - this->headColor.g, 255 - this->headColor.b, this->headColor.a);
		float rDiff = (float)(this->finalEndingColor.r - this->headColor.r) / (this->minGradientLength - 1);
		float gDiff = (float)(this->finalEndingColor.g - this->headColor.g) / (this->minGradientLength - 1);
		float bDiff = (float)(this->finalEndingColor.b - this->headColor.b) / (this->minGradientLength - 1);
		float aDiff = (float)(this->finalEndingColor.a - this->headColor.a) / (this->minGradientLength - 1);

		for (int i = 0; i < this->minGradientLength; i++) {
			this->endingColors[i] = sf::Color(this->headColor.r + round(rDiff * i), this->headColor.g + round(gDiff * i),
				this->headColor.b + round(bDiff * i), this->headColor.a + round(aDiff * i));
		}

		if (this->length < this->minGradientLength)
			this->endingColor = this->endingColors[this->length - 1];
		else
			this->endingColor = this->finalEndingColor;
	}
	else {
		this->endingColor = this->headColor;
		this->finalEndingColor = this->headColor;
	}
}

void Snake::setRandomColors()
{
	sf::Color color1;
	sf::Color color2;

	if (this->randomizeHeadColor) {
		color1 = this->getRandomColor(255);
	}
	else
		color1 = this->headColor;

	if (this->randomizeEndingColor) {
		color2 = this->getRandomColor(255);
	}
	else if (this->randomizeHeadColor && this->oppositeGradientColor) {  // When the head color is randomized, and the ending color is opposite
		color2.r = 255 - color1.r;
		color2.g = 255 - color1.g;
		color2.b = 255 - color1.b;
	}
	else {
		color2 = this->finalEndingColor;
		if ( (color2.a == SNAKE_GRADIENT_TRANSPARENCY) && this->randomizeHeadColor) {  // When the head color is randomized, and the ending color is semi-transparent
			color2.r = color1.r;
			color2.g = color1.g;
			color2.b = color1.b;
		}
	}

	this->setColors(color1, color2);
}

sf::Color Snake::getRandomColor(int minAlpha)
{
	bool correctColor = false;
	sf::Color color;
	do {
		color = sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256, 255 - std::rand() % (256 - minAlpha));

		int max = color.r;
		if (color.g > max) max = color.g;
		if (color.b > max) max = color.b;
		int min = color.r;
		if (color.g < min) min = color.g;
		if (color.b < min) min = color.b;

		float colorSaturation = (max - min) / max;
		float colorValue = max / 255;

		// Exclude dark and/or low-saturated colors
		correctColor = (colorSaturation >= 0.3f && colorValue >= 0.2f && (colorSaturation + colorValue) >= 1.f);

	} while (!correctColor);
	return color;
}

void Snake::setRandomizeColors(bool headColor, bool endingColor)
{
	this->randomizeHeadColor = headColor;
	this->randomizeEndingColor = endingColor;
}

void Snake::setEmergencyColors()
{
	this->emergencyColors = true;
	this->headColor = this->emergencyColor1;
	this->endingColor = this->emergencyColor1;
}

void Snake::setRainbow()
{
	this->rainbow = true;
	this->setColors(sf::Color(255, 0, 0, 255), this->finalEndingColor);
}

void Snake::setOppositeGradientColor()
{
	this->oppositeGradientColor = true;
}

void Snake::updateFacingDirection(FacingDirection newFacingDirection)
{
	if (this->canChangeDirection
		&& newFacingDirection != this->facingDirection && newFacingDirection != this->getOppositeFacingDirection(this->facingDirection)) {
		this->canChangeDirection = false;
		this->facingDirection = newFacingDirection;
		this->queuedFacingDirection = newFacingDirection;
	}
	else if (!this->canChangeDirection && newFacingDirection != this->getOppositeFacingDirection(this->facingDirection)) {
		this->queuedFacingDirection = newFacingDirection;
	}
	
}

void Snake::changeLength(int units) {
	this->length += units;
	if (this->length < 1) this->length = 1;
	this->updateEndingColor();
}

void Snake::updateEndingColor() {
	if (this->length < this->minGradientLength && this->gradientEnabled) this->endingColor = this->endingColors[this->length - 1];
	else this->endingColor = this->finalEndingColor;
}

sf::Vector2i Snake::move(int tiles)
{
	switch (this->facingDirection) {
	case FacingDirection::Right:
		this->headX += tiles;
		break;
	case FacingDirection::Left:
		this->headX -= tiles;
		break;
	case FacingDirection::Up:
		this->headY -= tiles;
		break;
	case FacingDirection::Down:
		this->headY += tiles;
		break;
	}
	int rotateAngle = ((int)this->facingDirection - 1) * 90;
	this->headSprite.setRotation(rotateAngle);
	this->headSprite.setPosition(this->headX * this->tileWidth + this->tileWidth * 0.5, this->headY * this->tileHeight + this->tileHeight * 0.5);

	this->canChangeDirection = true;
	this->updateFacingDirection(this->queuedFacingDirection);

	return sf::Vector2i(this->headX, this->headY);
}

void Snake::setPausedTexture(bool paused)
{
	sf::Vector2u newTextureSize;
	sf::FloatRect oldSpriteSize = this->headSprite.getGlobalBounds();
	if (paused) {
		newTextureSize = this->pausedHeadTexture.getSize();
		this->headSprite.setTexture(this->pausedHeadTexture);
	}
	else {
		newTextureSize = this->headTexture.getSize();
		this->headSprite.setTexture(this->headTexture);
	}
	this->headSprite.setScale(oldSpriteSize.width / newTextureSize.x, oldSpriteSize.height / newTextureSize.y);
}

void Snake::teleportHead(int newX, int newY, FacingDirection newFacingDirection)
{
	this->headX = newX;
	this->headY = newY;
	this->facingDirection = newFacingDirection;
	this->queuedFacingDirection = newFacingDirection;

	int rotateAngle = ((int)this->facingDirection - 1) * 90;
	this->headSprite.setRotation(rotateAngle);
	this->headSprite.setPosition(this->headX * this->tileWidth + this->tileWidth * 0.5, this->headY * this->tileHeight + this->tileHeight * 0.5);

	this->canChangeDirection = true;
}

void Snake::update(int currentFrameNumber)
{
	if (this->rainbow) this->updateRainbowColors();
	else if (this->emergencyColors) this->updateEmergencyColors(currentFrameNumber);
}

void Snake::updateRainbowColors()
{
	/* PHASES:
	1. 255, 0 (rising), 0
	2. 255 (lowering), 255, 0
	3. 0, 255, 0 (rising)
	4. 0, 255 (lowering), 255
	5. 0 (rising), 0, 255
	6. 255, 0, 255 (lowering)
	*/ 
	sf::Color color = this->headColor;
	sf::Color endingColor = this->finalEndingColor;

	int changePerFrame = 15;
	if (color.r == 255 && color.g < 255 && color.b == 0) {  // Phase 1
		color.g += changePerFrame;
	}
	else if (color.r > 0 && color.g == 255 && color.b == 0) {  // Phase 2
		color.r -= changePerFrame;
	}
	else if (color.r == 0 && color.g == 255 && color.b < 255) {  // Phase 3
		color.b += changePerFrame;
	}
	else if (color.r == 0 && color.g > 0 && color.b == 255) {  // Phase 4
		color.g -= changePerFrame;
	}
	else if (color.r < 255 && color.g == 0 && color.b == 255) {  // Phase 5
		color.r += changePerFrame;
	}
	else if (color.r == 255 && color.g == 0 && color.b > 0) {  // Phase 6
		color.b -= changePerFrame;
	}

	if (this->finalEndingColor.a == SNAKE_GRADIENT_TRANSPARENCY) { // When the head color is rainbow, and the ending color is semi-transparent
		endingColor.r = color.r;
		endingColor.g = color.g;
		endingColor.b = color.b;
	}
	else if (this->oppositeGradientColor) { // When the head color is rainbow, and the ending color is opposite
		endingColor.r = 255 - color.r;
		endingColor.g = 255 - color.g;
		endingColor.b = 255 - color.b;
	}

	this->setColors(color, endingColor);
}

void Snake::updateEmergencyColors(int currentFrameNumber)
{
	sf::Color color;
	sf::Color endingColor = this->finalEndingColor;

	if (currentFrameNumber % FPS >= (FPS / 2))
		color = this->emergencyColor2;
	else
		color = this->emergencyColor1;

	if (this->headColor != color) {
		if (this->finalEndingColor.a == SNAKE_GRADIENT_TRANSPARENCY) { // When the ending color is semi-transparent
			endingColor.r = color.r;
			endingColor.g = color.g;
			endingColor.b = color.b;
		}
		else if (this->oppositeGradientColor) { // When the ending color is opposite
			if (color == this->emergencyColor1) endingColor = this->emergencyColor2;
			else endingColor = this->emergencyColor1;
		}
		this->setColors(color, endingColor);
	}
}

FacingDirection Snake::getOppositeFacingDirection(FacingDirection facingDirection)
{
	return (FacingDirection)(((int)facingDirection + 1) % 4 + 1);
}
