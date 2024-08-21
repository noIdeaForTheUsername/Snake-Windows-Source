#include "Game.h"

// All variables written with capital letters are constants taken from SETTINGS.h

using namespace std;

Game::Game()
{
    this->gameState = GameState::Loading;
    this->loadSuccessful = true;
    std::srand(time(0));

    this->initVariables();
    this->initAssets();
    this->initWindow();
    this->initMenu();
    this->initText();
    this->initBottomBar();
    this->initBoard();
}

Game::~Game()
{
    delete this->board, this->startText, this->gameOverText, &this->defaultFont,
        this->window, & this->videoMode, & this->ev, this->bottomBar;
}

void Game::initVariables() {
    this->points = 0;
    this->window = nullptr;
    this->gameActiveFramesCount = 0;
    this->minPauseFrames = round(MIN_PAUSE_TIME * FPS);
    this->musicFadeOutFrames = round(MUSIC_FADEOUT_TIME * FPS);
    this->musicFadeInFrames = round(MUSIC_FADEIN_TIME * FPS);
}

void Game::initAssets() {
    this->gameMusicPtr = new sf::Music();
    this->loadSuccessful = this->loadSuccessful && this->defaultFont.loadFromFile("Fonts/04B_30__.TTF")
        && this->gameMusicPtr->openFromFile(this->musicDirectories[0])
        && this->menuMusic.openFromFile("Audio/menuMusic.mp3")
        && this->buttonHoverSoundBuffer.loadFromFile("Audio/buttonHover.mp3")
        && this->buttonClickSoundBuffer.loadFromFile("Audio/buttonClick.mp3")
        && this->achievementSoundBuffer.loadFromFile("Audio/achievement.mp3")
        && this->buttonDisabledClickSoundBuffer.loadFromFile("Audio/lockedButtonClick.mp3")
        && this->lockedAchievementLabelTexture.loadFromFile("Textures/achievementLabelLocked.png")
        && this->unlockedAchievementLabelTexture.loadFromFile("Textures/achievementLabelUnlocked.png")
        && this->controlsInfoTexture.loadFromFile("Textures/controls.png")
        && this->bsodTexture.loadFromFile("Textures/sad.png");

    if (this->loadSuccessful) {
        this->gameMusicPtr->setLoop(true);
        this->gameMusicPtr->setPitch(DEFAULT_MUSIC_PITCH);
        this->gameMusicPtr->setVolume(DEFAULT_VOLUME);
        this->buttonHoverSound.setBuffer(this->buttonHoverSoundBuffer);
        this->buttonHoverSound.setVolume(DEFAULT_VOLUME);
        this->buttonClickSound.setBuffer(this->buttonClickSoundBuffer);
        this->buttonClickSound.setVolume(DEFAULT_VOLUME);
        this->buttonDisabledClickSound.setBuffer(this->buttonDisabledClickSoundBuffer);
        this->buttonDisabledClickSound.setVolume(DEFAULT_VOLUME);
        this->achievementSound.setBuffer(this->achievementSoundBuffer);
        this->achievementSound.setVolume(DEFAULT_VOLUME);

        this->bsodSprite.setTexture(this->bsodTexture);
        sf::FloatRect spriteSize = this->bsodSprite.getLocalBounds();
        this->bsodSprite.setScale(BOARD_WIDTH / spriteSize.width, BOARD_HEIGHT / spriteSize.height);

        this->controlsInfoSprite.setTexture(this->controlsInfoTexture);
        spriteSize = this->controlsInfoSprite.getLocalBounds();
        float scale = (BOARD_WIDTH * 0.7f) / spriteSize.width;
        this->controlsInfoSprite.setScale(scale, scale);
        this->controlsInfoSprite.setOrigin(spriteSize.width / 2.f, spriteSize.height / 2.f);
        this->controlsInfoSprite.setPosition(BOARD_WIDTH / 2.f, BOARD_HEIGHT * 0.75f);
        this->controlsInfoSprite.setColor(sf::Color(255, 255, 255, 200));
    }
}

void Game::initWindow() {
    this->videoMode.width = WINDOW_WIDTH;
    this->videoMode.height = WINDOW_HEIGHT;
    
    this->window = new sf::RenderWindow(this->videoMode, "Snake", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(FPS);
    if (this->windowIcon.loadFromFile("Textures/icon.png")) this->window->setIcon(this->windowIcon.getSize().x, this->windowIcon.getSize().y, this->windowIcon.getPixelsPtr()); // The game will still run even if the icon doesn't load
}

void Game::initMenu() {
    this->menuObject = new Menu(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Vector2f(0, 0), &this->defaultFont,
        &this->unlockedAchievementLabelTexture, &this->lockedAchievementLabelTexture, this->gameMusicTracksNumber,
        &this->buttonHoverSound, &this->buttonClickSound, &this->buttonDisabledClickSound);
    this->getSettingsList();
}

void Game::initBoard() {
    this->board = new Board(BOARD_WIDTH, BOARD_HEIGHT, this->gameSettingsList.boardRows, this->gameSettingsList.boardColumns,
        this->gameSettingsList.tileColor1, this->gameSettingsList.tileColor2);
    this->board->placeSnake(SNAKE_START_LENGTH, FacingDirection::Right,
        this->gameSettingsList.snakeColorBegin, this->gameSettingsList.snakeColorEnd, this->gameSettingsList.minSnakeGradientLength,
        this->gameSettingsList.rainbowSnake, this->gameSettingsList.oppositeGradientColor, this->gameSettingsList.emergencySnake);
    this->board->setSnakeColorRandomization(this->gameSettingsList.randomizeSnakeBeginColor, this->gameSettingsList.randomizeSnakeEndingColor);
    for (int i = 0; i < this->gameSettingsList.fruitQuantity; i++) {
        this->board->placeNewFruit(this->gameSettingsList.fruitTextureIndex);
    }
    this->board->setSnakeLengthening(this->gameSettingsList.snakeLengthening);
    this->board->setGamemode(this->gameSettingsList.gamemode, this->gameSettingsList.boardUpdateFrames, &this->defaultFont);
    this->board->setCheckTimeloopAchievement(this->gameSettingsList.checkTimeloopAchievement);
}

void Game::initBottomBar() {
    this->bottomBar = new BottomBar(WINDOW_WIDTH, WINDOW_HEIGHT - BOARD_HEIGHT, sf::Vector2f(0, BOARD_HEIGHT), &this->defaultFont,
        &this->unlockedAchievementLabelTexture, &this->lockedAchievementLabelTexture, &this->achievementSound);
    this->bottomBar->setButtonSounds(&this->buttonHoverSound, &this->buttonClickSound);
}

void Game::initText() {
    this->startText = new sf::Text();
    this->startText->setFont(this->defaultFont);
    this->startText->setString("PRESS ANY KEY\n   TO START");
    this->startText->setCharacterSize(60);
    this->startText->setFillColor(sf::Color::White);
    this->startText->setPosition((BOARD_WIDTH / 2) - this->startText->getGlobalBounds().width / 2, (BOARD_HEIGHT / 2) - this->startText->getGlobalBounds().height / 2);

    this->gameOverText = new sf::Text();
    this->gameOverText->setFont(this->defaultFont);
    this->gameOverText->setString("GAME OVER");
    this->gameOverText->setCharacterSize(80);
    this->gameOverText->setFillColor(sf::Color::Red);
    this->gameOverText->setPosition((BOARD_WIDTH / 2) - this->gameOverText->getGlobalBounds().width / 2, (BOARD_HEIGHT / 2) - this->gameOverText->getGlobalBounds().height / 2);
}

bool Game::getAssetsLoaded() {
    return this->loadSuccessful && this->board->getLoadSuccessful() && this->bottomBar->getLoadSuccessful()
        && this->menuObject->getLoadSuccessful();
}

void Game::reloadGameMusic(int index)
{
    this->gameMusicPtr->stop();
    delete this->gameMusicPtr;
    this->gameMusicPtr = new sf::Music();
    this->loadSuccessful = this->loadSuccessful && this->gameMusicPtr->openFromFile(this->musicDirectories[index]);
    this->gameMusicPtr->setVolume(DEFAULT_VOLUME);
}




void Game::processGame()
{
    this->loadSuccessful = this->getAssetsLoaded();
    if (!this->loadSuccessful) {
        std::string errorMessage = "A loading error has occured. The game start has been interrupted. This is most likely caused by missing "
            "or inaccessible asset files. Make sure all assets are in their directories (Textures, Fonts, Audio) or reinstall the game.";
        this->stopGame();
        throw std::runtime_error(errorMessage);
    }
    this->gameState = GameState::Menu;
    
    

    // MAIN GAME LOOP
    while (this->window->isOpen())
    { 
        if (!this->loadSuccessful) {
            std::string errorMessage = "A loading error has occured. This is most likely caused by missing or inaccessible asset files - "
                "audio files are most likely in this case. The game window has been closed. "
                "Make sure all assets are in their directories (Textures, Fonts, Audio) or reinstall the game.";
            this->stopGame();
            throw std::runtime_error(errorMessage);
        }

        // Frame counters
        this->handleFrameCounters();

        // Music
        this->handleMusic();

        //Events
        this->handleEvents();

        // Update
        this->update();

        // Window drawing
        this->drawWindow();
    }

    if (this->menuObject != nullptr) this->menuObject->saveData();

}

// FUNCTIONS CALLED EVERY FRAME

void Game::handleEvents() {
    sf::Event ev;
    while (this->window->pollEvent(ev))
    {
        switch (ev.type)
        {
        case sf::Event::Closed:
            this->stopGame();
            break;
        case sf::Event::KeyPressed:
            // Keys being pressed
            handleKeyPressed(ev.key);
            break;
        case sf::Event::MouseButtonPressed:
            if (this->gameState == GameState::Paused || this->gameState == GameState::Over || this->gameState == GameState::Active) {
                if (!this->endingSequenceActive && this->bottomBar->getMenuButtonClicked(this->window)) {
                    this->enterMenu();
                }
            }
            else if (this->gameState == GameState::Menu) {
                this->menuObject->checkButtonClicks(this->window, ev.mouseButton.button);
                if (this->menuObject->getPlayGame()) this->exitMenu();
            }
        }
    }
}

void Game::handleKeyPressed(sf::Event::KeyEvent key)
{
    switch (this->gameState) {
    case GameState::Active:
        switch (key.scancode) {
        case sf::Keyboard::Scan::Right:
        case sf::Keyboard::Scan::D:
            this->board->updateSnakeFacingDirection(FacingDirection::Right);
            break;
        case sf::Keyboard::Scan::Left:
        case sf::Keyboard::Scan::A:
            this->board->updateSnakeFacingDirection(FacingDirection::Left);
            break;
        case sf::Keyboard::Scan::Down:
        case sf::Keyboard::Scan::S:
            this->board->updateSnakeFacingDirection(FacingDirection::Down);
            break;
        case sf::Keyboard::Scan::Up:
        case sf::Keyboard::Scan::W:
            this->board->updateSnakeFacingDirection(FacingDirection::Up);
            break;
        }
        break;

    case GameState::Paused:
        if (this->pauseFramesCount >= this->minPauseFrames) {
            this->startGame();
        }
        break;

    case GameState::Over:
        if (this->pauseFramesCount >= this->minPauseFrames) {
            this->resetGame();
            this->startGame();
        }
        break;

    default:
        break;
    }
}


void Game::handleMusic() {
    switch (this->gameState) {
    case GameState::Active:
    case GameState::Paused:
        if (this->menuMusic.getStatus() == sf::SoundSource::Status::Playing) this->menuMusic.stop();
        if (this->gameMusicPtr->getStatus() != sf::SoundSource::Status::Playing) this->gameMusicPtr->play();

        if (this->endingSequenceActive && this->endingSequenceFramesCount < this->maxEndingSequenceFrames) {
            int rewindsPerSecond = 5;
            if (this->endingSequenceFramesCount % (FPS / rewindsPerSecond) == 0) {
                if (this->gameMusicPtr->getPlayingOffset().asSeconds() < 1.f / rewindsPerSecond) {
                    this->gameMusicPtr->setPlayingOffset(sf::seconds(0.f));
                }
                else {
                    this->gameMusicPtr->setPlayingOffset(sf::seconds(this->gameMusicPtr->getPlayingOffset().asSeconds() - 1.f / rewindsPerSecond));
                }
            }
        }
        else if (this->endingSequenceActive && this->endingSequenceFramesCount >= this->maxEndingSequenceFrames) {
            this->gameMusicPtr->setVolume(20.f);
            this->gameMusicPtr->setPitch(100.f);
        }
        else if ((this->gameMusicPtr->getPitch() < DEFAULT_MUSIC_PITCH || round(this->gameMusicPtr->getVolume()) < round(DEFAULT_VOLUME))
            && this->gameActiveFramesCount < this->musicFadeInFrames) {
            this->gameMusicPtr->setPitch(MUSIC_PITCH_ON_PAUSE -  // Fade-in
                (MUSIC_PITCH_ON_PAUSE - DEFAULT_MUSIC_PITCH) * ((float)this->gameActiveFramesCount / this->musicFadeInFrames));
            this->gameMusicPtr->setVolume(MUSIC_VOLUME_ON_PAUSE -
                (MUSIC_VOLUME_ON_PAUSE - DEFAULT_VOLUME) * ((float)this->gameActiveFramesCount / this->musicFadeInFrames));
        }
        else {
            this->gameMusicPtr->setPitch(DEFAULT_MUSIC_PITCH);
            this->gameMusicPtr->setVolume(DEFAULT_VOLUME);
        }
        break;
    case GameState::Menu:
        // Menu music
        if (this->menuObject->getCurrentMusicPreviewIndex() == -1) {
            this->musicPreviewFramesCount = 0;
            if (this->menuMusic.getStatus() == sf::SoundSource::Status::Playing) {
                if (this->menuFramesCount < this->musicFadeInFrames) {
                    this->menuMusic.setVolume(DEFAULT_VOLUME * ((float)this->menuFramesCount / this->musicFadeInFrames));
                }
                else {
                    this->menuMusic.setVolume(DEFAULT_VOLUME);
                }
            }
            else {
                this->menuMusic.setLoop(true);
                this->menuMusic.play();
                this->menuMusic.setVolume(0.f);
                this->menuFramesCount = 0;
            }

        }
        else {  // Game music preview
            if (this->menuMusic.getStatus() == sf::SoundSource::Status::Playing) {
                // Fade-out
                if (this->musicPreviewFramesCount < this->musicFadeOutFrames) {
                    this->menuMusic.setVolume(DEFAULT_VOLUME -
                        DEFAULT_VOLUME * ((float)this->musicPreviewFramesCount / this->musicFadeOutFrames));
                }
                else {
                    this->menuMusic.pause();
                }
            }
            
            if (this->gameMusicPtr->getStatus() == sf::SoundSource::Status::Playing) {
                if (this->musicPreviewFramesCount < this->musicFadeInFrames) { // Fade-in
                    this->gameMusicPtr->setVolume(DEFAULT_VOLUME * ((float)this->musicPreviewFramesCount / this->musicFadeInFrames));
                }
                else if (this->gameMusicPtr->getPlayingOffset().asSeconds() > this->menuObject->getCurrentMusicPreviewEndOffset()) {  // Restart
                    this->gameMusicPtr->setVolume(0.f);
                    this->gameMusicPtr->setPlayingOffset(sf::seconds(this->menuObject->getCurrentMusicPreviewBeginOffset()));
                    this->musicPreviewFramesCount = 0;
                }
                else if (this->gameMusicPtr->getPlayingOffset().asSeconds() > this->menuObject->getCurrentMusicPreviewEndOffset() - MUSIC_FADEOUT_TIME) {  // Fade-out
                    this->gameMusicPtr->setVolume(DEFAULT_VOLUME -
                        DEFAULT_VOLUME * (( (float)this->gameMusicPtr->getPlayingOffset().asSeconds() - (this->menuObject->getCurrentMusicPreviewEndOffset() - MUSIC_FADEOUT_TIME))
                        / MUSIC_FADEOUT_TIME));
                }
                else if (this->gameMusicPtr->getVolume() != DEFAULT_VOLUME) {
                    this->gameMusicPtr->setVolume(DEFAULT_VOLUME);
                }
            }
            else {
                this->reloadGameMusic(this->menuObject->getCurrentMusicPreviewIndex());
                this->gameMusicPtr->setLoop(false);
                this->gameMusicPtr->setVolume(0.f);
                this->gameMusicPtr->play();
                this->gameMusicPtr->setPlayingOffset(sf::seconds(this->menuObject->getCurrentMusicPreviewBeginOffset()));
            }

            this->musicPreviewFramesCount++;
        }
        
        // Purposefully without break;
    case GameState::Over:  // AND GameState::Menu
        // Game music
        if (this->gameMusicPtr->getStatus() == sf::SoundSource::Status::Playing && this->menuObject->getCurrentMusicPreviewIndex() == -1) {
            if ((this->gameMusicPtr->getPitch() > MUSIC_PITCH_ON_PAUSE || this->gameMusicPtr->getVolume() > MUSIC_VOLUME_ON_PAUSE)
                && this->pauseFramesCount < this->musicFadeOutFrames) {
                // Fade-out
                this->gameMusicPtr->setPitch(DEFAULT_MUSIC_PITCH -
                    (DEFAULT_MUSIC_PITCH - MUSIC_PITCH_ON_PAUSE) * ((float)this->pauseFramesCount / this->musicFadeOutFrames));
                this->gameMusicPtr->setVolume(DEFAULT_VOLUME -
                    (DEFAULT_VOLUME - MUSIC_VOLUME_ON_PAUSE) * ((float)this->pauseFramesCount / this->musicFadeOutFrames));
            }
            else {
                this->gameMusicPtr->pause();
            }
        }
        break;
    }
}

void Game::handleFrameCounters() { // Music preview frame counter is handled separetely, in the handleMusic() function 
    if (this->gameState == GameState::Active) this->gameActiveFramesCount++;
    else if (this->gameState == GameState::Over || this->gameState == GameState::Paused || this->gameState == GameState::Menu) this->pauseFramesCount++;
    if (this->gameState == GameState::Menu) this->menuFramesCount++;

    if (this->endingSequenceActive) {
        if (this->endingSequenceFramesCount < this->maxEndingSequenceFrames) this->endingSequenceFramesCount++;
        else this->bsodWaitingFrames++;
    }  
}

void Game::update()
{
    if (this->gameState == GameState::Active && !this->endingSequenceActive) {
        this->timer = (float)gameActiveFramesCount / FPS;

        if (gameActiveFramesCount % this->gameSettingsList.boardUpdateFrames == 0 && !this->paused) {
            this->board->moveSnake();
            this->points = this->board->getPoints();
        }
        if (this->board->getGameOver()) this->gameOver();

        if (this->board->getBoardFull()) {
            this->endingSequenceActive = true;
        }
    }

    if (this->bsodWaitingFrames >= this->maxBsodWaitingFrames) {
        // Restart the game after the secret ending
        this->exitMenu();
        this->gameCompletions++;
    }

    if (this->gameState == GameState::Active || this->gameState == GameState::Over || this->gameState == GameState::Paused) {
        this->bottomBar->update(this->window, this->points);
        this->bottomBar->updateTimer(this->timer);
        if (this->gameSettingsList.gamemode == Gamemode::TimeTravel) {
            this->bottomBar->updateTimeTravelText(this->gameSettingsList.boardUpdateFrames, this->board->getMoveCounter());
        }
        this->board->update(this->gameActiveFramesCount);

        // Achievements unlocking
        GameData data = this->board->getGameData();
        data.timer = this->timer;
        data.unlockErrorAchievement = this->gameCompletions > 0;
        string unlockedAchievementName = this->menuObject->checkAchievementUnlocks(data);
        if (unlockedAchievementName.length() > 0) {
            this->bottomBar->displayAchievementNotification(unlockedAchievementName, ACHIEVEMENT_NOTIFCATION_DISPLAY_TIME);
            this->menuObject->reloadAchievementStatus();
        }
    }

    if (this->gameState == GameState::Menu) {
        this->menuObject->update(this->window);
    }
}

void Game::drawWindow()
{
    this->window->clear(sf::Color(0, 0, 150));
    this->bottomBar->draw(this->window);
    if (this->gameState == GameState::Active || this->gameState == GameState::Paused || this->gameState == GameState::Over) {
        if (this->endingSequenceFramesCount < this->maxEndingSequenceFrames)
            this->board->draw(*this->window);
        else
            this->window->draw(this->bsodSprite);
    }
    if (this->gameState == GameState::Paused) {
        this->window->draw(this->controlsInfoSprite);
        this->window->draw(*this->startText);
    }
    else if (this->gameState == GameState::Over) this->window->draw(*this->gameOverText);
    else if (this->gameState == GameState::Menu) this->menuObject->draw(*this->window);
    this->window->display();
}

void Game::enterMenu() { 
    this->resetGame();
    this->menuObject->reloadAchievementStatus();
    this->gameCompletions = 0;
    this->menuFramesCount = 0;
    this->menuObject->setCurrentPage(MenuPage::Main);
    this->gameState = GameState::Menu;
}

void Game::getSettingsList()
{
    this->gameSettingsList = this->menuObject->getGameSettings();
}

void Game::exitMenu()
{
    this->resetGame();
    this->gameMusicPtr->stop();
    this->reloadGameMusic(this->gameSettingsList.musicIndex);
    this->gameMusicPtr->setPitch(DEFAULT_MUSIC_PITCH);
    this->gameMusicPtr->setVolume(DEFAULT_VOLUME);
    this->bottomBar->resetAchievementNotification();
    if (this->gameSettingsList.tileColor2.r + this->gameSettingsList.tileColor2.g + this->gameSettingsList.tileColor2.b < 50)
        // Solution for the black bar being too dark
        this->bottomBar->setColor(sf::Color(this->gameSettingsList.tileColor2.r + 70, this->gameSettingsList.tileColor2.g + 70, this->gameSettingsList.tileColor2.b + 70));
    else
        this->bottomBar->setColor(this->gameSettingsList.tileColor2);
    this->pauseFramesCount = 0;
    this->menuFramesCount = 0;
    this->gameState = GameState::Paused;
}

void Game::gameOver() { 
    this->gameState = GameState::Over;
}

void Game::resetGame() {
    delete this->board;
    this->points = 0;
    this->bottomBar->update(this->window, 0);
    this->getSettingsList();
    this->initBoard();
    this->bottomBar->setTimeTravelModeTextVisibility(this->gameSettingsList.gamemode == Gamemode::TimeTravel);
    this->bottomBar->updateTimer(0.f);
    this->timer = 0.f;
    this->endingSequenceActive = false;
    this->endingSequenceFramesCount = 0;
    this->bsodWaitingFrames = 0;
    this->gameActiveFramesCount = 0;
}

void Game::startGame()
{
    this->gameState = GameState::Active;
    this->pauseFramesCount = 0;
}

void Game::stopGame()
{
    delete this->gameMusicPtr;
    this->window->close();
}
