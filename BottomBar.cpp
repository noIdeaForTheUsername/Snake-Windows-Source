#include "BottomBar.h"


BottomBar::BottomBar(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr, sf::Texture* unlockedAchievementTexturePtr, sf::Texture* lockedAchievementTexturePtr,
	sf::Sound* achievementSoundPtr)
	: renderTexture(), pointText()
{
	this->loadSuccessful = true;

	this->renderTexture.create(width, height);
	this->sprite.setTexture(this->renderTexture.getTexture());
	this->sprite.setPosition(topLeft);

	this->loadSuccessful = this->loadSuccessful && this->backgroundTexture.loadFromFile("Textures/bottomBar.png")
		&& this->menuButtonTexture.loadFromFile("Textures/buttonMenu.png")
		&& this->timerTexture.loadFromFile("Textures/timer.png");

	this->initSprite(&this->backgroundSprite, &this->backgroundTexture, 0, 0, width, height);

	this->menuButtonPtr = new Button(&this->menuButtonTexture, 60.f, height / 2, 100.f, 100.f, OriginPoint::Center, this->sprite.getPosition());
	this->menuButtonPtr->setHoverColor(sf::Color(128, 128, 128, 255));

	this->achievementSoundPtr = achievementSoundPtr;

	float achievementWidth = width / 13.f * 9.f;
	this->achievementNotificationHeight = height / 2.f;
	this->unlockedAchievementSprite.setTexture(*unlockedAchievementTexturePtr);
	this->lockedAchievementSprite.setTexture(*lockedAchievementTexturePtr);
	this->timerSprite.setTexture(this->timerTexture);

	sf::Vector2u textureSize = this->unlockedAchievementSprite.getTexture()->getSize();
	this->unlockedAchievementSprite.setScale(achievementWidth / textureSize.x, this->achievementNotificationHeight / textureSize.y);
	this->unlockedAchievementSprite.setOrigin(this->unlockedAchievementSprite.getLocalBounds().width / 2.f, 0.f);
	this->unlockedAchievementSprite.setPosition(width / 2.f, height);
	textureSize = this->lockedAchievementSprite.getTexture()->getSize();
	this->lockedAchievementSprite.setScale(achievementWidth / textureSize.x, this->achievementNotificationHeight / textureSize.y);
	this->lockedAchievementSprite.setOrigin(this->lockedAchievementSprite.getLocalBounds().width / 2.f, 0.f);
	this->lockedAchievementSprite.setPosition(width / 2.f, height);

	textureSize = this->timerSprite.getTexture()->getSize();
	this->timerSprite.setScale((this->backgroundSprite.getGlobalBounds().width / 13.f) / textureSize.x, (this->backgroundSprite.getGlobalBounds().height / 2.f) / textureSize.y);
	this->timerSprite.setOrigin(0.f, 0.f);
	this->timerSprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 13.f * 11.f, this->backgroundSprite.getGlobalBounds().height / 2.f - 15.f);
	
	this->pointsDisplayed = 0;
	this->fontPtr = fontPtr;
	this->initText();
}
BottomBar::~BottomBar() {
	delete this->menuButtonPtr;
}

void BottomBar::initSprite(sf::Sprite* spritePtr, sf::Texture* texturePtr, float posX, float posY, float width, float height) {
	spritePtr->setTexture(*texturePtr);
	sf::FloatRect spriteSize = spritePtr->getLocalBounds();
	spritePtr->setScale(width / spriteSize.width, height / spriteSize.height);
	spritePtr->setPosition(posX, posY);
}

bool BottomBar::getLoadSuccessful() {
	return this->loadSuccessful;
}

void BottomBar::setButtonSounds(sf::Sound* hoverSoundPtr, sf::Sound* clickSoundPtr)
{
	this->buttonHoverSoundPtr = hoverSoundPtr;
	this->buttonClickSoundPtr = clickSoundPtr;

	if (this->buttonHoverSoundPtr != nullptr) this->menuButtonPtr->setHoverSound(hoverSoundPtr);
	if (this->buttonClickSoundPtr != nullptr) this->menuButtonPtr->setClickSound(clickSoundPtr);
}

void BottomBar::setColor(sf::Color color)
{
	this->backgroundSprite.setColor(color);
	//this->menuButtonPtr->setDefaultColor(color);
}

void BottomBar::setTimeTravelModeTextVisibility(bool visible)
{
	this->showTimeTravelModeText = visible;
}

void BottomBar::initText()
{
	this->pointText.setFont(*this->fontPtr);
	this->pointText.setString("Points: 0");
	this->pointText.setCharacterSize(50.f);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setOrigin(this->pointText.getLocalBounds().width / 2, this->pointText.getLocalBounds().height / 2 + 10.f);
	this->pointText.setPosition(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
	this->pointTextDefaultY = this->pointText.getPosition().y;

	this->presentText.setFont(*this->fontPtr);
	this->presentText.setString("The present: 0:00.0");
	this->presentText.setCharacterSize(25.f);
	this->presentText.setFillColor(sf::Color(200, 200, 200));
	this->presentText.setOrigin(this->presentText.getLocalBounds().width / 2, this->presentText.getLocalBounds().height / 2 + 5.f);
	this->presentText.setPosition(this->backgroundSprite.getGlobalBounds().width / 2, 25.f);
	this->presentTextDefaultY = this->presentText.getPosition().y;
	this->presentTextDefaultColor = this->presentText.getFillColor();

	this->timerText.setFont(*this->fontPtr);
	this->timerText.setString("0:00.00");
	this->timerText.setCharacterSize(25.f);
	this->timerText.setFillColor(sf::Color(200, 200, 200));
	this->timerText.setOrigin(this->timerText.getLocalBounds().width / 2, this->timerText.getLocalBounds().height / 2 + 5.f);
	this->timerText.setPosition(this->backgroundSprite.getGlobalBounds().width / 13.f * 11.5f, this->sprite.getLocalBounds().height / 4.f);

	this->achievementNameText.setFont(*this->fontPtr);
	this->achievementNameText.setCharacterSize(35.f);
	this->achievementNameText.setFillColor(sf::Color(sf::Color::White));
	// Origin is set in displayAchievementNotification (the origin point has to be updated after the string is updated)
	this->achievementNameText.setPosition(this->backgroundSprite.getGlobalBounds().width / 2, this->backgroundSprite.getGlobalBounds().height + this->achievementNameText.getGlobalBounds().height / 2);

	this->achievementUnlockedText.setFont(*this->fontPtr);
	this->achievementUnlockedText.setString("UNLOCKED!");
	this->achievementUnlockedText.setCharacterSize(40.f);
	this->achievementUnlockedText.setFillColor(sf::Color::White);
	this->achievementUnlockedText.setPosition(this->backgroundSprite.getGlobalBounds().width / 2, this->backgroundSprite.getGlobalBounds().height + this->achievementUnlockedText.getGlobalBounds().height / 2);
}

void BottomBar::update(sf::RenderWindow* window, int points)
{
	if (this->pointsDisplayed != points) {
		this->pointsDisplayed = points;
		this->pointText.setString("Points: " + std::to_string(points));
		this->pointText.setOrigin(this->pointText.getLocalBounds().width / 2, this->pointText.getLocalBounds().height / 2 + 10.f);
	}
	this->handleAchievementNotification();
	this->menuButtonPtr->update(window);
}

void BottomBar::updateTimer(float timeInSeconds) {
	int minutes = floor(timeInSeconds / 60.f);
	int seconds = floor(timeInSeconds - minutes * 60);
	int hundredths = (timeInSeconds - minutes * 60 - seconds) * 100.f;
	std::string text = to_string(minutes) + ":" + (seconds >= 10 ? "" : "0") + to_string(seconds) + "." + (hundredths >= 10 ? "" : "0") + to_string(hundredths);
	this->timerText.setString(text);
	this->timerText.setOrigin(this->timerText.getLocalBounds().width / 2, this->timerText.getLocalBounds().height / 2 + 5.f);
}

void BottomBar::updateTimeTravelText(int boardUpdateFrames, int moveCounter) {
	float timeInSeconds = (float)moveCounter / ((float)FPS / boardUpdateFrames);
	int minutes = floor(timeInSeconds / 60.f);
	int seconds = floor(timeInSeconds) - minutes * 60;
	std::string text = "The present: " + std::to_string(minutes) + ":" + (seconds >= 10 ? "" : "0") + std::to_string(seconds);
	this->presentText.setString(text);
	this->presentText.setOrigin(this->presentText.getLocalBounds().width / 2, this->presentText.getLocalBounds().height / 2 + 5.f);
}


void BottomBar::displayAchievementNotification(string achievementName, float notificationVisibilityTime)
{
	this->achievementNameText.setString(achievementName);
	this->achievementDisplayPhaseFrames = round(notificationVisibilityTime * FPS);  // Total frames
	this->achievementDisplayPhaseFrames = ceil((float)this->achievementDisplayPhaseFrames / 10.f);  // Frames in one phase (10 phases)
	this->achievementDisplayCurrentFrames = 0;

	this->lockedAchievementSprite.setColor(sf::Color(255, 255, 255, 255));
	this->unlockedAchievementSprite.setColor(sf::Color(255, 255, 255, 255));
	this->achievementNameText.setFillColor(sf::Color(255, 255, 255, 255));
	this->achievementUnlockedText.setFillColor(sf::Color(255, 255, 255, 255));

	this->achievementUnlockedText.setOrigin(this->achievementUnlockedText.getLocalBounds().width / 2, this->achievementUnlockedText.getLocalBounds().height / 2 + 7.f);
	this->achievementNameText.setOrigin(this->achievementNameText.getLocalBounds().width / 2, this->achievementNameText.getLocalBounds().height / 2 + 7.f);

	this->achievementSoundPtr->play();
}

void BottomBar::resetAchievementNotification() {
	this->unlockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition().x, this->backgroundSprite.getGlobalBounds().height);
	this->lockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition());
	this->achievementNameText.setPosition(this->unlockedAchievementSprite.getPosition().x + (this->unlockedAchievementSprite.getGlobalBounds().width / 9.f / 2.f),
		this->unlockedAchievementSprite.getPosition().y + this->unlockedAchievementSprite.getGlobalBounds().height / 2.f);
	this->achievementUnlockedText.setPosition(this->achievementNameText.getPosition());

	this->unlockedAchievementSprite.setColor(sf::Color::White);
	this->lockedAchievementSprite.setColor(sf::Color::White);
	this->achievementNameText.setFillColor(sf::Color::White);
	this->achievementUnlockedText.setFillColor(sf::Color::White);

	this->achievementDisplayCurrentFrames = -2;

	this->pointText.setPosition(this->pointText.getPosition().x, this->pointTextDefaultY);
	this->presentText.setPosition(this->presentText.getPosition().x, this->presentTextDefaultY);
	this->presentText.setFillColor(this->presentTextDefaultColor);
}

void BottomBar::handleAchievementNotification() {
	/* PHASES:
	1. MOVE IN: 0-1
	2. STAY WITH "UNLOCKED" TEXT: 1-3
	3. ALPHA TRANSITION: 3-4
	4. STAY WITH ACHIEVEMENT NAME: 4-9
	5. MOVE OUT: 9-10
	*/
	float externalTextMovementMultipler = 0.5f;

	if (this->achievementDisplayCurrentFrames >= 0) {
		if (this->achievementDisplayCurrentFrames <= this->achievementDisplayPhaseFrames * 1) {
			// PHASE 1
			float offsetY = (float)this->achievementDisplayCurrentFrames / (this->achievementDisplayPhaseFrames * 1.f) * this->achievementNotificationHeight;

			this->unlockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition().x, this->backgroundSprite.getGlobalBounds().height - offsetY);
			this->lockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition());
			this->achievementNameText.setPosition(this->unlockedAchievementSprite.getPosition().x + (this->unlockedAchievementSprite.getGlobalBounds().width / 9.f / 2.f),
				this->unlockedAchievementSprite.getPosition().y + this->unlockedAchievementSprite.getGlobalBounds().height / 2.f);
			this->achievementUnlockedText.setPosition(this->achievementNameText.getPosition());

			this->pointText.setPosition(this->pointText.getPosition().x, this->pointTextDefaultY - offsetY * externalTextMovementMultipler);
			this->presentText.setPosition(this->presentText.getPosition().x, this->presentTextDefaultY - offsetY * externalTextMovementMultipler);
			this->presentText.setFillColor(sf::Color(this->presentTextDefaultColor.r, this->presentTextDefaultColor.g, this->presentTextDefaultColor.b,
				round(255.f - 255.f * (float)this->achievementDisplayCurrentFrames / (this->achievementDisplayPhaseFrames * 1.f))));
		}
		else if (this->achievementDisplayCurrentFrames <= this->achievementDisplayPhaseFrames * 3) {
			// PHASE 2 - WAITING UNTIL PHASE 3
		}
		else if (this->achievementDisplayCurrentFrames <= this->achievementDisplayPhaseFrames * 4) {
			// PHASE 3
			this->lockedAchievementSprite.setColor(sf::Color(255, 255, 255, 255 - round(255.f *
				(float)(this->achievementDisplayCurrentFrames - this->achievementDisplayPhaseFrames * 3) / (float)this->achievementDisplayPhaseFrames)));
			this->achievementUnlockedText.setFillColor(this->lockedAchievementSprite.getColor());
		}
		else if (this->achievementDisplayCurrentFrames <= this->achievementDisplayPhaseFrames * 9) {
			// PHASE 4 - WAITING UNTIL PHASE 5
		}
		else if (this->achievementDisplayCurrentFrames <= this->achievementDisplayPhaseFrames * 10) {
			// PHASE 5
			float offsetY = this->achievementNotificationHeight -
				(((float)this->achievementDisplayCurrentFrames - this->achievementDisplayPhaseFrames * 9.f) / (this->achievementDisplayPhaseFrames * 1.f) * this->achievementNotificationHeight);

			this->unlockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition().x, this->backgroundSprite.getGlobalBounds().height - offsetY);
			this->lockedAchievementSprite.setPosition(this->unlockedAchievementSprite.getPosition());
			this->achievementNameText.setPosition(this->unlockedAchievementSprite.getPosition().x + (this->unlockedAchievementSprite.getGlobalBounds().width / 9.f / 2.f),
				this->unlockedAchievementSprite.getPosition().y + this->unlockedAchievementSprite.getGlobalBounds().height / 2.f);
			this->achievementUnlockedText.setPosition(this->achievementNameText.getPosition());

			this->pointText.setPosition(this->pointText.getPosition().x, this->pointTextDefaultY - offsetY * externalTextMovementMultipler);
			this->presentText.setPosition(this->presentText.getPosition().x, this->presentTextDefaultY - offsetY * externalTextMovementMultipler);

			this->presentText.setFillColor(sf::Color(this->presentTextDefaultColor.r, this->presentTextDefaultColor.g, this->presentTextDefaultColor.b,
				round(255.f * (float)(this->achievementDisplayCurrentFrames - this->achievementDisplayPhaseFrames * 9) / (this->achievementDisplayPhaseFrames * 1.f))));
		}
		else {
			this->resetAchievementNotification();
		}

		this->achievementDisplayCurrentFrames++;
	}
}


bool BottomBar::getMenuButtonClicked(sf::RenderWindow* window) {
	return this->menuButtonPtr->checkClicked(window);
}


void BottomBar::drawRenderTexture() {
	this->renderTexture.clear();
	this->renderTexture.draw(this->backgroundSprite);
	this->renderTexture.draw(this->timerSprite);
	if (this->achievementDisplayCurrentFrames >= 0) {
		this->renderTexture.draw(this->unlockedAchievementSprite);
		this->renderTexture.draw(this->achievementNameText);
		this->renderTexture.draw(this->lockedAchievementSprite);
		this->renderTexture.draw(this->achievementUnlockedText);
	}
	if (this->showTimeTravelModeText) this->renderTexture.draw(this->presentText);
	this->renderTexture.draw(this->timerText);
	this->renderTexture.draw(this->pointText);
	this->menuButtonPtr->draw(&this->renderTexture);
	this->renderTexture.display();
}

void BottomBar::draw(sf::RenderWindow* renderWindowPtr) {
	this->drawRenderTexture();
	this->sprite.setTexture(this->renderTexture.getTexture());
	renderWindowPtr->draw(this->sprite);
}
void BottomBar::draw(sf::RenderTexture* renderTexturePtr) {
	this->drawRenderTexture();
	this->sprite.setTexture(this->renderTexture.getTexture());
	renderTexturePtr->draw(this->sprite);
}

