#include "Menu.h"


Menu::Menu(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr,
	sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr, int gameMusicTracksNumber,
	sf::Sound* buttonHoverSoundPtr, sf::Sound* buttonClickSoundPtr, sf::Sound* buttonDisabledClickSoundPtr) : dataManager() {
	this->loadSuccessful = true;
	this->loadData();
	this->initVariables(gameMusicTracksNumber);
	this->initAssetsAndSprites(width, height, topLeft, fontPtr, unlockedAchievementLabelTexturePtr, lockedAchievementLabelTexturePtr,
		buttonHoverSoundPtr, buttonClickSoundPtr, buttonDisabledClickSoundPtr);
	this->initMenuPages();
	this->reloadAchievementStatus();
}

Menu::~Menu()
{
	delete backButtonPtr, customizationTab1ButtonPtr, customizationTab2ButtonPtr,
		playButtonPtr, gamemodesButtonPtr, customizeButtonPtr, settingsButtonPtr;
	// Settings
	for (int i = 0; i < this->snakeSpeedButtonsNumber; i++) {
		delete this->snakeSpeedButtons[i];
	}
	for (int i = 0; i < this->boardSizeButtonsNumber; i++) {
		delete this->boardSizeButtons[i];
	}
	for (int i = 0; i < this->fruitQuantityButtonsNumber; i++) {
		delete this->fruitQuantityButtons[i];
	}
	for (int i = 0; i < this->snakeLengtheningButtonsNumber; i++) {
		delete this->snakeLengtheningButtons[i];
	}
	for (sf::Text* textPtr : this->ttModeInfoBackgroundTexts1) {
		delete textPtr;
	}
	ttModeInfoBackgroundTexts1.clear();
}

void Menu::initVariables(int gameMusicTracksNumber) {
	this->setCurrentPage(MenuPage::Main);

	this->musicButtonsNumber = gameMusicTracksNumber;

	this->menuBoardRows = 15;
	this->menuBoardColumns = 13;

	this->customizationTabColor1 = sf::Color(60, 30, 100);
	this->customizationTabColor2 = sf::Color(80, 40, 128);
}

void Menu::initAssetsAndSprites(float width, float height, sf::Vector2f topLeft, sf::Font* fontPtr,
	sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr,
	sf::Sound* buttonHoverSoundPtr, sf::Sound* buttonClickSoundPtr, sf::Sound* buttonDisabledClickSoundPtr) {

	this->fontPtr = fontPtr;
	this->buttonHoverSoundPtr = buttonHoverSoundPtr;
	this->buttonClickSoundPtr = buttonClickSoundPtr;
	this->buttonDisabledClickSoundPtr = buttonDisabledClickSoundPtr;

	this->unlockedAchievementLabelTexturePtr = unlockedAchievementLabelTexturePtr;
	this->lockedAchievementLabelTexturePtr = lockedAchievementLabelTexturePtr;

	this->renderTexture.create(width, height);
	this->sprite.setTexture(this->renderTexture.getTexture());
	this->sprite.setPosition(topLeft);

	Board* boardObjectForBackgroundTexture = new Board(width, height, this->menuBoardRows, this->menuBoardColumns);
	this->loadSuccessful = this->loadSuccessful && boardObjectForBackgroundTexture->getLoadSuccessful()
		&& this->playButtonTexture.loadFromFile("Textures/buttonPlay.png")
		&& this->settingsButtonTexture.loadFromFile("Textures/buttonSettings.png")
		&& this->gamemodesButtonTexture.loadFromFile("Textures/buttonGamemodes.png")
		&& this->customizeButtonTexture.loadFromFile("Textures/buttonCustomize.png")
		&& this->achievementsButtonTexture.loadFromFile("Textures/buttonAchievements.png")
		&& this->infoButtonTexture.loadFromFile("Textures/buttonInfo.png")
		&& this->backButtonTexture.loadFromFile("Textures/buttonBack.png")
		&& this->snakeSpeedButtonTexture.loadFromFile("Textures/buttonSpeed.png")
		&& this->boardSizeButtonTexture.loadFromFile("Textures/buttonBoardSize.png")
		&& this->fruitQuantityButtonTexture.loadFromFile("Textures/buttonFruitQuantity.png")
		&& this->snakeLengtheningButtonTexture.loadFromFile("Textures/buttonLengthening.png")
		&& this->tabButtonTexture.loadFromFile("Textures/buttonTab.png")
		&& this->colorButtonTexture.loadFromFile("Textures/buttonColor.png")
		&& this->rainbowColorButtonTexture.loadFromFile("Textures/buttonColorRainbow.png")
		&& this->randomColorButtonTexture.loadFromFile("Textures/buttonColorRandom.png")
		&& this->emergencyColorButtonTexture.loadFromFile("Textures/buttonColorEmergency.png")
		&& this->musicButtonTexture.loadFromFile("Textures/buttonMusic.png")
		&& this->colorOverlayTexture.loadFromFile("Textures/buttonColor_overlay.png")
		&& this->alphaOverlayTexture.loadFromFile("Textures/buttonGradientAlpha.png")
		&& this->gradientOffOverlayTexture.loadFromFile("Textures/buttonGradientOff_overlay.png")
		&& this->gamemodeButtonTexture.loadFromFile("Textures/buttonGamemode.png")
		&& this->disabledIconTexture.loadFromFile("Textures/lockedIcon.png")
		&& this->lockedAchievementButtonTexture.loadFromFile("Textures/buttonAchievementLocked.png")
		&& this->unlockedAchievementButtonTexture.loadFromFile("Textures/buttonAchievementUnlocked.png")
		&& this->gameInfoPageOptionTexture.loadFromFile("Textures/buttonGameInfoOption.png")
		&& this->activeButtonIndicator.loadFromFile("Textures/activeButtonIndicator.png")
		&& this->activeButtonIndicator2.loadFromFile("Textures/activeButtonIndicator2.png")
		&& this->ttModeInfoPageButtonTexture.loadFromFile("Textures/pageButton.png")
		&& this->dataEraseIconTexture.loadFromFile("Textures/dataEraseIcon.png")
		&& this->vanTexture.loadFromFile("Textures/van.png")
		&& this->candyTexture.loadFromFile("Textures/candy.png")
		&& this->brakeSoundBuffer.loadFromFile("Audio/brake.mp3")
		&& this->candySoundBuffer.loadFromFile("Audio/candy.mp3");


	this->tileWidth = boardObjectForBackgroundTexture->tileWidth;
	this->tileHeight = boardObjectForBackgroundTexture->tileHeight;

	Board* boardObjectForCustomizationTabTexture = new Board(width, this->tileHeight * 8.f, 8, this->menuBoardColumns,
		this->customizationTabColor1, this->customizationTabColor2);
	this->loadSuccessful = this->loadSuccessful && boardObjectForCustomizationTabTexture->getLoadSuccessful();

	if (this->loadSuccessful) {
		for (int i = 0; i < this->ttModeInfoPicturesNumber; i++) {
			this->loadSuccessful = this->loadSuccessful && this->ttModeInfoPictures[i].texture.loadFromFile("Textures/ttModeInstruction" + std::to_string(i + 1) + ".png");
			if (!this->loadSuccessful) break;
			this->ttModeInfoPictures[i].sprite.setTexture(this->ttModeInfoPictures[i].texture);
			sf::FloatRect localBounds = this->ttModeInfoPictures[i].sprite.getLocalBounds();
			this->ttModeInfoPictures[i].sprite.setScale(this->tileWidth * 13.f / localBounds.width, this->tileHeight * 2.f / localBounds.height);
			this->ttModeInfoPictures[i].sprite.setOrigin(localBounds.width / 2.f, 0.f);
		}
	}

	// Textures creation and assigning to sprites
	if (this->loadSuccessful) {
		// Background texture and sprite
		sf::RenderTexture tempRenderTexture;
		tempRenderTexture.create(boardObjectForBackgroundTexture->getSize().x, boardObjectForBackgroundTexture->getSize().y);
		boardObjectForBackgroundTexture->draw(tempRenderTexture);
		tempRenderTexture.display();
		this->backgroundTexture.loadFromImage(tempRenderTexture.getTexture().copyToImage());
		this->backgroundSprite.setTexture(this->backgroundTexture);
		sf::FloatRect backgroundSpriteSize = this->backgroundSprite.getLocalBounds();
		this->backgroundSprite.setScale(width / backgroundSpriteSize.width, height / backgroundSpriteSize.height);
		this->backgroundSprite.setPosition(0, 0);

		// Customization tab texture and sprite
		sf::RenderTexture tempRenderTexture2;
		tempRenderTexture2.create(boardObjectForCustomizationTabTexture->getSize().x, boardObjectForCustomizationTabTexture->getSize().y);
		boardObjectForCustomizationTabTexture->draw(tempRenderTexture2);
		tempRenderTexture2.display();
		this->customizationTabTexture.loadFromImage(tempRenderTexture2.getTexture().copyToImage());
		this->customizationTabSprite.setTexture(this->customizationTabTexture);
		sf::FloatRect customizationTabSpriteSize = this->customizationTabSprite.getLocalBounds();
		this->customizationTabSprite.setScale(boardObjectForCustomizationTabTexture->getSize().x / customizationTabSpriteSize.width,
			boardObjectForCustomizationTabTexture->getSize().y / customizationTabSpriteSize.height);
		this->customizationTabSprite.setPosition(0, this->tileHeight * 3.f);

		this->dataEraseIconSprite.setTexture(this->dataEraseIconTexture);
		sf::FloatRect iconTextureSize = this->dataEraseIconSprite.getLocalBounds();
		this->dataEraseIconSprite.setScale(this->tileWidth * 7.f / iconTextureSize.width, this->tileHeight * 7.f / iconTextureSize.height);
		this->dataEraseIconSprite.setOrigin(iconTextureSize.width / 2.f, iconTextureSize.height / 2.f);
		this->dataEraseIconSprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 8.5f);

		// Candy van
		this->vanSprite.setTexture(this->vanTexture);
		sf::FloatRect vanTextureSize = this->vanSprite.getLocalBounds();
		this->vanSprite.setScale(this->tileWidth * 13.f / vanTextureSize.width,
			this->tileHeight * 4.f / vanTextureSize.height);
		this->vanSprite.setPosition(0, this->backgroundSprite.getGlobalBounds().width);

		this->candySprite.setTexture(this->candyTexture);
		sf::FloatRect candyTextureSize = this->candySprite.getLocalBounds();
		this->candySprite.setScale(this->tileWidth * 5.f / candyTextureSize.width,
			this->tileHeight * 5.f / candyTextureSize.height);
		this->candySprite.setOrigin(this->candySprite.getLocalBounds().width / 2.f, this->candySprite.getLocalBounds().height / 2.f);
		this->candySprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 2.f);
		this->defaultCandySpriteScale = this->candySprite.getScale();

		this->brakeSound.setBuffer(this->brakeSoundBuffer);
		this->brakeSound.setVolume(DEFAULT_VOLUME);

		this->candySound.setBuffer(this->candySoundBuffer);
		this->candySound.setVolume(DEFAULT_VOLUME);
	}
	delete boardObjectForBackgroundTexture;
	delete boardObjectForCustomizationTabTexture;

	
}

void Menu::initMenuPages()
{
	this->initMainPage();
	this->initGamemodesPage();
	this->initSettingsPage();
	this->initCustomizePage();
	this->initAchievementsPage();
	this->initGameInfoPage();
	this->initCreditsPage();
	this->initTtModeInfoPages();
	this->initDataErasePage();

	this->backButtonPtr = new Button(&this->backButtonTexture, this->sprite.getGlobalBounds().width / 2, this->tileHeight * 13.5f,
		this->tileWidth * 5.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->backButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->backButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->backButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->backButtonPtr->setText(this->fontPtr, "BACK",
		sf::Vector2f(this->backButtonPtr->sprite.getGlobalBounds().width / 2, this->backButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		40.f, sf::Color::White, OriginPoint::Center);
}

void Menu::initMainPage()
{
	float buttonMovementX = this->tileWidth;
	float buttonTextMovementX = 50.f;
	float buttonMovementTime = 0.2f;

	float xPosition1 = -buttonMovementX;
	float xPosition2 = this->tileWidth * 4.f;
	float width = this->tileWidth * 9.f + buttonMovementX;
	float height = this->tileHeight;
	float textSize = 40.f;

	// TEXT
	this->titleText.setFont(*this->fontPtr);
	this->titleText.setString("SNAKE");
	this->titleText.setCharacterSize(120.f);
	this->titleText.setFillColor(this->headerTextColor);
	this->titleText.setOrigin(this->titleText.getLocalBounds().width / 2, this->titleText.getLocalBounds().height / 2 + 20.f);
	this->titleText.setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 2.f);

	
	// BUTTONS
	// Play button
	this->playButtonPtr = new Button(&this->playButtonTexture, xPosition1, this->tileHeight * 4.f,
		width, height, OriginPoint::Topleft);
	this->playButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->playButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->playButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->playButtonPtr->setText(this->fontPtr, "PLAY",
		sf::Vector2f((this->playButtonPtr->sprite.getGlobalBounds().width + buttonMovementX) / 2, this->playButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize, sf::Color::White, OriginPoint::Center);
	this->playButtonPtr->setHoverMovement(buttonMovementX, 0.f, buttonMovementTime, buttonTextMovementX, 0.f);

	// Gamemodes button
	this->gamemodesButtonPtr = new Button(&this->gamemodesButtonTexture, xPosition2, this->tileHeight * 6.f,
		width, height, OriginPoint::Topleft);
	this->gamemodesButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->gamemodesButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->gamemodesButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->gamemodesButtonPtr->setText(this->fontPtr, "GAMEMODES",
		sf::Vector2f((this->gamemodesButtonPtr->sprite.getGlobalBounds().width - buttonMovementX) / 2, this->gamemodesButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize, sf::Color::White, OriginPoint::Center);
	this->gamemodesButtonPtr->setHoverMovement(-buttonMovementX, 0.f, buttonMovementTime, -buttonTextMovementX, 0.f);

	// Settings button
	this->settingsButtonPtr = new Button(&this->settingsButtonTexture, xPosition1, this->tileHeight * 8.f,
		width, height, OriginPoint::Topleft);
	this->settingsButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->settingsButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->settingsButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->settingsButtonPtr->setText(this->fontPtr, "SETTINGS",
		sf::Vector2f((this->settingsButtonPtr->sprite.getGlobalBounds().width + buttonMovementX) / 2, this->settingsButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize, sf::Color::White, OriginPoint::Center);
	this->settingsButtonPtr->setHoverMovement(buttonMovementX, 0.f, buttonMovementTime, buttonTextMovementX, 0.f);

	// Customize button
	this->customizeButtonPtr = new Button(&this->customizeButtonTexture, xPosition2, this->tileHeight * 10.f,
		width, height, OriginPoint::Topleft);
	this->customizeButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->customizeButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->customizeButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->customizeButtonPtr->setText(this->fontPtr, "CUSTOMIZE",
		sf::Vector2f((this->customizeButtonPtr->sprite.getGlobalBounds().width - buttonMovementX) / 2, this->customizeButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize, sf::Color::White, OriginPoint::Center);
	this->customizeButtonPtr->setHoverMovement(-buttonMovementX, 0.f, buttonMovementTime, -buttonTextMovementX, 0.f);

	// Achievements button
	this->achievementsButtonPtr = new Button(&this->achievementsButtonTexture, xPosition1, this->tileHeight * 12.f,
		width, height, OriginPoint::Topleft);
	this->achievementsButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->achievementsButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->achievementsButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->achievementsButtonPtr->setText(this->fontPtr, "ACHIEVEMENTS",
		sf::Vector2f((this->achievementsButtonPtr->sprite.getGlobalBounds().width + buttonMovementX) / 2, this->achievementsButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize - 3.f, sf::Color::White, OriginPoint::Center);
	this->achievementsButtonPtr->setHoverMovement(buttonMovementX, 0.f, buttonMovementTime, buttonTextMovementX - 20.f, 0.f);

	// Leave button
	this->infoButtonPtr = new Button(&this->infoButtonTexture, xPosition2, this->tileHeight * 14.f,
		width, height, OriginPoint::Topleft);
	this->infoButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->infoButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->infoButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->infoButtonPtr->setText(this->fontPtr, "INFO",
		sf::Vector2f((this->infoButtonPtr->sprite.getGlobalBounds().width - buttonMovementX) / 2, this->infoButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		textSize, sf::Color::White, OriginPoint::Center);
	this->infoButtonPtr->setHoverMovement(-buttonMovementX, 0.f, buttonMovementTime, -buttonTextMovementX, 0.f);


	// Secret candy van
	this->titleButtonPtr = new Button(&this->backButtonTexture, this->tileWidth * 2.f, this->tileHeight * 1.f,
		this->tileWidth * 9.f, this->tileHeight * 2.f, OriginPoint::Topleft);
	this->titleButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->titleButtonPtr->setIsDisabled(false);

	this->vanButtonPtr = new Button(&this->backButtonTexture, this->tileWidth * 2.f, 0.f,
		this->tileWidth * 9.f, this->tileHeight * 4.f, OriginPoint::Topleft);
	this->vanButtonPtr->setIsDisabled(true);

}

void Menu::initGamemodesPage()
{
	// TEXT
	this->initPageHeaderText(&this->gamemodesHeaderText, "GAMEMODES");

	this->gamemodePageInfoText.setFont(*this->fontPtr);
	this->gamemodePageInfoText.setString("Info:");
	this->gamemodePageInfoText.setCharacterSize(40.f);
	this->gamemodePageInfoText.setFillColor(this->gamemodeDescriptionTextColor);
	this->gamemodePageInfoText.setOrigin(this->gamemodePageInfoText.getLocalBounds().width / 2, this->gamemodePageInfoText.getLocalBounds().height / 2 + 10.f);
	this->gamemodePageInfoText.setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 11.f + 5.f);

	this->gamemodeDescriptionText.setFont(*this->fontPtr);
	this->gamemodeDescriptionText.setCharacterSize(this->gamemodeDescriptionTextSize);
	this->gamemodeDescriptionText.setFillColor(this->gamemodeDescriptionTextColor);
	this->gamemodeDescriptionText.setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 11.75f + 5.f);

	this->gamemodeDescriptionText2.setFont(*this->fontPtr);
	this->gamemodeDescriptionText2.setCharacterSize(this->gamemodeDescriptionTextSize);
	this->gamemodeDescriptionText2.setFillColor(this->gamemodeDescriptionTextColor);
	this->gamemodeDescriptionText2.setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 12.25f + 5.f);

	// BUTTONS
	this->gamemodeButtons = new Button*[this->gamemodeButtonsNumber];
	for (int i = 0; i < this->gamemodeButtonsNumber; i++) {
		float posX = this->tileWidth * 4.f;
		float posY = this->tileHeight * 3.f;
		if (i > 0) {
			if (i % 2 == 1)
				posX -= this->tileWidth * 3.f;
			else
				posX += this->tileWidth * 3.f;
			posY += this->tileHeight * 2.f * (floor(((float)i - 1.f) / 2.f) + 1);
		}
		this->gamemodeButtons[i] = new Button(&this->gamemodeButtonTexture, posX, posY,
			this->tileWidth * 5.f, this->tileHeight, OriginPoint::Topleft);
		this->gamemodeButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->gamemodeButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->gamemodeButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));
		this->gamemodeButtons[i]->setActiveIcon(&this->activeButtonIndicator2);
		float textSize;
		switch (i) {
		case 4: textSize = 22.f; break;  // Moving fruits
		case 6: textSize = 25.f; break;  // Time Travel
		default: textSize = 30.f; break; }
		this->gamemodeButtons[i]->setText(this->fontPtr, this->gamemodeNames[i],
			sf::Vector2f(this->gamemodeButtons[i]->sprite.getGlobalBounds().width / 2, this->gamemodeButtons[i]->sprite.getGlobalBounds().height / 2 - (textSize / 6.f)),
			textSize, this->optionButtonsTextColor, OriginPoint::Center);
	}

	if (this->dataManager.savedGamemodeIndex < this->gamemodeButtonsNumber) {
		this->gamemodeButtons[this->dataManager.savedGamemodeIndex]->setIsActive(true);
		this->updateGamemodeDescriptionText(this->dataManager.savedGamemodeIndex);
	}
}

void Menu::initSettingsPage()
{
	// TEXT
	this->initPageHeaderText(&this->settingsHeaderText, "SETTINGS");

	this->snakeSpeedText.setFont(*this->fontPtr);
	this->snakeSpeedText.setString("Snake\nspeed:");
	this->snakeSpeedText.setCharacterSize(40.f);
	this->snakeSpeedText.setFillColor(this->optionNameTextColor);
	this->snakeSpeedText.setOrigin(this->snakeSpeedText.getLocalBounds().width / 2, this->snakeSpeedText.getLocalBounds().height / 2 + 10.f);
	this->snakeSpeedText.setPosition(this->tileWidth * 2.f, this->tileHeight * 4.f);

	this->fruitAmountText.setFont(*this->fontPtr);
	this->fruitAmountText.setString("Fruits:");
	this->fruitAmountText.setCharacterSize(40.f);
	this->fruitAmountText.setFillColor(this->optionNameTextColor);
	this->fruitAmountText.setOrigin(this->fruitAmountText.getLocalBounds().width / 2, this->fruitAmountText.getLocalBounds().height / 2 + 10.f);
	this->fruitAmountText.setPosition(this->tileWidth * 2.f, this->tileHeight * 6.5f);

	this->boardSizeText.setFont(*this->fontPtr);
	this->boardSizeText.setString("Board\n size:");
	this->boardSizeText.setCharacterSize(40.f);
	this->boardSizeText.setFillColor(this->optionNameTextColor);
	this->boardSizeText.setOrigin(this->boardSizeText.getLocalBounds().width / 2, this->boardSizeText.getLocalBounds().height / 2 + 10.f);
	this->boardSizeText.setPosition(this->tileWidth * 2.f, this->tileHeight * 9.f);

	this->snakeLengthingText.setFont(*this->fontPtr);
	this->snakeLengthingText.setString(" Snake\nLengthening:");
	this->snakeLengthingText.setCharacterSize(35.f);
	this->snakeLengthingText.setFillColor(this->optionNameTextColor);
	this->snakeLengthingText.setOrigin(this->snakeLengthingText.getLocalBounds().width / 2, this->snakeLengthingText.getLocalBounds().height / 2 + 10.f);
	this->snakeLengthingText.setPosition(this->tileWidth * 3.f, this->tileHeight * 11.5f);


	// BUTTONS
	float buttonTextSize = 30.f;
	// Snake speed
	this->snakeSpeedButtons = new Button*[this->snakeSpeedButtonsNumber];
	for (int i = 0; i < this->snakeSpeedButtonsNumber; i++) {
		float posY = (i % 2 == 0) ? this->tileHeight * 4.f : this->tileHeight * 3.f;
		this->snakeSpeedButtons[i] = new Button(&this->snakeSpeedButtonTexture, this->tileWidth * (4.f + i), posY,
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->snakeSpeedButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->snakeSpeedButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->snakeSpeedButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));
		this->snakeSpeedButtons[i]->setText(this->fontPtr, std::to_string(i + 1),
			sf::Vector2f(this->snakeSpeedButtons[i]->sprite.getGlobalBounds().width / 2, this->snakeSpeedButtons[i]->sprite.getGlobalBounds().height / 2 - 5.f),
			buttonTextSize, this->optionButtonsTextColor, OriginPoint::Center);
		//this->snakeSpeedButtons[i]->setActiveColor(activeColor);
		this->snakeSpeedButtons[i]->setActiveIcon(&this->activeButtonIndicator);
	}
	if (this->dataManager.savedSnakeSpeedIndex < this->snakeSpeedButtonsNumber)
		this->snakeSpeedButtons[this->dataManager.savedSnakeSpeedIndex]->setIsActive(true);

	// Fruit amount
	this->fruitQuantityButtons = new Button * [this->fruitQuantityButtonsNumber];
	for (int i = 0; i < this->fruitQuantityButtonsNumber; i++) {
		float posX;
		if (this->fruitQuantities[i] == 1) posX = this->tileWidth * 4.f;
		else if (this->fruitQuantities[i] <= 3) posX = this->tileWidth * (5.f + i);
		else if (this->fruitQuantities[i] <= 5) posX = this->tileWidth * (6.f + i);
		else posX = this->tileWidth * (7.f + i);

		this->fruitQuantityButtons[i] = new Button(&this->fruitQuantityButtonTexture, posX, this->tileHeight * 6.f,
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->fruitQuantityButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->fruitQuantityButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->fruitQuantityButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));
		this->fruitQuantityButtons[i]->setText(this->fontPtr, std::to_string(this->fruitQuantities[i]),
			sf::Vector2f(this->fruitQuantityButtons[i]->sprite.getGlobalBounds().width / 2, this->fruitQuantityButtons[i]->sprite.getGlobalBounds().height / 2 - 5.f),
			buttonTextSize, this->optionButtonsTextColor, OriginPoint::Center);
		//this->fruitQuantityButtons[i]->setActiveColor(activeColor);
		this->fruitQuantityButtons[i]->setActiveIcon(&this->activeButtonIndicator);
	}
	if (this->dataManager.savedFruitQuantityIndex < this->fruitQuantityButtonsNumber)
		this->fruitQuantityButtons[this->dataManager.savedFruitQuantityIndex]->setIsActive(true);

	// Board size
	this->boardSizeButtons = new Button * [this->boardSizeButtonsNumber];
	for (int i = 0; i < this->boardSizeButtonsNumber; i++) {
		float posY = (i % 2 == 0) ? this->tileHeight * 9.f : this->tileHeight * 8.f;
		this->boardSizeButtons[i] = new Button(&this->boardSizeButtonTexture, this->tileWidth * (5.f + i), posY,
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->boardSizeButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->boardSizeButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->boardSizeButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));
		this->boardSizeButtons[i]->setText(this->fontPtr, std::to_string(this->boardSizes[i]),
			sf::Vector2f(this->boardSizeButtons[i]->sprite.getGlobalBounds().width / 2, this->boardSizeButtons[i]->sprite.getGlobalBounds().height / 2 - 5.f),
			buttonTextSize, this->optionButtonsTextColor, OriginPoint::Center);
		//this->boardSizeButtons[i]->setthis->optionButtonsActiveColor(activeColor);
		this->boardSizeButtons[i]->setActiveIcon(&this->activeButtonIndicator);
	}
	if (this->dataManager.savedBoardSizeIndex < this->boardSizeButtonsNumber)
		this->boardSizeButtons[this->dataManager.savedBoardSizeIndex]->setIsActive(true);

	// Snake lengthening
	this->snakeLengtheningButtons = new Button * [this->snakeLengtheningButtonsNumber];
	for (int i = 0; i < this->snakeLengtheningButtonsNumber; i++) {
		this->snakeLengtheningButtons[i] = new Button(&this->snakeLengtheningButtonTexture, this->tileWidth * (6.f + 2*i), this->tileHeight * 11.f,
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->snakeLengtheningButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->snakeLengtheningButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->snakeLengtheningButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));
		this->snakeLengtheningButtons[i]->setText(this->fontPtr, std::to_string(this->snakeLengtheningValues[i]),
			sf::Vector2f(this->snakeLengtheningButtons[i]->sprite.getGlobalBounds().width / 2, this->snakeLengtheningButtons[i]->sprite.getGlobalBounds().height / 2 - 5.f),
			buttonTextSize, this->optionButtonsTextColor, OriginPoint::Center);
		//this->snakeLengtheningButtons[i]->setActiveColor(activeColor);
		this->snakeLengtheningButtons[i]->setActiveIcon(&this->activeButtonIndicator);
	}
	if (this->dataManager.savedSnakeLengtheningIndex < this->snakeLengtheningButtonsNumber)
		this->snakeLengtheningButtons[this->dataManager.savedSnakeLengtheningIndex]->setIsActive(true);
}

void Menu::initCustomizePage()
{
	// TEXT
	this->initPageHeaderText(&this->customizeHeaderText, "CUSTOMIZATION");

	this->customizeRightClickText.setFont(*this->fontPtr);
	this->customizeRightClickText.setString("(Right-click a button for more info)");
	this->customizeRightClickText.setCharacterSize(25.f);
	this->customizeRightClickText.setFillColor(sf::Color::White);
	this->customizeRightClickText.setOrigin(this->customizeRightClickText.getLocalBounds().width / 2, this->customizeRightClickText.getLocalBounds().height / 2 + 5.f);
	this->customizeRightClickText.setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 2.5f);

	this->snakeColorText.setFont(*this->fontPtr);
	this->snakeColorText.setString("Snake\ncolor:");
	this->snakeColorText.setCharacterSize(40.f);
	this->snakeColorText.setFillColor(this->optionNameTextColor);
	this->snakeColorText.setOrigin(this->snakeColorText.getLocalBounds().width / 2, this->snakeColorText.getLocalBounds().height / 2 + 10.f);
	this->snakeColorText.setPosition(this->tileWidth * 2.f, this->tileHeight * 4.f);

	this->gradientColorText.setFont(*this->fontPtr);
	this->gradientColorText.setString("Gradient\ncolor:");
	this->gradientColorText.setCharacterSize(40.f);
	this->gradientColorText.setFillColor(this->optionNameTextColor);
	this->gradientColorText.setOrigin(this->gradientColorText.getLocalBounds().width / 2, this->gradientColorText.getLocalBounds().height / 2 + 10.f);
	this->gradientColorText.setPosition(this->tileWidth * 2.f, this->tileHeight * 7.f);

	this->tileColorText.setFont(*this->fontPtr);
	this->tileColorText.setString("Ground\ncolor:");
	this->tileColorText.setCharacterSize(40.f);
	this->tileColorText.setFillColor(this->optionNameTextColor);
	this->tileColorText.setOrigin(this->tileColorText.getLocalBounds().width / 2, this->tileColorText.getLocalBounds().height / 2 + 10.f);
	this->tileColorText.setPosition(this->tileWidth * 2.f, this->tileHeight * 10.f);

	this->fruitTextureText.setFont(*this->fontPtr);
	this->fruitTextureText.setString("Fruits:");
	this->fruitTextureText.setCharacterSize(40.f);
	this->fruitTextureText.setFillColor(this->optionNameTextColor);
	this->fruitTextureText.setOrigin(this->fruitTextureText.getLocalBounds().width / 2, this->fruitTextureText.getLocalBounds().height / 2 + 10.f);
	this->fruitTextureText.setPosition(this->tileWidth * 2.f, this->tileHeight * 5.f);

	this->musicText.setFont(*this->fontPtr);
	this->musicText.setString("Game\nmusic:");
	this->musicText.setCharacterSize(40.f);
	this->musicText.setFillColor(this->optionNameTextColor);
	this->musicText.setOrigin(this->musicText.getLocalBounds().width / 2, this->musicText.getLocalBounds().height / 2 + 10.f);
	this->musicText.setPosition(this->tileWidth * 2.f, this->tileHeight * 9.f);


	// BUTTONS
	// Snake color
	//this->snakeColorButtons = new Button* [this->snakeColorsNumber];
	for (int i = 0; i < this->snakeColorsNumber; i++) {
		if (i < 9)
			this->snakeColors[i].buttonPtr = new Button(&this->colorButtonTexture, this->tileWidth * (4.f + i), this->tileHeight * 3.f,
				this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		else if (i < 12)
			this->snakeColors[i].buttonPtr = new Button(&this->colorButtonTexture, this->tileWidth * (4.f + (i - 9)), this->tileHeight * 4.f,
				this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		else {
			sf::Texture* buttonTexturePtr;
			switch (i) {
			default:
			case 12:
				buttonTexturePtr = &this->randomColorButtonTexture;
				break;
			case 13:
				buttonTexturePtr = &this->emergencyColorButtonTexture;
				break;
			case 14:
				buttonTexturePtr = &this->rainbowColorButtonTexture;
				break;
			}
			this->snakeColors[i].buttonPtr = new Button(buttonTexturePtr, this->tileWidth * (5.f + (i - 9)), this->tileHeight * 4.f,
				this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		}
		this->snakeColors[i].buttonPtr->setHoverSound(this->buttonHoverSoundPtr);
		this->snakeColors[i].buttonPtr->setClickSound(this->buttonClickSoundPtr);
		this->snakeColors[i].buttonPtr->setDisabledClickSound(this->buttonDisabledClickSoundPtr);
		if (i < 12)
			this->snakeColors[i].buttonPtr->setDefaultColor(this->snakeColors[i].color);
		this->snakeColors[i].buttonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
		this->snakeColors[i].buttonPtr->setActiveIcon(&this->activeButtonIndicator);
		this->snakeColors[i].buttonPtr->setDisabledIcon(&this->disabledIconTexture);
	}
	this->getItemById(this->dataManager.savedSnakeColorId, this->snakeColors).buttonPtr->setIsActive(true);

	// Gradient color
	/*this->gradientColorButtons = new Button* [this->gradientColorsNumber];*/
	for (int i = 0; i < this->gradientColorsNumber; i++) {
		float posX = this->tileWidth * (4.f + (i % 9));
		float posY = this->tileHeight * 6.f;
		if (i > 8) posY += this->tileHeight;
		if (i > 11) posX += this->tileWidth;
		this->gradientColors[i].buttonPtr = new Button(&this->colorButtonTexture, posX, posY,
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);

		this->gradientColors[i].buttonPtr->setHoverSound(this->buttonHoverSoundPtr);
		this->gradientColors[i].buttonPtr->setClickSound(this->buttonClickSoundPtr);
		this->gradientColors[i].buttonPtr->setDisabledClickSound(this->buttonDisabledClickSoundPtr);
		if (this->gradientColors[i].id < 213)  // Standard colors
			this->gradientColors[i].buttonPtr->setDefaultColor(this->gradientColors[i].color);
		this->gradientColors[i].buttonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
		this->gradientColors[i].buttonPtr->setActiveIcon(&this->activeButtonIndicator);
		this->gradientColors[i].buttonPtr->setDisabledIcon(&this->disabledIconTexture);
	}
	// Special gradients
	// Default gradient
	this->getItemById(213, this->gradientColors).buttonPtr->setDefaultColor(this->gameSettingsList.snakeColorEnd);
	this->getItemById(213, this->gradientColors).buttonPtr->setOverlay(&this->alphaOverlayTexture, this->gameSettingsList.snakeColorBegin);
	// No gradient
	this->getItemById(214, this->gradientColors).buttonPtr->setDefaultColor(this->gameSettingsList.snakeColorBegin);
	this->getItemById(214, this->gradientColors).buttonPtr->setOverlay(&this->gradientOffOverlayTexture);
	// Semi-transparent gradient
	this->getItemById(215, this->gradientColors).buttonPtr->setDefaultTexture(&this->alphaOverlayTexture);
	this->getItemById(215, this->gradientColors).buttonPtr->setDefaultColor(this->gameSettingsList.snakeColorBegin);
	// Opposite color gradient
	this->getItemById(216, this->gradientColors).buttonPtr->setDefaultColor(sf::Color(255 - this->gameSettingsList.snakeColorBegin.r,
		255 - this->gameSettingsList.snakeColorBegin.g, 255 - this->gameSettingsList.snakeColorBegin.b));
	this->getItemById(216, this->gradientColors).buttonPtr->setOverlay(&this->alphaOverlayTexture, this->gameSettingsList.snakeColorBegin);
	// Randomized gradient
	this->getItemById(217, this->gradientColors).buttonPtr->setDefaultTexture(&this->randomColorButtonTexture);

	// Activate the correct button
	this->getItemById(this->dataManager.savedGradientColorId, this->gradientColors).buttonPtr->setIsActive(true);

	// Tile colors
	for (int i = 0; i < this->tileColorButtonsNumber; i++) {
		if (i < 9) {
			this->tileColors[i].buttonPtr = new Button(&this->colorButtonTexture, this->tileWidth * (4.f + i), this->tileHeight * 9.f,
				this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		}
		else {
			this->tileColors[i].buttonPtr = new Button(&this->colorButtonTexture, this->tileWidth * (4.f + (i - 9)), this->tileHeight * 10.f,
				this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		}
		this->tileColors[i].buttonPtr->setDefaultColor(this->tileColors[i].color1);
		this->tileColors[i].buttonPtr->setOverlay(&this->colorOverlayTexture, this->tileColors[i].color2);
		this->tileColors[i].buttonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
		this->tileColors[i].buttonPtr->setActiveIcon(&this->activeButtonIndicator);
		this->tileColors[i].buttonPtr->setDisabledIcon(&this->disabledIconTexture);
		this->tileColors[i].buttonPtr->setHoverSound(this->buttonHoverSoundPtr);
		this->tileColors[i].buttonPtr->setClickSound(this->buttonClickSoundPtr);
		this->tileColors[i].buttonPtr->setDisabledClickSound(this->buttonDisabledClickSoundPtr);
	}
	this->getItemById(this->dataManager.savedTileColorId, this->tileColors).buttonPtr->setIsActive(true);

	// Fruit texture buttons
	std::vector<sf::Texture*> fruitTexturePtrs = Fruit::getTextures();
	for (int i = 0; i < this->fruitTextureButtonsNumber; i++) {
		this->fruitTextures[i].externalArrayIndex = i;
		this->fruitTextures[i].buttonPtr = new Button(fruitTexturePtrs[this->fruitTextures[i].externalArrayIndex], this->tileWidth * (4.f + i), this->tileHeight * (i % 2 == 0 ? 5.f : 4.f),
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->fruitTextures[i].buttonPtr->setHoverSound(this->buttonHoverSoundPtr);
		this->fruitTextures[i].buttonPtr->setClickSound(this->buttonClickSoundPtr);
		this->fruitTextures[i].buttonPtr->setDisabledClickSound(this->buttonDisabledClickSoundPtr);
		this->fruitTextures[i].buttonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
		this->fruitTextures[i].buttonPtr->setActiveIcon(&this->activeButtonIndicator);
		this->fruitTextures[i].buttonPtr->setDisabledIcon(&this->disabledIconTexture);
	}
	this->getItemById(this->dataManager.savedFruitTextureId, this->fruitTextures).buttonPtr->setIsActive(true);

	// Music buttons
	for (int i = 0; i < this->musicButtonsNumber; i++) {
		this->musicTracks[i].externalArrayIndex = i;
		this->musicTracks[i].buttonPtr = new Button(&this->musicButtonTexture, this->tileWidth * (4.f + 2 * i), this->tileHeight * (i % 2 == 0 ? 9.f : 8.f),
			this->tileWidth, this->tileHeight, OriginPoint::Topleft);
		this->musicTracks[i].buttonPtr->setHoverSound(this->buttonHoverSoundPtr);
		this->musicTracks[i].buttonPtr->setClickSound(this->buttonClickSoundPtr);
		this->musicTracks[i].buttonPtr->setDisabledClickSound(this->buttonDisabledClickSoundPtr);
		this->musicTracks[i].buttonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
		this->musicTracks[i].buttonPtr->setActiveIcon(&this->activeButtonIndicator);
		this->musicTracks[i].buttonPtr->setDisabledIcon(&this->disabledIconTexture);
		this->musicTracks[i].buttonPtr->setText(this->fontPtr, std::to_string(i + 1),
			sf::Vector2f(this->musicTracks[i].buttonPtr->sprite.getGlobalBounds().width / 2, this->musicTracks[i].buttonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
			40.f, sf::Color(0, 0, 255, 255), OriginPoint::Center);
	}
	this->getItemById(this->dataManager.savedMusicTrackId, this->musicTracks).buttonPtr->setIsActive(true);

	// Tab buttons
	this->customizationTab1ButtonPtr = new Button(&this->tabButtonTexture, this->tileWidth * 0.875f, this->tileHeight * 11.f,
		this->tileWidth * 5.25f, this->tileHeight * 1.125f, OriginPoint::Topleft);
	this->customizationTab1ButtonPtr->setDefaultColor(sf::Color(150, 150, 150));
	this->customizationTab1ButtonPtr->setActiveColor(this->customizationTabColor1);
	this->customizationTab1ButtonPtr->setText(this->fontPtr, "Colors",
		sf::Vector2f(this->customizationTab1ButtonPtr->sprite.getGlobalBounds().width / 2, this->customizationTab1ButtonPtr->sprite.getGlobalBounds().height / 2 - 8.f),
		40.f, sf::Color::White, OriginPoint::Center);
	this->customizationTab1ButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->customizationTab1ButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->customizationTab1ButtonPtr->setClickSound(this->buttonClickSoundPtr);

	this->customizationTab2ButtonPtr = new Button(&this->tabButtonTexture, this->tileWidth * 6.875f, this->tileHeight * 11.f,
		this->tileWidth * 5.25f, this->tileHeight * 1.125f, OriginPoint::Topleft);
	this->customizationTab2ButtonPtr->setDefaultColor(sf::Color(150, 150, 150));
	this->customizationTab2ButtonPtr->setActiveColor(this->customizationTabColor1);
	this->customizationTab2ButtonPtr->setText(this->fontPtr, "Fruits/music",
		sf::Vector2f(this->customizationTab2ButtonPtr->sprite.getGlobalBounds().width / 2, this->customizationTab2ButtonPtr->sprite.getGlobalBounds().height / 2 - 8.f),
		30.f, sf::Color::White, OriginPoint::Center);
	this->customizationTab2ButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->customizationTab2ButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->customizationTab2ButtonPtr->setClickSound(this->buttonClickSoundPtr);
}

void Menu::initAchievementsPage()
{
	// TEXT
	this->initPageHeaderText(&this->achievementsHeaderText, "ACHIEVEMENTS");

	float achievementNameTextSize = 19.f;
	this->achievementButtons = new Button*[DataManager::achievementsNumber];

	AchievementInfoPage::initStaticElements(this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height, this->tileWidth, this->tileHeight,
		this->fontPtr, this->unlockedAchievementLabelTexturePtr, this->lockedAchievementLabelTexturePtr);

	
	for (int i = 0; i < DataManager::achievementsNumber; i++) {

		const Achievement& currentAchievement = this->dataManager.getAchievementData(i);
		this->achievementInfoPages[i] = new AchievementInfoPage(currentAchievement.name, currentAchievement.description, currentAchievement.extraRequirements);
		this->achievementInfoPages[i]->setAchievementUnlocked(currentAchievement.unlocked);

		float buttonTopleftX = this->tileWidth * (1.f + 3*(i%4));
		if (i % 4 >= 2) buttonTopleftX += this->tileWidth * 1.f;
		float buttonTopleftY = this->tileHeight * 3.f;
		buttonTopleftY += this->tileHeight * 2.f * floor(i / 4.f);
		
		this->achievementButtons[i] = new Button(&this->unlockedAchievementButtonTexture, buttonTopleftX, buttonTopleftY, this->tileWidth, this->tileHeight);
		this->achievementButtons[i]->setHoverSound(this->buttonHoverSoundPtr);
		this->achievementButtons[i]->setClickSound(this->buttonClickSoundPtr);
		this->achievementButtons[i]->setHoverColor(sf::Color(160, 160, 160, 255));

		// Split long texts
		string lowerLabelText = currentAchievement.name;
		if (lowerLabelText.length() > 11) {
			string firstWord = "";
			string secondWord = "";
			bool spaceFound = false;
			for (char character : currentAchievement.name) {
				if (spaceFound) {
					secondWord += character;
				}
				else {
					if (character == ' ') {
						spaceFound = true;
					}
					else {
						firstWord += character;
					}
				}
			}

			if (spaceFound) {
				sf::Text* upperLabel = new sf::Text(firstWord, *this->fontPtr, achievementNameTextSize);
				upperLabel->setFillColor(this->optionNameTextColor);
				upperLabel->setOrigin(upperLabel->getLocalBounds().width / 2, upperLabel->getLocalBounds().height / 2 + 5.f);
				upperLabel->setPosition(buttonTopleftX + this->tileWidth * 0.5f, buttonTopleftY - this->tileHeight * 0.5f);
				this->achievementNameLabels.push_back(upperLabel);

				lowerLabelText = secondWord;
			}
		}
		
		sf::Text* lowerLabel = new sf::Text(lowerLabelText, *this->fontPtr, achievementNameTextSize);
		lowerLabel->setFillColor(this->optionNameTextColor);
		lowerLabel->setOrigin(lowerLabel->getLocalBounds().width / 2, lowerLabel->getLocalBounds().height / 2 + 11.f);
		lowerLabel->setPosition(buttonTopleftX + this->tileWidth * 0.5f, buttonTopleftY - this->tileHeight * 0.1f);
		this->achievementNameLabels.push_back(lowerLabel);
	}
}

void Menu::initGameInfoPage()
{
	// TEXT
	this->initPageHeaderText(&this->gameInfoHeaderText, "GAME INFO");

	// BUTTONS
	this->creditsButtonPtr = new Button(&this->gameInfoPageOptionTexture, this->sprite.getGlobalBounds().width / 2, this->tileHeight * 4.5f,
		this->tileWidth * 7.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->creditsButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->creditsButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->creditsButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->creditsButtonPtr->setText(this->fontPtr, "Credits",
		sf::Vector2f(this->creditsButtonPtr->sprite.getGlobalBounds().width / 2, this->creditsButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		35.f, sf::Color::White, OriginPoint::Center);

	this->ttModeInfoButtonPtr = new Button(&this->gameInfoPageOptionTexture, this->sprite.getGlobalBounds().width / 2, this->tileHeight * 6.5f,
		this->tileWidth * 7.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->ttModeInfoButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->ttModeInfoButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->ttModeInfoButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->ttModeInfoButtonPtr->setText(this->fontPtr, "Time Travel Mode",
		sf::Vector2f(this->ttModeInfoButtonPtr->sprite.getGlobalBounds().width / 2, this->ttModeInfoButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		30.f, sf::Color::White, OriginPoint::Center);

	this->eraseDataButtonPtr = new Button(&this->gameInfoPageOptionTexture, this->sprite.getGlobalBounds().width / 2, this->tileHeight * 8.5f,
		this->tileWidth * 7.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->eraseDataButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->eraseDataButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->eraseDataButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->eraseDataButtonPtr->setText(this->fontPtr, "Erase data",
		sf::Vector2f(this->eraseDataButtonPtr->sprite.getGlobalBounds().width / 2, this->eraseDataButtonPtr->sprite.getGlobalBounds().height / 2 - 5.f),
		35.f, sf::Color::White, OriginPoint::Center);
}

void Menu::initCreditsPage() {
	// TEXT
	this->initPageHeaderText(&this->creditsHeaderText, "CREDITS");

	this->creditsTexts = new sf::Text[this->creditsTextsNumber];
	for (int i = 0; i < this->creditsTextsNumber; i++) {
		this->creditsTexts[i].setFont(*this->fontPtr);
		this->creditsTexts[i].setString(this->creditsTextLines[i]);
		this->creditsTexts[i].setCharacterSize(30.f);
		this->creditsTexts[i].setFillColor(sf::Color::White);
		this->creditsTexts[i].setOrigin(this->creditsTexts[i].getLocalBounds().width / 2, this->creditsTexts[i].getLocalBounds().height / 2 + 5.f);
		this->creditsTexts[i].setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * (3.5f + i));
	}
}

void Menu::initTtModeInfoPages() {

	this->initPageHeaderText(&this->ttModeInfoHeaderText, "TIME T. MODE");

	sf::Color picTextColor(255, 255, 255, 255);
	sf::Color picFutureColor(0, 204, 185);
	sf::Color picPresentColor(100, 255, 100);
	sf::Color picPastColor(204, 0, 0);

	this->ttModeInfoPage1ButtonPtr = new Button(&this->ttModeInfoPageButtonTexture, this->tileWidth * 0.5f, this->tileHeight * 12.5f,
		this->tileWidth * 3.f, this->tileHeight * 2.f, OriginPoint::Topleft);
	this->ttModeInfoPage1ButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->ttModeInfoPage1ButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->ttModeInfoPage1ButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->ttModeInfoPage1ButtonPtr->setText(this->fontPtr, "PAGE 1",
		sf::Vector2f(this->ttModeInfoPage1ButtonPtr->sprite.getGlobalBounds().width / 2, this->ttModeInfoPage1ButtonPtr->sprite.getGlobalBounds().height / 4 - 5.f),
		30.f, sf::Color::White, OriginPoint::Center);
	
	this->ttModeInfoPage2ButtonPtr = new Button(&this->ttModeInfoPageButtonTexture, this->tileWidth * 9.5f, this->tileHeight * 12.5f,
		this->tileWidth * 3.f, this->tileHeight * 2.f, OriginPoint::Topleft);
	this->ttModeInfoPage2ButtonPtr->setHoverSound(this->buttonHoverSoundPtr);
	this->ttModeInfoPage2ButtonPtr->setClickSound(this->buttonClickSoundPtr);
	this->ttModeInfoPage2ButtonPtr->setHoverColor(sf::Color(160, 160, 160, 255));
	this->ttModeInfoPage2ButtonPtr->setText(this->fontPtr, "PAGE 2",
		sf::Vector2f(this->ttModeInfoPage2ButtonPtr->sprite.getGlobalBounds().width / 2, this->ttModeInfoPage2ButtonPtr->sprite.getGlobalBounds().height / 4 - 5.f),
		30.f, sf::Color::White, OriginPoint::Center);

	// PAGE 1
	this->ttModeInfoTexts1 = new sf::Text[this->ttModeInfoTextLines1.size()];
	for (int i = 0; i < this->ttModeInfoTextLines1.size(); i++) {
		this->ttModeInfoTexts1[i] = sf::Text(this->ttModeInfoTextLines1[i], *this->fontPtr, (i == 13 ? 24.7f : 25.f));
		this->ttModeInfoTexts1[i].setFillColor(sf::Color::White);
		this->ttModeInfoTexts1[i].setOrigin(this->ttModeInfoTexts1[i].getLocalBounds().width / 2.f, 0.f);
		this->ttModeInfoTexts1[i].setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * (2.1f + i * 0.45f));
	}

	this->ttModeInfoPictures[0].sprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 4.f);
	this->ttModeInfoPictures[1].sprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 10.f);

	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:05", sf::Vector2f(37.f, 315.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:10", sf::Vector2f(173.f, 370.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:25", sf::Vector2f(754.f, 380.f), picPresentColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("PRESENT", sf::Vector2f(754.f, 350.f), picPresentColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("PAST", sf::Vector2f(410.f, 330.f), picPastColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("-15.0s", sf::Vector2f(463.f, 290.f), sf::Color(255, 187, 153)));

	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:05", sf::Vector2f(37.f, 695.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:10", sf::Vector2f(173.f, 750.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("PRESENT - 0:12", sf::Vector2f(295.f, 770.f), picPresentColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:17", sf::Vector2f(507.f, 750.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("0:25", sf::Vector2f(780.f, 695.f), picTextColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("PAST", sf::Vector2f(154.f, 700.f), picPastColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("FUTURE", sf::Vector2f(650.f, 700.f), picFutureColor));
	this->ttModeInfoBackgroundTexts1.push_back(this->createTtModeInfoBackgroundText("+5.0s", sf::Vector2f(400.f, 665.f), sf::Color(153, 255, 255)));

	// PAGE 2
	this->ttModeInfoTexts2 = new sf::Text[this->ttModeInfoTextLines2.size()];
	for (int i = 0; i < this->ttModeInfoTextLines2.size(); i++) {
		this->ttModeInfoTexts2[i] = sf::Text(this->ttModeInfoTextLines2[i], *this->fontPtr, (i == 13 ? 24.7f : 25.f));
		this->ttModeInfoTexts2[i].setFillColor(sf::Color::White);
		this->ttModeInfoTexts2[i].setOrigin(this->ttModeInfoTexts2[i].getLocalBounds().width / 2.f, 0.f);
		this->ttModeInfoTexts2[i].setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * (2.1f + i * 0.45f));
	}

	this->ttModeInfoPictures[2].sprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 5.2f);
	this->ttModeInfoPictures[3].sprite.setPosition(this->backgroundSprite.getGlobalBounds().width / 2.f, this->tileHeight * 11.f);

	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("0:05", sf::Vector2f(37.f, 390.f), picTextColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("0:14", sf::Vector2f(368.f, 448.f), picTextColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("PRESENT - 0:21", sf::Vector2f(640.f, 460.f), picPresentColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("0:25", sf::Vector2f(780.f, 390.f), picTextColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("PAST", sf::Vector2f(198.f, 398.f), picPastColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("\"NEW\" FUTURE", sf::Vector2f(502.f, 398.f), picFutureColor));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("TO BE ER.", sf::Vector2f(716.f, 360.f), sf::Color(255, 255, 80)));
	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("-7.0s", sf::Vector2f(502.f, 363.f), sf::Color(255, 187, 153)));

	this->ttModeInfoBackgroundTexts2.push_back(this->createTtModeInfoBackgroundText("-1.6s", sf::Vector2f(97.f, 714.f), sf::Color(255, 187, 153)));

}

void Menu::initDataErasePage() {
	// TEXT
	this->initPageHeaderText(&this->dataEraseHeaderText, "ERASE DATA");

	this->dataEraseInfoTexts = new sf::Text[this->dataEraseInfoTextsNumber];
	for (int i = 0; i < this->dataEraseInfoTextsNumber; i++) {
		this->dataEraseInfoTexts[i].setFont(*this->fontPtr);
		this->dataEraseInfoTexts[i].setString(this->dataEraseInfoTextLines[i]);
		this->dataEraseInfoTexts[i].setCharacterSize(27.f);
		this->dataEraseInfoTexts[i].setFillColor(sf::Color::White);
		this->dataEraseInfoTexts[i].setOrigin(this->dataEraseInfoTexts[i].getLocalBounds().width / 2, this->dataEraseInfoTexts[i].getLocalBounds().height / 2 + 5.f);
		this->dataEraseInfoTexts[i].setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * (2.7f + i * 0.6f));
	}

	// BUTTONS
	this->dataEraseCancelButton = new Button(&this->backButtonTexture, this->tileWidth * 3.5f, this->tileHeight * 13.5f,
		this->tileWidth * 5.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->dataEraseCancelButton->setHoverSound(this->buttonHoverSoundPtr);
	this->dataEraseCancelButton->setClickSound(this->buttonClickSoundPtr);
	this->dataEraseCancelButton->setHoverColor(sf::Color(160, 160, 160, 255));
	this->dataEraseCancelButton->setText(this->fontPtr, "CANCEL",
		sf::Vector2f(this->dataEraseCancelButton->sprite.getGlobalBounds().width / 2, this->dataEraseCancelButton->sprite.getGlobalBounds().height / 2 - 5.f),
		40.f, sf::Color::White, OriginPoint::Center);

	this->dataEraseConfirmButton = new Button(&this->backButtonTexture, this->tileWidth * 9.5f, this->tileHeight * 13.5f,
		this->tileWidth * 5.f, this->tileHeight * 1.f, OriginPoint::Center);
	this->dataEraseConfirmButton->setHoverSound(this->buttonHoverSoundPtr);
	this->dataEraseConfirmButton->setClickSound(this->buttonClickSoundPtr);
	this->dataEraseConfirmButton->setHoverColor(sf::Color(160, 160, 160, 255));
	this->dataEraseConfirmButton->setText(this->fontPtr, "ERASE",
		sf::Vector2f(this->dataEraseConfirmButton->sprite.getGlobalBounds().width / 2, this->dataEraseConfirmButton->sprite.getGlobalBounds().height / 2 - 5.f),
		40.f, sf::Color::Red, OriginPoint::Center);
}

void Menu::initPageHeaderText(sf::Text* textObjectPtr, std::string string) {
	textObjectPtr->setFont(*this->fontPtr);
	textObjectPtr->setString(string);
	textObjectPtr->setCharacterSize(this->headerTextSize);
	textObjectPtr->setFillColor(this->headerTextColor);
	textObjectPtr->setOrigin(textObjectPtr->getLocalBounds().width / 2, textObjectPtr->getLocalBounds().height / 2 + 10.f);
	textObjectPtr->setPosition(this->sprite.getGlobalBounds().width / 2, this->tileHeight * 1.5f);;
}

sf::Text* Menu::createTtModeInfoBackgroundText(std::string string, sf::Vector2f pos, sf::Color color)
{
	sf::Text* text = new sf::Text(string, *this->fontPtr, 17.f);
	text->setFillColor(color);
	text->setOrigin(text->getLocalBounds().width / 2.f, text->getLocalBounds().height / 2.f);
	text->setPosition(pos);
	return text;
}

void Menu::loadData(string directory) {
	this->dataManager.loadData(directory);
	if (!this->dataManager.verifyLoadedData(this->gamemodeButtonsNumber, this->snakeSpeedButtonsNumber,
		this->boardSizeButtonsNumber, this->fruitQuantityButtonsNumber, this->snakeLengtheningButtonsNumber)) {
		std::string errorMessage = "The loaded data has been detected to be corrupted. Press OK to recreate the save file with default data.";
		MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_ICONERROR | MB_OK);
		this->dataManager.restoreDefualtData();
		this->dataManager.saveData();
	}
}

void Menu::saveData(string directory) {
	this->dataManager.saveData(directory);
}

void Menu::saveSelectedSettings()
{
	this->dataManager.saveSelectedSettings(
		this->getActiveButtonIndex(this->gamemodeButtons, this->gamemodeButtonsNumber),
		this->getActiveButtonIndex(this->snakeSpeedButtons, this->snakeSpeedButtonsNumber),
		this->getActiveButtonIndex(this->boardSizeButtons, this->boardSizeButtonsNumber),
		this->getActiveButtonIndex(this->fruitQuantityButtons, this->fruitQuantityButtonsNumber),
		this->getActiveButtonIndex(this->snakeLengtheningButtons, this->snakeLengtheningButtonsNumber),
		this->getSelectedItem(this->snakeColors).id,
		this->getSelectedItem(this->gradientColors).id,
		this->getSelectedItem(this->tileColors).id,
		this->getSelectedItem(this->fruitTextures).id,
		this->getSelectedItem(this->musicTracks).id);
}

int Menu::getActiveButtonIndex(Button** buttonArray, int buttonArrayLength)
{
	for (int i = 0; i < buttonArrayLength; i++) {
		if (buttonArray[i]->isActive) return i;
	}
	return -1;
}

void Menu::resetActivatedButtons()
{
	this->deactivateButtonArray(this->gamemodeButtons, this->gamemodeButtonsNumber);
	this->deactivateButtonArray(this->snakeSpeedButtons, this->snakeSpeedButtonsNumber);
	this->deactivateButtonArray(this->fruitQuantityButtons, this->fruitQuantityButtonsNumber);
	this->deactivateButtonArray(this->boardSizeButtons, this->boardSizeButtonsNumber);
	this->deactivateButtonArray(this->snakeLengtheningButtons, this->snakeLengtheningButtonsNumber);

	this->deactivateButtonGroup(this->snakeColors);
	this->deactivateButtonGroup(this->gradientColors);
	this->deactivateButtonGroup(this->tileColors);
	this->deactivateButtonGroup(this->fruitTextures);
	this->deactivateButtonGroup(this->musicTracks);

	this->gamemodeButtons[this->dataManager.savedGamemodeIndex]->setIsActive(true);
	this->snakeSpeedButtons[this->dataManager.savedSnakeSpeedIndex]->setIsActive(true);
	this->fruitQuantityButtons[this->dataManager.savedFruitQuantityIndex]->setIsActive(true);
	this->boardSizeButtons[this->dataManager.savedBoardSizeIndex]->setIsActive(true);
	this->snakeLengtheningButtons[this->dataManager.savedSnakeLengtheningIndex]->setIsActive(true);

	this->getItemById(this->dataManager.savedSnakeColorId, this->snakeColors).buttonPtr->setIsActive(true);
	this->getItemById(this->dataManager.savedGradientColorId, this->gradientColors).buttonPtr->setIsActive(true);
	this->getItemById(this->dataManager.savedTileColorId, this->tileColors).buttonPtr->setIsActive(true);
	this->getItemById(this->dataManager.savedFruitTextureId, this->fruitTextures).buttonPtr->setIsActive(true);
	this->getItemById(this->dataManager.savedMusicTrackId, this->musicTracks).buttonPtr->setIsActive(true);
}


bool Menu::getLoadSuccessful() {
	return this->loadSuccessful;
}

void Menu::update(sf::RenderWindow* window, sf::Vector2f relativeTopLeftPoint)
{
	this->handleCustomizeRightClickTextPulseEffect();
	this->handleVanAnimation();
	switch (this->currentPage) {
	case MenuPage::Main:
		this->playButtonPtr->update(window);
		this->gamemodesButtonPtr->update(window);
		this->settingsButtonPtr->update(window);
		this->customizeButtonPtr->update(window);
		this->achievementsButtonPtr->update(window);
		this->infoButtonPtr->update(window);
		break;
	case MenuPage::Gamemodes:
		this->backButtonPtr->update(window);
		for (int i = 0; i < this->gamemodeButtonsNumber; i++) {
			this->gamemodeButtons[i]->update(window);
		}
		break;
	case MenuPage::Settings:
		for (int i = 0; i < this->snakeSpeedButtonsNumber; i++) {
			this->snakeSpeedButtons[i]->update(window);
		}
		for (int i = 0; i < this->boardSizeButtonsNumber; i++) {
			this->boardSizeButtons[i]->update(window);
		}
		for (int i = 0; i < this->fruitQuantityButtonsNumber; i++) {
			this->fruitQuantityButtons[i]->update(window);
		}
		for (int i = 0; i < this->snakeLengtheningButtonsNumber; i++) {
			this->snakeLengtheningButtons[i]->update(window);
		}
		this->backButtonPtr->update(window);
		break;
	case MenuPage::Customize1:
		this->backButtonPtr->update(window);
		this->customizationTab1ButtonPtr->update(window);
		this->customizationTab2ButtonPtr->update(window);
		for (const SnakeColor& color : this->snakeColors) {
			color.buttonPtr->update(window);
		}
		for (const GradientColor& color : this->gradientColors) {
			color.buttonPtr->update(window);
		}
		for (const TileColor& color : this->tileColors) {
			color.buttonPtr->update(window);
		}
		this->updateGradientColorButtonsColors();
		break;
	case MenuPage::Customize2:
		this->backButtonPtr->update(window);
		for (const FruitTexture& fruit : this->fruitTextures) {
			fruit.buttonPtr->update(window);
		}
		for (const MusicTrack& musicTrack : this->musicTracks) {
			musicTrack.buttonPtr->update(window);
		}
		this->customizationTab1ButtonPtr->update(window);
		this->customizationTab2ButtonPtr->update(window);
		break;
	case MenuPage::Achievements:
		for (int i = 0; i < DataManager::achievementsNumber; i++) {
			this->achievementButtons[i]->update(window);
		}
		this->backButtonPtr->update(window);
		break;
	case MenuPage::AchievementInfo:
		this->backButtonPtr->update(window);
		break;
	case MenuPage::GameInfo:
		this->creditsButtonPtr->update(window);
		this->ttModeInfoButtonPtr->update(window);
		this->eraseDataButtonPtr->update(window);
		this->backButtonPtr->update(window);
		break;
	case MenuPage::Credits:
		this->backButtonPtr->update(window);
		break;
	case MenuPage::TTModeInfo1:
		this->ttModeInfoPage2ButtonPtr->update(window);
		this->backButtonPtr->update(window);
		break;
	case MenuPage::TTModeInfo2:
		this->ttModeInfoPage1ButtonPtr->update(window);
		this->backButtonPtr->update(window);
		break;
	case MenuPage::DataErase:
		this->dataEraseCancelButton->update(window);
		this->dataEraseConfirmButton->update(window);
		break;
	}
}

string Menu::checkAchievementUnlocks(GameData data)
{
	data.gamemode = this->gameSettingsList.gamemode;
	data.boardUpdateFrames = this->gameSettingsList.boardUpdateFrames;
	data.boardSize = this->gameSettingsList.boardColumns;
	data.fruitQuantity = this->gameSettingsList.fruitQuantity;
	data.snakeLengthening = this->gameSettingsList.snakeLengthening;
	
	return this->dataManager.checkAchievementUnlocks(data);
}

void Menu::checkButtonClicks(sf::RenderWindow* window, sf::Mouse::Button mouseButton)
{
	MusicTrack* selectedMusic = nullptr;
	switch (this->currentPage) {
	case MenuPage::Main:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->vanAnimationFrames >= this->vanAnimationPhaseFrames * 2) {
				this->vanAnimationFrames++;
			}

			if (this->playButtonPtr->checkClicked(window)) {
				this->playButtonActivated = true;

				// Reset candy van
				if (this->vanAnimationFrames >= 0 && this->vanAnimationFrames <= this->vanAnimationPhaseFrames) {
					this->titleButtonPtr->setIsDisabled(false);
					this->vanButtonPtr->setIsDisabled(true);
				}
				this->vanSprite.setPosition(this->backgroundSprite.getGlobalBounds().width, 0.f);
				this->titleText.setFillColor(this->headerTextColor);
				this->vanAnimationFrames = -2;
				this->titleButtonClicks = 0;
			}
			else if (this->gamemodesButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Gamemodes);
			}
			else if (this->settingsButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Settings);
			}
			else if (this->customizeButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Customize1);
			}
			else if (this->achievementsButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Achievements);
			}
			else if (this->infoButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::GameInfo);
			}
			// CANDY VAN
			else if (this->vanButtonPtr->checkClicked(window) && this->vanAnimationFrames == this->vanAnimationPhaseFrames) {
				this->vanAnimationFrames++;
				this->candySound.play();
				if (this->dataManager.unlockAchievement(18)) {
					this->reloadAchievementStatus();
				}
			}
			else if (this->titleButtonPtr->checkClicked(window) && this->vanAnimationFrames < 0) {
				this->titleButtonClicks++;
				sf::Color titleTextColor = this->headerTextColor;
				titleTextColor.a = 255 - this->titleButtonClicks * 10;
				this->titleText.setFillColor(titleTextColor);
				if (this->titleButtonClicks > 20) {
					this->beginVanAnimation(1.5f);
					this->titleButtonPtr->setIsDisabled(true);
				}
			}
			break;
		}
	case MenuPage::Gamemodes:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
				this->saveSelectedSettings();
			}
			this->checkSettingsButtonArrayClicks(this->gamemodeButtons, this->gamemodeButtonsNumber, window);
		}

		if (this->getActiveButtonIndex(this->gamemodeButtons, this->gamemodeButtonsNumber) != -1)
			this->updateGamemodeDescriptionText(this->getActiveButtonIndex(this->gamemodeButtons, this->gamemodeButtonsNumber));
		break;
	case MenuPage::Settings:
		if (mouseButton == sf::Mouse::Button::Left) {
			this->checkSettingsButtonArrayClicks(this->snakeSpeedButtons, this->snakeSpeedButtonsNumber, window);
			this->checkSettingsButtonArrayClicks(this->boardSizeButtons, this->boardSizeButtonsNumber, window);
			this->checkSettingsButtonArrayClicks(this->fruitQuantityButtons, this->fruitQuantityButtonsNumber, window);
			this->checkSettingsButtonArrayClicks(this->snakeLengtheningButtons, this->snakeLengtheningButtonsNumber, window);
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
				this->saveSelectedSettings();
			}
		}
		break;
	case MenuPage::Customize1:
		this->checkCustomizationButtonGroupClicks(this->snakeColors, window, mouseButton);
		this->checkCustomizationButtonGroupClicks(this->gradientColors, window, mouseButton);
		this->checkCustomizationButtonGroupClicks(this->tileColors, window, mouseButton);

		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
				this->saveSelectedSettings();
			}
			else if (this->customizationTab1ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Customize1);
			}
			else if (this->customizationTab2ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Customize2);
			}
		}

		this->updateGradientColorButtonsColors();

		break;
	case MenuPage::Customize2:
		this->checkCustomizationButtonGroupClicks(this->fruitTextures, window, mouseButton);
		selectedMusic = this->checkCustomizationButtonGroupClicks(this->musicTracks, window, mouseButton);
		if (selectedMusic != nullptr && mouseButton == sf::Mouse::Button::Right) {
			this->currentMusicPreviewIndex = selectedMusic->externalArrayIndex;
		}

		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
				this->saveSelectedSettings();
			}
			else if (this->customizationTab1ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Customize1);
			}
			else if (this->customizationTab2ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Customize2);
			}
		}
		break;
	case MenuPage::Achievements:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
			}
			for (int i = 0; i < DataManager::achievementsNumber; i++) {
				if (this->achievementButtons[i]->checkClicked(window)) {
					this->currentAchievementInfoPageIndex = i;
					this->showItemInfoOnAchievementInfoPage = false;
					this->setCurrentPage(MenuPage::AchievementInfo);
				}
			}
		}
		break;
	case MenuPage::AchievementInfo:
		if (mouseButton == sf::Mouse::Button::Left && this->backButtonPtr->checkClicked(window)) {
			this->currentMusicPreviewIndex = -1;
			if (this->showItemInfoOnAchievementInfoPage && this->returnToCustomizationTab2) {
				this->setCurrentPage(MenuPage::Customize2);
			}
			else if (this->showItemInfoOnAchievementInfoPage && !this->returnToCustomizationTab2) {
				this->setCurrentPage(MenuPage::Customize1);
			}
			else {
				this->setCurrentPage(MenuPage::Achievements);
			}
		}
		break;
	case MenuPage::GameInfo:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Main);
			}
			else if (this->creditsButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::Credits);
			}
			else if (this->ttModeInfoButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::TTModeInfo1);
			}
			else if (this->eraseDataButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::DataErase);
			}
		}
		break;
	case MenuPage::Credits:
		if (mouseButton == sf::Mouse::Button::Left && this->backButtonPtr->checkClicked(window)) {
			this->setCurrentPage(MenuPage::GameInfo);
		}
		break;
	case MenuPage::TTModeInfo1:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::GameInfo);
			}
			else if (this->ttModeInfoPage2ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::TTModeInfo2);
			}
		}
		break;
	case MenuPage::TTModeInfo2:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->backButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::GameInfo);
			}
			else if (this->ttModeInfoPage1ButtonPtr->checkClicked(window)) {
				this->setCurrentPage(MenuPage::TTModeInfo1);
			}
		}
		break;
	case MenuPage::DataErase:
		if (mouseButton == sf::Mouse::Button::Left) {
			if (this->dataEraseCancelButton->checkClicked(window)) {
				this->setCurrentPage(MenuPage::GameInfo);
			}
			else if (this->dataEraseConfirmButton->checkClicked(window)) {
				this->dataManager.restoreDefualtData(SAVE_FILE_DIRECTORY);
				this->dataManager.saveData(SAVE_FILE_DIRECTORY);
				this->reloadAchievementStatus();
				this->resetActivatedButtons();
				this->setCurrentPage(MenuPage::Main);
			}
		}
	}
}

void Menu::checkSettingsButtonArrayClicks(Button** buttonArrayPtr, int buttonArrayLength, sf::RenderWindow* window)
{
	for (int i = 0; i < buttonArrayLength; i++) {
		if (buttonArrayPtr[i]->checkClicked(window)) {
			this->deactivateButtonArray(buttonArrayPtr, buttonArrayLength);
			buttonArrayPtr[i]->setIsActive(true);
			break;
		}
	}
}

void Menu::deactivateButtonArray(Button** buttonArrayPtr, int buttonArrayLength)
{
	for (int i = 0; i < buttonArrayLength; i++) {
		buttonArrayPtr[i]->setIsActive(false);
	}
}

void Menu::reloadAchievementStatus()
{
	std::vector<int> lockedItemsIds = this->dataManager.getLockedItems();

	for (int i = 0; i < DataManager::achievementsNumber; i++) {
		if (this->dataManager.getAchievementData(i).unlocked) {
			this->achievementButtons[i]->setDefaultTexture(&this->unlockedAchievementButtonTexture);
		}
		else {
			this->achievementButtons[i]->setDefaultTexture(&this->lockedAchievementButtonTexture);
		}
	}

	// Unlock all items
	for (SnakeColor& color : this->snakeColors) {
		color.buttonPtr->setIsDisabled(false);
	}
	for (GradientColor& color : this->gradientColors) {
		color.buttonPtr->setIsDisabled(false);
	}
	for (TileColor& color : this->tileColors) {
		color.buttonPtr->setIsDisabled(false);
	}
	for (FruitTexture& fruit : this->fruitTextures) {
		fruit.buttonPtr->setIsDisabled(false);
	}
	for (MusicTrack& musicTrack : this->musicTracks) {
		musicTrack.buttonPtr->setIsDisabled(false);
	}

	// Lock items that require achievements
	for (int id : lockedItemsIds) {
		if (id >= 100 && id < 200) { // Snake colors
			this->getItemById(id, this->snakeColors).buttonPtr->setIsDisabled(true);
		}
		else if (id >= 200 && id < 300) { // Gradient colors
			this->getItemById(id, this->gradientColors).buttonPtr->setIsDisabled(true);
		}
		else if (id >= 300 && id < 400) { // Tile colors
			this->getItemById(id, this->tileColors).buttonPtr->setIsDisabled(true);
		}
		else if (id >= 400 && id < 500) { // Fruit textures
			this->getItemById(id, this->fruitTextures).buttonPtr->setIsDisabled(true);
		}
		else if (id >= 500 && id < 600) { // Music tracks
			this->getItemById(id, this->musicTracks).buttonPtr->setIsDisabled(true);
		}
	}

	// Update achievement info pages
	for (int i = 0; i < DataManager::achievementsNumber; i++) {
		this->achievementInfoPages[i]->setAchievementUnlocked(this->dataManager.getAchievementData(i).unlocked);
	}
}

void Menu::updateGradientColorButtonsColors()
{
	SnakeColor& selectedColor = this->getSelectedItem(this->snakeColors);
	if (selectedColor.id != -1) {
		/*if (selectedColorId >= 112) selectedColorId = 101;  */// If the choosen option is not a standard color (random or rainbow), use red
		this->getItemById(213, this->gradientColors).buttonPtr->setDefaultColor(selectedColor.defualtGradient);
		this->getItemById(213, this->gradientColors).buttonPtr->setOverlayColor(selectedColor.color);
		this->getItemById(214, this->gradientColors).buttonPtr->setDefaultColor(selectedColor.color);
		this->getItemById(215, this->gradientColors).buttonPtr->setDefaultColor(selectedColor.color);
		this->getItemById(216, this->gradientColors).buttonPtr->setDefaultColor(sf::Color(255 - selectedColor.color.r,
			255 - selectedColor.color.g, 255 - selectedColor.color.b));
		this->getItemById(216, this->gradientColors).buttonPtr->setOverlay(&this->alphaOverlayTexture, selectedColor.color);
	}
}

void Menu::handleCustomizeRightClickTextPulseEffect()
{
	if (this->customizeRightClickTextPulse > 0) {
		float multipler = pow(this->customizeRightClickTextPulse, 3.f) / pow(this->customizeRightClickTextPulseMaxValue, 3.f);

		int rDiff = this->customizeRightClickTextPulseColor.r - this->customizeRightClickTextDefaultColor.r;
		int gDiff = this->customizeRightClickTextPulseColor.g - this->customizeRightClickTextDefaultColor.g;
		int bDiff = this->customizeRightClickTextPulseColor.b - this->customizeRightClickTextDefaultColor.b;

		this->customizeRightClickText.setFillColor(sf::Color(this->customizeRightClickTextDefaultColor.r + round((float)rDiff * multipler),
			this->customizeRightClickTextDefaultColor.g + round((float)gDiff * multipler),
			this->customizeRightClickTextDefaultColor.b + round((float)bDiff * multipler)));

		this->customizeRightClickTextPulse--;
	}
	else if (this->customizeRightClickText.getFillColor() != this->customizeRightClickTextDefaultColor) {
		this->customizeRightClickText.setFillColor(this->customizeRightClickTextDefaultColor);
	}
}

void Menu::beginVanAnimation(float time)
{
	this->vanAnimationPhaseFrames = round(time * FPS);
	this->vanAnimationFrames = 0;
	this->brakeSound.play();
}

void Menu::handleVanAnimation() {
	if (this->vanAnimationFrames >= 0 && this->vanAnimationFrames <= this->vanAnimationPhaseFrames) {
		this->vanSprite.setPosition(this->backgroundSprite.getGlobalBounds().width
			- (cbrt(this->vanAnimationFrames) / cbrt(this->vanAnimationPhaseFrames)) * this->backgroundSprite.getGlobalBounds().width, 0.f);

		if (this->vanAnimationFrames < this->vanAnimationPhaseFrames) {
			this->vanAnimationFrames++;
		}
		else {
			this->vanButtonPtr->setIsDisabled(false);
			this->titleButtonPtr->setIsDisabled(true);
		}
	}
	else if (this->vanAnimationFrames >= 0 && this->vanAnimationFrames <= this->vanAnimationPhaseFrames * 2) {
		this->candySprite.setColor(sf::Color(255, 255, 255, 255));
		float multiplier = (float)(this->vanAnimationFrames - this->vanAnimationPhaseFrames) / this->vanAnimationPhaseFrames;
		this->candySprite.setScale(this->defaultCandySpriteScale.x * multiplier, this->defaultCandySpriteScale.y * multiplier);
		this->candySprite.setRotation(360.f * multiplier);

		if (this->vanAnimationFrames < this->vanAnimationPhaseFrames * 2) this->vanAnimationFrames++;
	}
	else if (this->vanAnimationFrames >= vanAnimationPhaseFrames * 2) {
		this->vanSprite.setPosition(this->backgroundSprite.getGlobalBounds().width, 0.f);
		this->titleText.setFillColor(this->headerTextColor);
		this->titleButtonPtr->setIsDisabled(true);
		this->vanButtonPtr->setIsDisabled(true);
		this->vanAnimationFrames = -1;
	}
}

void Menu::updateGamemodeDescriptionText(int selectedGamemodeIndex) {
	this->gamemodeDescriptionText.setString(this->gamemodeDescriptions[selectedGamemodeIndex][0]);
	this->gamemodeDescriptionText.setOrigin(this->gamemodeDescriptionText.getLocalBounds().width / 2, this->gamemodeDescriptionText.getLocalBounds().height / 2 + 10.f);

	this->gamemodeDescriptionText2.setString(this->gamemodeDescriptions[selectedGamemodeIndex][1]);
	this->gamemodeDescriptionText2.setOrigin(this->gamemodeDescriptionText2.getLocalBounds().width / 2, this->gamemodeDescriptionText2.getLocalBounds().height / 2 + 10.f);
}

void Menu::setCurrentPage(MenuPage page)
{
	this->currentPage = page;
	switch (page) {
	case MenuPage::Customize1:
		this->customizationTab1ButtonPtr->setIsActive(true);
		this->customizationTab2ButtonPtr->setIsActive(false);
		break;
	case MenuPage::Customize2:
		this->customizationTab1ButtonPtr->setIsActive(false);
		this->customizationTab2ButtonPtr->setIsActive(true);
		break;
	default:
		break;
	}
}

bool Menu::getPlayGame()
{
	if (this->playButtonActivated) {
		this->playButtonActivated = false;
		return true;
	}
	else return false;
}

int Menu::getCurrentMusicPreviewIndex()
{
	return this->currentMusicPreviewIndex;
}

float Menu::getCurrentMusicPreviewBeginOffset()
{
	for (MusicTrack track : this->musicTracks) {
		if (track.externalArrayIndex == this->currentMusicPreviewIndex) {
			return track.previewPlayingBeginOffset;
		}
	}
	std::string errorMessage = "Menu::getCurrentMusicPreviewBeginOffset(): Couldn't find a track with the externalArrayIndex equal to "
		"this->currentMusicPreviewIndex. This error should only appear if the source code had been modified and a mistake was made. ";
	throw std::out_of_range(errorMessage);
}

float Menu::getCurrentMusicPreviewEndOffset()
{
	for (MusicTrack track : this->musicTracks) {
		if (track.externalArrayIndex == this->currentMusicPreviewIndex) {
			return track.previewPlayingEndOffset;
		}
	}
	std::string errorMessage = "Menu::getCurrentMusicPreviewEndOffset(): Couldn't find a track with the externalArrayIndex equal to "
		"this->currentMusicPreviewIndex. currentMusicPreviewIndex is out of range. This error should only appear if the source code "
		"had been modified and a mistake was made.";
	throw std::out_of_range(errorMessage);
}

GameSettings Menu::getGameSettings()
{
	// SETTINGS TAB
	int snakeSpeedSeleted = this->getActiveButtonIndex(this->snakeSpeedButtons, this->snakeSpeedButtonsNumber);
	if (snakeSpeedSeleted != -1)
		this->gameSettingsList.boardUpdateFrames = this->snakeSpeeds[snakeSpeedSeleted];

	int boardSizeSelected = this->getActiveButtonIndex(this->boardSizeButtons, this->boardSizeButtonsNumber);
	if (boardSizeSelected != -1)
	{
		this->gameSettingsList.boardRows = this->boardSizes[boardSizeSelected];
		this->gameSettingsList.boardColumns = this->boardSizes[boardSizeSelected];
	}

	int fruitQuantitySelected = this->getActiveButtonIndex(this->fruitQuantityButtons, this->fruitQuantityButtonsNumber);
	if (fruitQuantitySelected != -1)
		this->gameSettingsList.fruitQuantity = this->fruitQuantities[fruitQuantitySelected];

	int snakeLengtheningSelected = this->getActiveButtonIndex(this->snakeLengtheningButtons, this->snakeLengtheningButtonsNumber);
	if (snakeLengtheningSelected != -1)
		this->gameSettingsList.snakeLengthening = this->snakeLengtheningValues[snakeLengtheningSelected];


	// CUSTOMIZATION TAB
	const SnakeColor& snakeColorSelected = this->getSelectedItem(this->snakeColors);
	//SnakeColor& snakeColorSelected = this->getItemById(snakeColorSelected, this->snakeColors);
	switch (snakeColorSelected.id) {
	default:
		this->gameSettingsList.snakeColorBegin = snakeColorSelected.color;
		this->gameSettingsList.randomizeSnakeBeginColor = false;
		this->gameSettingsList.rainbowSnake = false;
		this->gameSettingsList.emergencySnake = false;
		break;
	case 113:  // Random colors
		this->gameSettingsList.snakeColorBegin = SNAKE_COLOR_HEAD;
		this->gameSettingsList.randomizeSnakeBeginColor = true;
		this->gameSettingsList.rainbowSnake = false;
		this->gameSettingsList.emergencySnake = false;
		break;
	case 114:  // Emergency colors
		this->gameSettingsList.snakeColorBegin = sf::Color(150, 0, 0);
		this->gameSettingsList.randomizeSnakeBeginColor = false;
		this->gameSettingsList.rainbowSnake = false;
		this->gameSettingsList.emergencySnake = true;
		break;
	case 115:  // Rainbow snake
		/*snakeColorSelected = 10;*/ // Gradient color will get set to gray (snake color: white)
		this->gameSettingsList.snakeColorBegin = sf::Color(100, 100, 100, 250); // A non-existing color to avoid mistakes
		this->gameSettingsList.randomizeSnakeBeginColor = false;
		this->gameSettingsList.rainbowSnake = true;
		this->gameSettingsList.emergencySnake = false;
		break;
	}
	
	

	const GradientColor& gradientColorSelected = this->getSelectedItem(this->gradientColors);
	switch (gradientColorSelected.id) {
	default:  // < 12 - Custom gradient color choosen
		this->gameSettingsList.snakeColorEnd = gradientColorSelected.color;
		this->gameSettingsList.minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION_2;
		this->gameSettingsList.randomizeSnakeEndingColor = false;
		this->gameSettingsList.oppositeGradientColor = false;
		break;
	case 213:  // Default gradient for the selected snake color
		this->gameSettingsList.snakeColorEnd = snakeColorSelected.defualtGradient;
		this->gameSettingsList.minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION;
		this->gameSettingsList.randomizeSnakeEndingColor = false;
		this->gameSettingsList.oppositeGradientColor = false;
		break;
	case 214:  // No gradient
		this->gameSettingsList.snakeColorEnd = this->gameSettingsList.snakeColorBegin;
		this->gameSettingsList.minSnakeGradientLength = 2;
		this->gameSettingsList.randomizeSnakeEndingColor = false;
		this->gameSettingsList.oppositeGradientColor = false;
		break;
	case 215:  // Semi-transparent
		this->gameSettingsList.snakeColorEnd = this->gameSettingsList.snakeColorBegin;
		this->gameSettingsList.snakeColorEnd.a = SNAKE_GRADIENT_TRANSPARENCY;
		this->gameSettingsList.minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION_2;
		this->gameSettingsList.randomizeSnakeEndingColor = false;
		this->gameSettingsList.oppositeGradientColor = false;
		break;
	case 216:  // Opposite color
		this->gameSettingsList.minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION_2;
		if (snakeColorSelected.id == 114) { // Emergency siren colors
			this->gameSettingsList.snakeColorEnd = sf::Color(128, 128, 128, 255);
		}
		else {
			this->gameSettingsList.snakeColorEnd.r = 255 - this->gameSettingsList.snakeColorBegin.r;
			this->gameSettingsList.snakeColorEnd.g = 255 - this->gameSettingsList.snakeColorBegin.g;
			this->gameSettingsList.snakeColorEnd.b = 255 - this->gameSettingsList.snakeColorBegin.b;
			this->gameSettingsList.snakeColorEnd.a = 255;
		}
		this->gameSettingsList.minSnakeGradientLength = 10;
		this->gameSettingsList.randomizeSnakeEndingColor = false;
		this->gameSettingsList.oppositeGradientColor = true;
		break;
	case 217:  // Randomize after fruit collection
		this->gameSettingsList.minSnakeGradientLength = MIN_SNAKE_COLOR_TRANSITION_2;
		this->gameSettingsList.snakeColorEnd = SNAKE_COLOR_ENDING;
		this->gameSettingsList.randomizeSnakeEndingColor = true;
		this->gameSettingsList.oppositeGradientColor = false;
		break;
	}
	

	const TileColor& tileColorSelected = this->getSelectedItem(this->tileColors);
	this->gameSettingsList.tileColor1 = tileColorSelected.color1;
	this->gameSettingsList.tileColor2 = tileColorSelected.color2;

	const FruitTexture& fruitTextureSelected = this->getSelectedItem(this->fruitTextures);
	this->gameSettingsList.fruitTextureIndex = fruitTextureSelected.externalArrayIndex;

	const MusicTrack& musicSelected = this->getSelectedItem(this->musicTracks);
	this->gameSettingsList.musicIndex = musicSelected.externalArrayIndex;

	int gamemodeSelected = this->getActiveButtonIndex(this->gamemodeButtons, this->gamemodeButtonsNumber);
	if (gamemodeSelected == -1) gamemodeSelected = 0;
	this->gameSettingsList.gamemode = (Gamemode)gamemodeSelected;

	this->gameSettingsList.checkTimeloopAchievement = this->gameSettingsList.gamemode == Gamemode::TimeTravel && !this->dataManager.getAchievementData(13).unlocked;

	// GAMEMODE SETTINGS OVERWRITE
	switch (this->gameSettingsList.gamemode) {
	case Gamemode::Invisible:
		this->gameSettingsList.minSnakeGradientLength = 3;
		this->gameSettingsList.snakeColorEnd = this->gameSettingsList.snakeColorBegin;
		//this->gameSettingsList.snakeColorEnd.a = 0;
		break;
	case Gamemode::Portal:
		if (this->gameSettingsList.fruitQuantity > 5) {
			this->gameSettingsList.fruitQuantity = 5;
		}
		break;
	case Gamemode::TimeTravel:
		if (this->gameSettingsList.fruitQuantity > 5) {
			this->gameSettingsList.fruitQuantity = 5;
		}
		break;
	default:
		break;
	}

	return this->gameSettingsList;
}


void Menu::drawRenderTexture() {
	this->renderTexture.clear();
	this->renderTexture.draw(this->backgroundSprite);
	switch (this->currentPage) {
	case MenuPage::Main:
		this->playButtonPtr->draw(&this->renderTexture);
		this->gamemodesButtonPtr->draw(&this->renderTexture);
		this->settingsButtonPtr->draw(&this->renderTexture);
		this->customizeButtonPtr->draw(&this->renderTexture);
		this->achievementsButtonPtr->draw(&this->renderTexture);
		this->infoButtonPtr->draw(&this->renderTexture);
		if (this->vanAnimationFrames < 0) {
			this->renderTexture.draw(this->titleText);
		}
		else {
			this->renderTexture.draw(this->vanSprite);
			if (this->vanAnimationFrames > this->vanAnimationPhaseFrames) {
				this->renderTexture.draw(this->candySprite);
			}
		}
		
		break;
	case MenuPage::Gamemodes:
		this->renderTexture.draw(this->gamemodePageInfoText);
		this->renderTexture.draw(this->gamemodeDescriptionText);
		this->renderTexture.draw(this->gamemodeDescriptionText2);
		this->backButtonPtr->draw(&this->renderTexture);
		for (int i = 0; i < this->gamemodeButtonsNumber; i++) {
			this->gamemodeButtons[i]->draw(&this->renderTexture);
		}
		this->renderTexture.draw(this->gamemodesHeaderText);
		break;
	case MenuPage::Settings:
		for (int i = 0; i < this->snakeSpeedButtonsNumber; i++) {
			this->snakeSpeedButtons[i]->draw(&this->renderTexture);
		}
		for (int i = 0; i < this->boardSizeButtonsNumber; i++) {
			this->boardSizeButtons[i]->draw(&this->renderTexture);
		}
		for (int i = 0; i < this->fruitQuantityButtonsNumber; i++) {
			this->fruitQuantityButtons[i]->draw(&this->renderTexture);
		}
		for (int i = 0; i < this->snakeLengtheningButtonsNumber; i++) {
			this->snakeLengtheningButtons[i]->draw(&this->renderTexture);
		}
		this->renderTexture.draw(this->snakeSpeedText);
		this->renderTexture.draw(this->boardSizeText);
		this->renderTexture.draw(this->fruitAmountText);
		this->renderTexture.draw(this->snakeLengthingText);
		this->backButtonPtr->draw(&this->renderTexture);
		this->renderTexture.draw(this->settingsHeaderText);
		break;
	case MenuPage::Customize1:
		this->renderTexture.draw(this->customizationTabSprite);
		for (const SnakeColor& color : this->snakeColors) {
			color.buttonPtr->draw(&this->renderTexture);
		}
		for (const GradientColor& color : this->gradientColors) {
			color.buttonPtr->draw(&this->renderTexture);
		}
		for (const TileColor& color : this->tileColors) {
			color.buttonPtr->draw(&this->renderTexture);
		}
		this->renderTexture.draw(this->snakeColorText);
		this->renderTexture.draw(this->gradientColorText);
		this->renderTexture.draw(this->tileColorText);
		this->backButtonPtr->draw(&this->renderTexture);
		this->customizationTab1ButtonPtr->draw(&this->renderTexture);
		this->customizationTab2ButtonPtr->draw(&this->renderTexture);
		this->renderTexture.draw(this->customizeRightClickText);
		this->renderTexture.draw(this->customizeHeaderText);
		break;
	case MenuPage::Customize2:
		this->renderTexture.draw(this->customizationTabSprite);
		this->renderTexture.draw(this->fruitTextureText);
		this->renderTexture.draw(this->musicText);
		for (const FruitTexture& color : this->fruitTextures) {
			color.buttonPtr->draw(&this->renderTexture);
		}
		for (const MusicTrack& musicTrack : this->musicTracks) {
			musicTrack.buttonPtr->draw(&this->renderTexture);
		}
		this->backButtonPtr->draw(&this->renderTexture);
		this->customizationTab1ButtonPtr->draw(&this->renderTexture);
		this->customizationTab2ButtonPtr->draw(&this->renderTexture);
		this->renderTexture.draw(this->customizeRightClickText);
		this->renderTexture.draw(this->customizeHeaderText);
		break;
	case MenuPage::Achievements:
		for (int i = 0; i < DataManager::achievementsNumber; i++) {
			this->achievementButtons[i]->draw(&this->renderTexture);
		}
		for (auto label : this->achievementNameLabels) {
			this->renderTexture.draw(*label);
		}
		this->backButtonPtr->draw(&this->renderTexture);
		this->renderTexture.draw(this->achievementsHeaderText);
		break;
	case MenuPage::AchievementInfo:
		this->backButtonPtr->draw(&this->renderTexture);
		if (this->currentAchievementInfoPageIndex >= 0) {
			this->achievementInfoPages[this->currentAchievementInfoPageIndex]->draw(this->renderTexture, this->showItemInfoOnAchievementInfoPage, false);
		}
		else {
			this->achievementInfoPages[0]->draw(this->renderTexture, this->showItemInfoOnAchievementInfoPage, true);
		}
		
		break;
	case MenuPage::GameInfo:
		this->creditsButtonPtr->draw(&this->renderTexture);
		this->ttModeInfoButtonPtr->draw(&this->renderTexture);
		this->eraseDataButtonPtr->draw(&this->renderTexture);
		this->backButtonPtr->draw(&this->renderTexture);
		this->renderTexture.draw(this->gameInfoHeaderText);
		break;
	case MenuPage::Credits:
		this->renderTexture.draw(this->creditsHeaderText);
		for (int i = 0; i < this->creditsTextsNumber; i++) {
			this->renderTexture.draw(this->creditsTexts[i]);
		}
		this->backButtonPtr->draw(&this->renderTexture);
		break;
	case MenuPage::TTModeInfo1:
		this->renderTexture.draw(this->ttModeInfoPictures[0].sprite);
		this->renderTexture.draw(this->ttModeInfoPictures[1].sprite);
		for (sf::Text* textPtr : this->ttModeInfoBackgroundTexts1) {
			this->renderTexture.draw(*textPtr);
		}
		for (int i = 0; i < this->ttModeInfoTextLines1.size(); i++) {
			this->renderTexture.draw(this->ttModeInfoTexts1[i]);
		}
		this->renderTexture.draw(this->ttModeInfoHeaderText);
		this->ttModeInfoPage2ButtonPtr->draw(&this->renderTexture);
		this->backButtonPtr->draw(&this->renderTexture);
		break;
	case MenuPage::TTModeInfo2:
		this->renderTexture.draw(this->ttModeInfoPictures[2].sprite);
		this->renderTexture.draw(this->ttModeInfoPictures[3].sprite);
		for (sf::Text* textPtr : this->ttModeInfoBackgroundTexts2) {
			this->renderTexture.draw(*textPtr);
		}
		for (int i = 0; i < this->ttModeInfoTextLines2.size(); i++) {
			this->renderTexture.draw(this->ttModeInfoTexts2[i]);
		}
		this->renderTexture.draw(this->ttModeInfoHeaderText);
		this->ttModeInfoPage1ButtonPtr->draw(&this->renderTexture);
		this->backButtonPtr->draw(&this->renderTexture);
		break;
	case MenuPage::DataErase:
		this->renderTexture.draw(this->dataEraseIconSprite);
		for (int i = 0; i < this->dataEraseInfoTextsNumber; i++) {
			this->renderTexture.draw(this->dataEraseInfoTexts[i]);
		}
		this->dataEraseCancelButton->draw(&this->renderTexture);
		this->dataEraseConfirmButton->draw(&this->renderTexture);
		this->renderTexture.draw(this->dataEraseHeaderText);
		break;
	}
	
	this->renderTexture.display();
}
