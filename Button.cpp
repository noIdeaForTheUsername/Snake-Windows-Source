#include "Button.h"


Button::Button(sf::Texture* buttonTexturePtr, float posX, float posY, float width, float height,
    OriginPoint originPoint, sf::Vector2f relativeTopLeftPoint)
    : defaultColor(255, 255, 255, 255), hoverColor(255, 255, 255, 255), textColor(255, 255, 255, 255), activeColor(255, 255, 255, 255), disabledColor(255, 255, 255, 255),
    hoverMovementPerFrame(0, 0), textHoverMovementPerFrame(0, 0)
{
    this->width = width;
    this->height = height;
    this->buttonTexturePtr = buttonTexturePtr;
    this->originPoint = originPoint;

    this->relativeTopLeftPoint = relativeTopLeftPoint;
    this->hoverMovementPhase = 0;

    this->renderTexture.create(this->width, this->height);

    this->sprite.setPosition(posX, posY);
    this->sprite.setTexture(this->renderTexture.getTexture());
    sf::FloatRect spriteSize = this->sprite.getLocalBounds();  // Why the fuck doesn't it work without scaling?
    this->sprite.setScale(this->width / spriteSize.width, this->height / spriteSize.height);

    switch (this->originPoint) {
    case OriginPoint::Topleft:
        this->sprite.setOrigin(0, 0);
        break;
    case OriginPoint::Center:
        this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
        break;
    case OriginPoint::Custom:
        break;
    }

    this->buttonSprite.setPosition(0, 0);
    this->setSpriteTexture(&this->buttonSprite, this->buttonTexturePtr);
}

Button::~Button()
{
    delete this->hoverIconSprite, this->activeIconSprite, this->disabledIconSprite;
}

void Button::setDefaultColor(sf::Color color) {
    this->defaultColor = color;
    this->updateColor();
}

void Button::setOverlayColor(sf::Color color) {
    this->overlayColor = color;
    this->updateColor();
}

void Button::setHoverColor(sf::Color color)
{
    this->hoverColor = color;
    this->updateColor();
}

void Button::setActiveColor(sf::Color color)
{
    this->activeColor = color;
    this->updateColor();
}

void Button::setDisabledColor(sf::Color color) {
    this->disabledColor = color;
    this->updateColor();
}

void Button::setDefaultTexture(sf::Texture* texturePtr) {
    this->setSpriteTexture(&this->buttonSprite, texturePtr);
}

void Button::setOverlay(sf::Texture* texturePtr, sf::Color color) {
    this->overlaySprite = new sf::Sprite();
    this->setSpriteTexture(this->overlaySprite, texturePtr);
    this->overlayColor = color;
    this->overlaySprite->setColor(this->overlayColor);
}

void Button::setHoverIcon(sf::Texture* texturePtr) {
    this->hoverIconSprite = new sf::Sprite();
    this->setSpriteTexture(this->hoverIconSprite, texturePtr);
    this->updateColor();
}

void Button::setActiveIcon(sf::Texture* texturePtr)
{
    this->activeIconSprite = new sf::Sprite();
    this->setSpriteTexture(this->activeIconSprite, texturePtr);
    this->updateColor();
}

void Button::setDisabledIcon(sf::Texture* texturePtr)
{
    this->disabledIconSprite = new sf::Sprite();
    this->setSpriteTexture(this->disabledIconSprite, texturePtr);
    this->updateColor();
}


void Button::setClickSound(sf::Sound* soundPtr)
{
    this->clickSoundPtr = soundPtr;
}


void Button::setDisabledClickSound(sf::Sound* soundPtr) {
    if (soundPtr == nullptr) return;
    this->disabledClickSoundPtr = soundPtr;
}


void Button::setHoverSound(sf::Sound* soundPtr, bool playAtHoverEnter, bool playAtHoverExit)
{
    if (soundPtr == nullptr) return;
    this->playSoundAtHoverEnter = playAtHoverEnter;
    this->playSoundAtHoverExit = playAtHoverExit;
    this->hoverSoundPtr = soundPtr;
}

void Button::setText(sf::Font* fontPtr, std::string string, sf::Vector2f position, float textSize, sf::Color color, OriginPoint originPoint)
{
    this->hasText = true;
    this->textColor = color;
    this->text.setFont(*fontPtr);
    this->text.setString(string);
    this->text.setCharacterSize(textSize);
    //this->text.setPosition(this->sprite.getPosition() -
    //    sf::Vector2f((this->sprite.getOrigin().x * this->sprite.getScale().x), (this->sprite.getOrigin().y * this->sprite.getScale().y)) + position);
    //this->text.setPosition(this->sprite.getPosition().x + position.x, this->sprite.getPosition().y + position.y);
    this->text.setPosition(position);
    this->text.setFillColor(color);
    this->textOriginPoint = originPoint;
    this->resetTextOriginPoint();
    this->updateColor();
}

void Button::setTextOriginPoint(OriginPoint newOriginPoint, sf::Vector2f customOriginPoint)
{
    this->textOriginPoint = newOriginPoint;
    this->text.setOrigin(customOriginPoint);
}


void Button::resetTextOriginPoint() {
    if (this->hasText) {
        switch (this->textOriginPoint) {
        case OriginPoint::Topleft:
            this->text.setOrigin(0, 0);
            break;
        case OriginPoint::Center:
            this->text.setOrigin(this->text.getLocalBounds().width / 2, this->text.getLocalBounds().height / 2);
            break;
        case OriginPoint::Custom:
            break;
        }
    }
}

void Button::setHoverMovement(float x, float y, float time, float textX, float textY)
{
    this->totalHoverMovementFrames = FPS * time;
    this->hoverMovementPerFrame.x = x / this->totalHoverMovementFrames;
    this->hoverMovementPerFrame.y = y / this->totalHoverMovementFrames;
    this->textHoverMovementPerFrame.x = textX / this->totalHoverMovementFrames;
    this->textHoverMovementPerFrame.y = textY / this->totalHoverMovementFrames;
    this->moveAtHover = true;
}

void Button::setSpriteTexture(sf::Sprite* spritePtr, sf::Texture* newTexturePtr) {
    spritePtr->setTexture(*newTexturePtr);

    sf::FloatRect spriteSize = spritePtr->getLocalBounds();
    spritePtr->setScale(this->width / spriteSize.width, this->height / spriteSize.height);
}

bool Button::checkMouseOver(sf::RenderWindow* window, sf::Vector2f relativeTopLeftPoint) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    mousePos.x -= relativeTopLeftPoint.x;
    mousePos.y -= relativeTopLeftPoint.y;

    sf::FloatRect buttonBounds = this->sprite.getGlobalBounds();
    sf::IntRect windowBounds(0, 0, window->getSize().x, window->getSize().y);
    return buttonBounds.contains(mousePos.x, mousePos.y) && windowBounds.contains(mousePos.x, mousePos.y) && window->hasFocus();
}

bool Button::checkClicked(sf::RenderWindow* window, bool ignoreDisabledParameter, bool playSound)
{
    if (this->checkMouseOver(window, this->relativeTopLeftPoint)) {
        if (isDisabled && !ignoreDisabledParameter) {
            if (this->disabledClickSoundPtr != nullptr && playSound) this->disabledClickSoundPtr->play();
            return false;
        }
        else {
            if (this->clickSoundPtr != nullptr && playSound) this->clickSoundPtr->play();
            return true;
        }
    }
    else
        return false;
}


void Button::updateColor()
{
    sf::Color newSpriteColor = this->defaultColor;
    sf::Color newOverlayColor = this->overlayColor;
    sf::Color newTextColor = this->textColor;
    sf::Color newDisabledIconColor = sf::Color(255, 255, 255, 255);

    if (this->atHover) {
        newSpriteColor *= this->hoverColor;
        newTextColor *= this->hoverColor;
        newOverlayColor *= this->hoverColor;
        newDisabledIconColor *= this->hoverColor;
    }
    if (this->isActive) {
        newSpriteColor *= this->activeColor;
        newTextColor *= this->activeColor;
        newOverlayColor *= this->activeColor;
    }
    if (this->isDisabled) {
        newSpriteColor *= this->disabledColor;
        newTextColor *= this->disabledColor;
        newOverlayColor *= this->disabledColor;
    }

    if (this->buttonSprite.getColor() != newSpriteColor) this->buttonSprite.setColor(newSpriteColor);
    if (this->overlaySprite != nullptr && this->overlaySprite->getColor() != newOverlayColor) this->overlaySprite->setColor(newOverlayColor);
    if (this->disabledIconSprite != nullptr && this->disabledIconSprite->getColor() != newDisabledIconColor) this->disabledIconSprite->setColor(newDisabledIconColor);
    if (this->hasText && this->text.getFillColor() != newTextColor) this->text.setFillColor(newTextColor);
}

void Button::setAtHover(bool newHover) {
    if (newHover == this->atHover) return;
    if ((newHover == true && this->playSoundAtHoverEnter) || (newHover == false && this->playSoundAtHoverExit))
        this->hoverSoundPtr->play();
    this->atHover = newHover;
    this->updateColor();
}

void Button::setIsActive(bool newIsActive)
{
    if (newIsActive == this->isActive) return;
    this->isActive = newIsActive;
    this->updateColor();
}

void Button::setIsDisabled(bool isDisabled)
{
    if (isDisabled == this->isDisabled) return;
    this->isDisabled = isDisabled;
    this->updateColor();
}

void Button::handleHoverMovement()
{
    if (this->moveAtHover) {
        if (this->atHover && this->hoverMovementPhase < this->totalHoverMovementFrames) {
            this->hoverMovementPhase++;
            this->sprite.setPosition(this->sprite.getPosition() + this->hoverMovementPerFrame);
            if (this->hasText) this->text.setPosition(this->text.getPosition() + this->textHoverMovementPerFrame);
        }
        else if (!this->atHover && this->hoverMovementPhase > 0) {
            this->hoverMovementPhase--;
            this->sprite.setPosition(this->sprite.getPosition() - this->hoverMovementPerFrame);
            if (this->hasText) this->text.setPosition(this->text.getPosition() - this->textHoverMovementPerFrame);
        }
    }
}

void Button::update(sf::RenderWindow* window) {
    bool checkAtHover = this->checkMouseOver(window, this->relativeTopLeftPoint);
    if (this->atHover != checkAtHover) this->setAtHover(checkAtHover);
    if (this->moveAtHover) this->handleHoverMovement();
}

sf::Texture Button::getStandardTexture() {
    sf::RenderTexture renderTextureToReturn;
    renderTextureToReturn.create(this->width, this->height);
    renderTextureToReturn.clear(sf::Color(0, 0, 0, 0));

    sf::Color savedColor = this->buttonSprite.getColor();
    this->buttonSprite.setColor(this->defaultColor);
    renderTextureToReturn.draw(this->buttonSprite);
    this->buttonSprite.setColor(savedColor);

    if (this->overlaySprite != nullptr) {
        savedColor = this->overlaySprite->getColor();
        this->overlaySprite->setColor(this->overlayColor);
        renderTextureToReturn.draw(*this->overlaySprite);
        this->overlaySprite->setColor(savedColor);
    }
    if (this->hasText) {
        savedColor = this->text.getFillColor();
        this->text.setFillColor(this->textColor);
        renderTextureToReturn.draw(this->text);
        this->text.setFillColor(savedColor);
    }

    renderTextureToReturn.display();
    sf::Texture textureToReturn;
    textureToReturn.loadFromImage(renderTextureToReturn.getTexture().copyToImage());

    return textureToReturn;
}

void Button::drawRenderTexture() {
    this->renderTexture.clear(sf::Color(0, 0, 0, 0));
    this->renderTexture.draw(this->buttonSprite);
    if (this->overlaySprite != nullptr) this->renderTexture.draw(*this->overlaySprite);
    if (this->hasText) this->renderTexture.draw(this->text);
    if (this->hoverIconSprite != nullptr && this->atHover) this->renderTexture.draw(*this->hoverIconSprite);
    if (this->activeIconSprite != nullptr && this->isActive) this->renderTexture.draw(*this->activeIconSprite);
    if (this->disabledIconSprite != nullptr && this->isDisabled) this->renderTexture.draw(*this->disabledIconSprite);
    this->renderTexture.display();
    this->sprite.setTexture(this->renderTexture.getTexture());
}


void Button::draw(sf::RenderWindow* window)
{
    this->drawRenderTexture();
    window->draw(this->sprite);
    //window->draw(this->text);
}
void Button::draw(sf::RenderTexture* renderTexturePtr)
{
    this->drawRenderTexture();
    renderTexturePtr->draw(this->sprite);
    //renderTexturePtr->draw(this->text);
}
