#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace std;

class AchievementInfoPage
{
private:
	sf::Texture itemIconTexture;
	sf::Sprite itemIconSprite;
	static sf::Texture* unlockedAchievementLabelTexturePtr;
	static sf::Sprite unlockedAchievementLabelSprite;
	static sf::Texture* lockedAchievementLabelTexturePtr;
	static sf::Sprite lockedAchievementLabelSprite;

	static sf::Font* fontPtr;

	static sf::Text lockedText;
	static sf::Text unlockedText;
	static sf::Text unlockedByDefaultText1;
	static sf::Text unlockedByDefaultText2;

	static sf::Text itemNameText;
	static sf::Text itemNameUpperText;
	static sf::Text itemNameLowerText;
	sf::Text achievementNameText;
	static sf::Text extraRequirementsHeaderText;
	sf::Text* descriptionText;
	int descriptionTextLines = 1;
	sf::Text* extraRequirementsTexts;
	int extraRequirementsNumber;

	static float pageWidth;
	static float pageHeight;
	static float tileWidth;
	static float tileHeight;

	bool unlocked = false;

public:
	static void initStaticElements(float pageWidth, float pageHeight, float tileWidth, float tileHeight,
		sf::Font* fontPtr, sf::Texture* unlockedAchievementLabelTexturePtr, sf::Texture* lockedAchievementLabelTexturePtr);

	AchievementInfoPage(const string& achievementName, const vector<string>& achievementDescription, const vector<string>& extraRequirements = vector<string>());

	void setDisplayedUnlockableItem(string itemName, sf::Texture itemTexture);
	void setAchievementUnlocked(bool unlocked);

	template <typename RenderObject>
	void draw(RenderObject& renderObject, bool displayItemInfo, bool displayUnlockedByDefaultText = false);
};

template <typename RenderObject>
void AchievementInfoPage::draw(RenderObject& renderObject, bool displayItemInfo, bool displayUnlockedByDefaultText) {
	if (displayItemInfo) {
		renderObject.draw(this->itemIconSprite);
		renderObject.draw(this->itemNameText);
		renderObject.draw(this->itemNameUpperText);
		renderObject.draw(this->itemNameLowerText);
	}
	else {
		if (this->unlocked) {
			renderObject.draw(this->unlockedText);
		}
		else {
			renderObject.draw(this->lockedText);
		}
	}

	if (displayUnlockedByDefaultText) {
		renderObject.draw(this->unlockedByDefaultText1);
		renderObject.draw(this->unlockedByDefaultText2);
	}
	else {
		if (this->unlocked) {
			renderObject.draw(this->unlockedAchievementLabelSprite);
		}
		else {
			renderObject.draw(this->lockedAchievementLabelSprite);
		}

		renderObject.draw(this->achievementNameText);

		for (int i = 0; i < this->descriptionTextLines; i++) {
			renderObject.draw(this->descriptionText[i]);
		}

		if (this->extraRequirementsNumber > 0) {
			renderObject.draw(this->extraRequirementsHeaderText);
			for (int i = 0; i < this->extraRequirementsNumber; i++) {
				renderObject.draw(this->extraRequirementsTexts[i]);
			}
		}
	}
}
