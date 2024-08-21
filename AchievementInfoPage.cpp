#include "AchievementInfoPage.h"


sf::Texture* AchievementInfoPage::unlockedAchievementLabelTexturePtr;
sf::Sprite AchievementInfoPage::unlockedAchievementLabelSprite;
sf::Texture* AchievementInfoPage::lockedAchievementLabelTexturePtr;
sf::Sprite AchievementInfoPage::lockedAchievementLabelSprite;

sf::Text AchievementInfoPage::unlockedByDefaultText1;
sf::Text AchievementInfoPage::unlockedByDefaultText2;

sf::Text AchievementInfoPage::itemNameText;
sf::Text AchievementInfoPage::itemNameUpperText;
sf::Text AchievementInfoPage::itemNameLowerText;

sf::Font* AchievementInfoPage::fontPtr;
sf::Text AchievementInfoPage::lockedText;
sf::Text AchievementInfoPage::unlockedText;
sf::Text AchievementInfoPage::extraRequirementsHeaderText;

float AchievementInfoPage::pageWidth;
float AchievementInfoPage::pageHeight;
float AchievementInfoPage::tileWidth;
float AchievementInfoPage::tileHeight;

// static init function
void AchievementInfoPage::initStaticElements(float pageWidth, float pageHeight, float tileWidth, float tileHeight, sf::Font* fontPtr,
	sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr)
{
	AchievementInfoPage::pageWidth = pageWidth;
	AchievementInfoPage::pageHeight = pageHeight;
	AchievementInfoPage::tileWidth = tileWidth;
	AchievementInfoPage::tileHeight = tileHeight;
	AchievementInfoPage::fontPtr = fontPtr;

	lockedText.setFont(*fontPtr);
	lockedText.setString("LOCKED");
	lockedText.setCharacterSize(60.f);
	lockedText.setFillColor(sf::Color(180, 35, 35));
	lockedText.setOrigin(lockedText.getLocalBounds().width / 2, lockedText.getLocalBounds().height / 2 + 10.f);
	lockedText.setPosition(pageWidth / 2.f, tileHeight * 2.f);

	unlockedText.setFont(*fontPtr);
	unlockedText.setString("UNLOCKED");
	unlockedText.setCharacterSize(60.f);
	unlockedText.setFillColor(sf::Color(35, 180, 40));
	unlockedText.setOrigin(unlockedText.getLocalBounds().width / 2, unlockedText.getLocalBounds().height / 2 + 10.f);
	unlockedText.setPosition(pageWidth / 2.f, tileHeight * 2.f);

	extraRequirementsHeaderText.setFont(*fontPtr);
	extraRequirementsHeaderText.setString("Extra requirements:");
	extraRequirementsHeaderText.setCharacterSize(28.f);
	extraRequirementsHeaderText.setFillColor(sf::Color::Yellow);
	extraRequirementsHeaderText.setOrigin(extraRequirementsHeaderText.getLocalBounds().width / 2, extraRequirementsHeaderText.getLocalBounds().height / 2 + 5.f);
	extraRequirementsHeaderText.setPosition(pageWidth / 2.f, tileHeight * 9.5f);

	AchievementInfoPage::unlockedAchievementLabelTexturePtr = unlockedAchievementLabelTexturePtr;
	AchievementInfoPage::lockedAchievementLabelTexturePtr = lockedAchievementLabelTexturePtr;

	AchievementInfoPage::unlockedAchievementLabelSprite.setTexture(*unlockedAchievementLabelTexturePtr);
	sf::Vector2u textureSize = unlockedAchievementLabelSprite.getTexture()->getSize();
	unlockedAchievementLabelSprite.setScale(tileWidth * 9.f / textureSize.x, tileHeight / textureSize.y);
	unlockedAchievementLabelSprite.setPosition(tileWidth * 2.f, tileHeight * 4.f);

	AchievementInfoPage::lockedAchievementLabelSprite.setTexture(*lockedAchievementLabelTexturePtr);
	textureSize = lockedAchievementLabelSprite.getTexture()->getSize();
	lockedAchievementLabelSprite.setScale(tileWidth * 9.f / textureSize.x, tileHeight / textureSize.y);
	lockedAchievementLabelSprite.setPosition(tileWidth * 2.f, tileHeight * 4.f);

	float unlockedByDefaultTextPosY = tileHeight * 6.f;
	float unlockedByDefualtTextSize = 50.f;

	unlockedByDefaultText1.setFont(*fontPtr);
	unlockedByDefaultText1.setString("UNLOCKED BY");
	unlockedByDefaultText1.setCharacterSize(unlockedByDefualtTextSize);
	unlockedByDefaultText1.setFillColor(sf::Color(200, 200, 200));
	unlockedByDefaultText1.setOrigin(unlockedByDefaultText1.getLocalBounds().width / 2, unlockedByDefaultText1.getLocalBounds().height / 2 + 10.f);
	unlockedByDefaultText1.setPosition(pageWidth / 2, unlockedByDefaultTextPosY);

	unlockedByDefaultText2.setFont(*fontPtr);
	unlockedByDefaultText2.setString("DEFAULT");
	unlockedByDefaultText2.setCharacterSize(unlockedByDefualtTextSize);
	unlockedByDefaultText2.setFillColor(sf::Color(200, 200, 200));
	unlockedByDefaultText2.setOrigin(unlockedByDefaultText2.getLocalBounds().width / 2, 10.f);
	unlockedByDefaultText2.setPosition(pageWidth / 2.f, unlockedByDefaultTextPosY + tileHeight * 1.f);

	itemNameText.setFont(*fontPtr);
	itemNameText.setCharacterSize(50.f);
	itemNameText.setFillColor(sf::Color(255, 255, 255));
	itemNameText.setOrigin(itemNameText.getLocalBounds().width / 2, itemNameText.getLocalBounds().height / 2 + 10.f);
	itemNameText.setPosition((pageWidth - tileWidth * 4.f) / 2.f + (tileWidth * 3.f), tileHeight * 2.f);

	itemNameUpperText.setFont(*fontPtr);
	itemNameUpperText.setCharacterSize(50.f);
	itemNameUpperText.setFillColor(sf::Color(255, 255, 255));
	itemNameUpperText.setOrigin(itemNameUpperText.getLocalBounds().width / 2, itemNameUpperText.getLocalBounds().height / 2 + 10.f);
	itemNameUpperText.setPosition((pageWidth - tileWidth * 4.f) / 2.f + (tileWidth * 3.f), tileHeight * 1.5f);

	itemNameLowerText.setFont(*fontPtr);
	itemNameLowerText.setCharacterSize(50.f);
	itemNameLowerText.setFillColor(sf::Color(255, 255, 255));
	itemNameLowerText.setOrigin(itemNameLowerText.getLocalBounds().width / 2, itemNameLowerText.getLocalBounds().height / 2 + 10.f);
	itemNameLowerText.setPosition((pageWidth - tileWidth * 4.f) / 2.f + (tileWidth * 3.f), tileHeight * 2.5f);
}


AchievementInfoPage::AchievementInfoPage(const string& achievementName, const vector<string>& achievementDescription, const vector<string>& extraRequirements)
{
	this->achievementNameText.setFont(*fontPtr);
	this->achievementNameText.setString(achievementName);
	this->achievementNameText.setCharacterSize(40.f);
	this->achievementNameText.setFillColor(sf::Color(255, 255, 255));
	this->achievementNameText.setOrigin(this->achievementNameText.getLocalBounds().width / 2, this->achievementNameText.getLocalBounds().height / 2 + 8.f);
	this->achievementNameText.setPosition((pageWidth - tileWidth * 5.f) / 2.f + (tileWidth * 3.f), tileHeight * 4.5f);

	this->descriptionTextLines = achievementDescription.size();
	this->descriptionText = new sf::Text[this->descriptionTextLines];

	float spaceBetweenLines = tileHeight * 0.6f;
	float posY = tileHeight * (this->descriptionTextLines < 2 ? 6.5f : 6.4f);
	
	for (int i = 0; i < this->descriptionTextLines; i++) {
		this->descriptionText[i].setFont(*fontPtr);
		this->descriptionText[i].setString(achievementDescription[i]);
		this->descriptionText[i].setCharacterSize(25.f);
		this->descriptionText[i].setFillColor(sf::Color(255, 255, 255));
		this->descriptionText[i].setOrigin(this->descriptionText[i].getLocalBounds().width / 2, this->descriptionText[i].getLocalBounds().height / 2 + 5.f);
		this->descriptionText[i].setPosition(pageWidth / 2.f, posY);

		posY += spaceBetweenLines;
	}

	this->extraRequirementsNumber = extraRequirements.size();


	if (this->extraRequirementsNumber > 0) {
		this->extraRequirementsTexts = new sf::Text[this->extraRequirementsNumber];

		for (int i = 0; i < this->extraRequirementsNumber; i++) {
			this->extraRequirementsTexts[i].setFont(*fontPtr);
			this->extraRequirementsTexts[i].setString("- " + extraRequirements[i]);
			this->extraRequirementsTexts[i].setCharacterSize(23.5f);
			this->extraRequirementsTexts[i].setFillColor(sf::Color(255, 255, 255));
			if (this->extraRequirementsNumber <= 2) {
				float posX = pageWidth / 2.f;
				float posY = tileHeight * (10.5f + i);
				this->extraRequirementsTexts[i].setOrigin(this->extraRequirementsTexts[i].getLocalBounds().width / 2, this->extraRequirementsTexts[i].getLocalBounds().height / 2 + 5.f);
				this->extraRequirementsTexts[i].setPosition(posX, posY);
			}
			else {
				float posX = i % 2 == 0 ? tileWidth * 0.15f : tileWidth * 7.15f;
				float posY = tileHeight * (10.5f + floor(i / 2.f));
				this->extraRequirementsTexts[i].setOrigin(0.f, this->extraRequirementsTexts[i].getLocalBounds().height / 2 + 5.f);
				this->extraRequirementsTexts[i].setPosition(posX, posY);
			}
		}
	}

	
}

void AchievementInfoPage::setAchievementUnlocked(bool unlocked) {
	this->unlocked = unlocked;
}

void AchievementInfoPage::setDisplayedUnlockableItem(string itemName, sf::Texture itemTexture)
{
	this->itemIconTexture = itemTexture;
	this->itemIconSprite.setTexture(this->itemIconTexture);
	sf::Vector2u textureSize = this->itemIconSprite.getTexture()->getSize();
	this->itemIconSprite.setScale(tileWidth * 2.f / textureSize.x, tileHeight * 2.f / textureSize.y);
	this->itemIconSprite.setPosition(tileWidth * 1.f, tileHeight * 1.f);

	// Split the name into 2 lines if it's long
	bool twoLines = false;
	string itemNameUpperString = "";
	string itemNameLowerString = "";

	if (itemName.length() > 12) {
		int minDifference = std::numeric_limits<int>::max();
		size_t splitPos = std::string::npos;

		for (size_t i = 0; i < itemName.length(); ++i) {
			if (itemName[i] == ' ') {
				int length1 = i;
				int length2 = itemName.length() - i - 1;
				int difference = std::abs(length1 - length2);

				if (difference < minDifference) {
					minDifference = difference;
					splitPos = i;
				}
			}
		}

		if (splitPos != std::string::npos) {
			twoLines = true;
			itemNameUpperString = itemName.substr(0, splitPos);
			itemNameLowerString = itemName.substr(splitPos + 1);
		}
	}
	
	if (twoLines) {
		itemNameText.setString("");

		itemNameUpperText.setString(itemNameUpperString);
		itemNameUpperText.setOrigin(this->itemNameUpperText.getLocalBounds().width / 2, this->itemNameUpperText.getLocalBounds().height / 2 + 10.f);
		itemNameLowerText.setString(itemNameLowerString);
		itemNameLowerText.setOrigin(this->itemNameLowerText.getLocalBounds().width / 2, this->itemNameLowerText.getLocalBounds().height / 2 + 10.f);
	}
	else {
		itemNameUpperText.setString("");
		itemNameLowerText.setString("");

		itemNameText.setString(itemName);
		itemNameText.setOrigin(this->itemNameText.getLocalBounds().width / 2, this->itemNameText.getLocalBounds().height / 2 + 10.f);
	}
}

