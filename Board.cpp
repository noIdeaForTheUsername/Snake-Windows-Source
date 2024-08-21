#include "Board.h"


Board::Board(float boardWidth, float boardHeight, int boardRows, int boardColumns, sf::Color tileColor1, sf::Color tileColor2)
    : renderTexture()
{
    this->loadSuccessful = true;
    // Size parameters
    this->width = boardWidth;
    this->height = boardHeight;
    this->rows = boardRows;
    this->columns = boardColumns;
    this->tileWidth = this->width / this->columns;
    this->tileHeight = this->height / this->rows;

    this->snakeLengthening = FRUIT_COLLECTION_LENGTHENING;
    this->tileColor1 = tileColor1;
    this->tileColor2 = tileColor2;

    this->isGameOver = false;
    this->isBoardFull = false;
    this->points = 0;

    this->tiles = new Tile**[this->columns];
    for (int i = 0; i < columns; i++) {
        tiles[i] = new Tile*[this->rows];
    }

    if (!Fruit::assetsLoaded) Fruit::loadAssets();
    if (!TileIndicator::assetsLoaded) TileIndicator::loadAssets();

    this->initTiles();
}

Board::~Board()
{
    for (int i = 0; i < this->columns; ++i) {
        for (int j = 0; j < this->rows; ++j) {
            delete this->tiles[i][j];
        }
        delete[] this->tiles[i];
    }
    delete[] this->tiles;
    delete this->snake;
    for (auto obj : this->fruits) {
        delete obj;
    }
    this->fruits.clear();
}

void Board::initTiles()
{
    if (!Tile::assetsLoaded) Tile::loadAssets();
    this->loadSuccessful = Tile::assetsLoaded;

    for (int x = 0; x < this->columns; x++) {
        for (int y = 0; y < this->rows; y++) {
            float topX = this->tileWidth * x;
            float topY = this->tileHeight * y;
            if ((x + y) % 2 == 0) {
                this->tiles[x][y] = new Tile(topX, topY, this->tileWidth, this->tileHeight, this->tileColor1);
            }
            else {
                this->tiles[x][y] = new Tile(topX, topY, this->tileWidth, this->tileHeight, this->tileColor2);
            }
        }
    }
}




bool Board::getLoadSuccessful() {
    if (this->snake != nullptr) this->loadSuccessful = this->loadSuccessful && this->snake->getLoadSuccessful();
    this->loadSuccessful = this->loadSuccessful && Fruit::assetsLoaded && Tile::assetsLoaded && TileIndicator::assetsLoaded;
    return this->loadSuccessful;
}

void Board::setSnakeLengthening(int tilesPerFruit)
{
    this->snakeLengthening = tilesPerFruit;
}

void Board::setCheckTimeloopAchievement(bool value) {
    this->checkTimeloopAchievement = value;
    if (!value) {
        this->timepointsVisitCounters.clear();
    }
}

void Board::setGamemode(Gamemode gamemode, int boardUpdateFrames, sf::Font* fontPtr) {
    this->gamemode = gamemode;
    this->maxSnakeMovementPauseUpdates = ceil((float)FPS * SNAKE_MOVEMENT_PAUSE_TIME / (float)boardUpdateFrames);
    switch (this->gamemode) {
    case Gamemode::Invisible:
        Tile::setSnakeInvisibility(true, 3);
        break;
    case Gamemode::MovingFruits:
        for (auto fruit : this->fruits) {
            fruit->setRandomMoveVector();
        }
        break;
    case Gamemode::Portal:
        if (this->fruits.size() > 1) {
            int i = 1;
            int textureIndex = (*this->fruits.begin())->textureIndex;
            for (auto it = this->fruits.begin(); it != this->fruits.end(); ) {
                if (i <= 5) {
                    (*it)->setPortalNumber(i, iconColors[i - 1]);
                    i++;
                    ++it;
                }
                else {  // Max amount of fruits in the portal mode is set to 5 (fruits are then duplicated so it will give in total 10, 5 pairs)
                    this->tiles[(*it)->x][(*it)->y]->removeFruit(false);
                    delete *it;
                    it = fruits.erase(it);
                    i = 6;
                }
            }
            for (int j = 1; j < i; j++) {
                Fruit* newFruit = this->placeNewFruit(textureIndex);
                newFruit->setPortalNumber(j, this->iconColors[j - 1]);
            }
        }
        else {
            (*this->fruits.begin())->setPortalNumber(1, sf::Color(0, 0, 0, 0));
            Fruit* newFruit = this->placeNewFruit((*this->fruits.begin())->textureIndex);
            newFruit->setPortalNumber(1, sf::Color(0, 0, 0, 0));
        }
        break;
    case Gamemode::TimeTravel:
        this->snakeMovesPerSecond = (float)FPS / boardUpdateFrames;
        this->moveCounter = 0;
        this->presentPoint = -1;
        // Set timeTravelNumbers for each fruit and remove some fruits if there are more than 5
        if (this->fruits.size() > 1) {
            int i = 1;
            for (auto it = this->fruits.begin(); it != this->fruits.end(); ) {
                if (i <= 5) {
                    (*it)->setTimeTravelNumber(i, iconColors[i - 1], fontPtr);
                    i++;
                    ++it;
                }
                else {  // Max amount of fruits in the time travel mode is set to 5
                    this->tiles[(*it)->x][(*it)->y]->removeFruit(false);
                    delete* it;
                    it = fruits.erase(it);
                    i = 6;
                }
            }
            this->saveBoardSnakeLengths(0);
            for (int j = 1; j < i; j++) {
                this->reassignTimeTravelDestinationToFruit(j);
                this->replaceTimeTravelIndicators(j);
            }
            
        }
        else {
            (*this->fruits.begin())->setTimeTravelNumber(1, sf::Color(0, 0, 0, 0), fontPtr);
            this->saveBoardSnakeLengths(0);
            this->reassignTimeTravelDestinationToFruit(1);
            this->replaceTimeTravelIndicators(1);
        }
        break;
    }

    if (this->gamemode != Gamemode::Invisible) Tile::setSnakeInvisibility(false);

    this->updateTileColors();
}

bool Board::placeSnake(int startLength, FacingDirection facingDirection,
    sf::Color beginColor, sf::Color endingColor, int minGradientLength, bool rainbow, bool oppositeGradientColor, bool emergencyColors)
{
    sf::Vector2f snakeHeadSize = sf::Vector2f(this->tileWidth, this->tileHeight);
    FacingDirection snakeFacingDirection = facingDirection;
    int snakeHeadX;
    int snakeHeadY;
    switch (snakeFacingDirection) {
    default:
    case FacingDirection::Right:
        snakeHeadX = startLength - 1;
        snakeHeadY = std::round(((float)this->rows / 2)) - 1;
        break;
    case FacingDirection::Left:
        snakeHeadX = this->columns - startLength;
        snakeHeadY = std::round(((float)this->rows / 2)) - 1;
        break;
    case FacingDirection::Up:
        snakeHeadX = std::round(((float)this->columns / 2)) - 1;
        snakeHeadY = this->rows - startLength;
        break;
    case FacingDirection::Down:
        snakeHeadX = std::round(((float)this->columns / 2)) - 1;
        snakeHeadY = startLength - 1;
        break;
    }

    bool gradientEnabled = beginColor != endingColor;
    this->snake = new Snake(snakeHeadSize, snakeHeadX, snakeHeadY, startLength, snakeFacingDirection, minGradientLength, gradientEnabled);
    this->snake->setColors(beginColor, endingColor);
    if (rainbow) this->snake->setRainbow();
    else if (emergencyColors) this->snake->setEmergencyColors();
    if (oppositeGradientColor) this->snake->setOppositeGradientColor();

    int targetTileX = this->snake->headX;
    int targetTileY = this->snake->headY;
    //FacingDirection oppositeSnakeFacingDirection = this->snake.getOppositeFacingDirection(this->snake.facingDirection);
    for (int i = 0; i < this->snake->length; i++) {
        switch (this->snake->facingDirection) {
        default:
        case FacingDirection::Right:
            targetTileX = i;
            targetTileY = this->snake->headY;
            break;
        case FacingDirection::Left:
            targetTileX = this->columns - 1 - i;
            targetTileY = this->snake->headY;
            break;
        case FacingDirection::Up:
            targetTileX = this->snake->headX;
            targetTileY = this->rows - 1 - i;
            break;
        case FacingDirection::Down:
            targetTileX = this->snake->headX;
            targetTileY = i;
            break;
        }
        if (this->checkCoordinatesInBoardRange(targetTileX, targetTileY)) tiles[targetTileX][targetTileY]->placeSnakeTile(i + 1, this->snake->length, this->snake->headColor, this->snake->endingColor);
        else {
            string errorMessage = "Loading error - given snake length (source files: SETTINGS.h, Game::gameSettingsList) along with "
                "the board size doesn't allow for correct snake placement. This error should never appear if the source code hasn't been modified. "
                "(dev note: if it somehow did then, well... I don't know what have you done with the game...)";
            MessageBoxA(NULL, errorMessage.c_str(), "Loading error", MB_ICONERROR | MB_OK);
            this->loadSuccessful = false;
            return false;
        }
    }
    return true;
}

void Board::setSnakeColorRandomization(bool beginColor, bool endColor)
{
    this->snake->setRandomizeColors(beginColor, endColor);
}




Fruit* Board::placeNewFruit(int textureIndex)
{
    Fruit* fruitPtr = new Fruit(this->tileWidth, this->tileHeight, textureIndex);
    this->fruits.push_back(fruitPtr);
    bool placedCorrectly = false;
    do {
        sf::Vector2i fruitPosition = fruitPtr->moveToRandomPosition(this->columns - 1, this->rows - 1);
        placedCorrectly = tiles[fruitPosition.x][fruitPosition.y]->placeFruitTile(fruitPtr);
    } while (!placedCorrectly);
    return fruitPtr;
}

void Board::moveFruitToRandomPosition(Fruit* fruitToMovePtr)
{
    // The function doesn't remove the Fruit status from the Tile the Fruit was previously placed on, but it adds the Fruit status to the Tile at the new Fruit position
    bool placedCorrectly = false;

    bool spaceLeft = this->getTileNumberByStatus(TileStatus::Empty) > 0;
    if (!spaceLeft) {
        for (auto it = this->fruits.begin(); it != this->fruits.end(); it++) {
            if ((*it) == fruitToMovePtr) {
                it = this->fruits.erase(it);
                delete fruitToMovePtr;
                return;
            }
        }
    }

    int extraAttempts = 0;
    do {
        sf::Vector2i fruitPosition = fruitToMovePtr->moveToRandomPosition(this->columns - 1, this->rows - 1);
        
        if (this->gamemode == Gamemode::Portal) {
            placedCorrectly = this->tiles[fruitPosition.x][fruitPosition.y]->status == TileStatus::Empty;
            if (placedCorrectly) {
                placedCorrectly = floor(extraAttempts / 50.f) >= PORTAL_MODE_MINIMUM_FRUIT_DISTANCE
                    || this->checkPortalModeFruitPositionCorrect(fruitPosition.x, fruitPosition.y, PORTAL_MODE_MINIMUM_FRUIT_DISTANCE - floor(extraAttempts / 50.f));

                if (placedCorrectly) this->tiles[fruitPosition.x][fruitPosition.y]->placeFruitTile(fruitToMovePtr);
                extraAttempts++;
            }
        }
        else if (this->gamemode == Gamemode::Wall &&
            this->getTileNumberByStatus(TileStatus::Empty) > 2 && this->getFreeSpacesAroundTile(fruitPosition.x, fruitPosition.y) < 2) {
            placedCorrectly = false;
        }
        else {
            placedCorrectly = tiles[fruitPosition.x][fruitPosition.y]->placeFruitTile(fruitToMovePtr);
        }
    } while (!placedCorrectly);

    if (this->gamemode == Gamemode::MovingFruits) {
        fruitToMovePtr->setRandomMoveVector();
    }
}

bool Board::placeWallTile(int x, int y)
{
    // Checking if there's wall at tiles around
    if (this->checkCoordinatesInBoardRange(x - 1, y - 1) && this->tiles[x - 1][y - 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x, y - 1) && this->tiles[x][y - 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x + 1, y - 1) && this->tiles[x + 1][y - 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x + 1, y) && this->tiles[x + 1][y]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x + 1, y + 1) && this->tiles[x + 1][y + 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x, y + 1) && this->tiles[x][y + 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x - 1, y + 1) && this->tiles[x - 1][y + 1]->markedForWallPlacement) return false;
    if (this->checkCoordinatesInBoardRange(x - 1, y) && this->tiles[x - 1][y]->markedForWallPlacement) return false;

    // If the tile is on the edge of the board, check if there's wall 2 spaces further, and check if the edge is 2 spaces further
    if (x == 0 || x == this->columns - 1) {
        if (!this->checkCoordinatesInBoardRange(x, y + 2) || this->tiles[x][y + 2]->markedForWallPlacement) return false;
        if (!this->checkCoordinatesInBoardRange(x, y - 2) || this->tiles[x][y - 2]->markedForWallPlacement) return false;
        if (!this->checkCoordinatesInBoardRange(x, y - 3)) return false;
        if (!this->checkCoordinatesInBoardRange(x, y + 3)) return false;
    }
    if (y == 0 || y == this->rows - 1) {
        if (!this->checkCoordinatesInBoardRange(x + 2, y) || this->tiles[x + 2][y]->markedForWallPlacement) return false;
        if (!this->checkCoordinatesInBoardRange(x - 2, y) || this->tiles[x - 2][y]->markedForWallPlacement) return false;
        if (!this->checkCoordinatesInBoardRange(x - 3, y)) return false;
        if (!this->checkCoordinatesInBoardRange(x + 3, y)) return false;
    }

    this->tiles[x][y]->markForWallPlacement();
    return true;
}

void Board::invertSnake()
{
    if (this->snake == nullptr) return;
    sf::Vector2i maxSnakeRemainingTile(0, 0);
    int maxSnakeRemaining = 0;
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < rows; j++) {
            if (this->tiles[i][j]->status == TileStatus::Snake) {
                this->tiles[i][j]->snakeRemaining = (this->snake->length - this->tiles[i][j]->snakeRemaining) + 1;

                // Find tile with max snakeRemaining value
                if (tiles[i][j]->snakeRemaining > maxSnakeRemaining) {
                    maxSnakeRemainingTile = sf::Vector2i(i, j);
                    maxSnakeRemaining = this->tiles[i][j]->snakeRemaining;
                }
            }
        }
    }
    if (this->tiles[maxSnakeRemainingTile.x][maxSnakeRemainingTile.y]->snakeRemaining < this->snake->length) {
        // If the snake didn't have enough time to increase its actual occupied tiles to its new length
        this->lengthenSnakeOnTiles(this->snake->length - maxSnakeRemaining);
        maxSnakeRemaining = this->snake->length;
    }

    // Get the new facing direction
    this->snake->teleportHead(maxSnakeRemainingTile.x, maxSnakeRemainingTile.y,
        this->getSnakeFacingDirectionByHeadTile(maxSnakeRemainingTile.x, maxSnakeRemainingTile.y, false));

    this->updateTileColors();

    // Check if the player would die in the next 2 moves - if yes, pause the snake movement for a while
    if (this->checkGameOverInXMoves(2)) {
        this->snakeMovementPauseUpdates = this->maxSnakeMovementPauseUpdates;
        this->snake->setPausedTexture(true);
    }
}


void Board::checkFruitPositionsCorrect()
{
    std::vector<sf::Vector2i> fruitPositions;

    for (auto fruit : this->fruits) {
        for (auto position : fruitPositions) {
            int counter = 0;
            while (!this->checkCoordinatesInBoardRange(fruit->x, fruit->y)
                || (fruit->x == position.x && fruit->y == position.y) || this->tiles[fruit->x][fruit->y]->status != TileStatus::Empty) {
                if (counter < 50) {
                    fruit->x -= fruit->moveVector.x;
                    fruit->y -= fruit->moveVector.y;
                    fruit->moveVector.x *= -1;
                    fruit->moveVector.y *= -1;
                }
                else if (counter < 1000) {
                    this->moveFruitToRandomPosition(fruit);
                    this->tiles[fruit->x][fruit->y]->removeFruit(false);  // The moving function marks the tile status as Fruit - revert this change
                }
                else {
                    for (auto it = this->fruits.begin(); it != this->fruits.end(); ++it) {
                        if ((*it) == fruit) {
                            this->tiles[fruit->x][fruit->y]->removeFruit(false);
                            delete fruit;
                            this->fruits.erase(it);
                            break;
                        }
                    }
                }
                //this->checkFruitPositionsCorrect();
                counter++;
            }
        }
        fruitPositions.push_back(sf::Vector2i(fruit->x, fruit->y));
    }
}

void Board::moveFruits()
{
    for (auto fruit : this->fruits) {
        this->tiles[fruit->x][fruit->y]->removeFruit(false);
        
        // Check if the fruit would be eaten by the Snake - if yes, don't move the fruit anymore (and let it get eaten)
        switch (this->snake->facingDirection) {
        case FacingDirection::Right:
            if (fruit->moveVector.x < 0 && this->snake->headX == fruit->x - 1 && this->snake->headY == fruit->y)
                continue;
            break;
        case FacingDirection::Left:
            if (fruit->moveVector.x > 0 && this->snake->headX == fruit->x + 1 && this->snake->headY == fruit->y)
                continue;
            break;
        case FacingDirection::Up:
            if (fruit->moveVector.y > 0 && this->snake->headX == fruit->x && this->snake->headY == fruit->y + 1)
                continue;
            break;
        case FacingDirection::Down:
            if (fruit->moveVector.y < 0 && this->snake->headX == fruit->x && this->snake->headY == fruit->y - 1)
                continue;
            break;
        }

        // Bouncing
        bool vectorModificationDone = false;
        if (fruit->x + fruit->moveVector.x >= this->columns || fruit->x + fruit->moveVector.x < 0
            || this->tiles[fruit->x + fruit->moveVector.x][fruit->y]->status == TileStatus::Snake
            || this->tiles[fruit->x + fruit->moveVector.x][fruit->y]->status == TileStatus::Wall)
        {
            fruit->moveVector.x *= -1;
            vectorModificationDone = true;
        }
        if (fruit->y + fruit->moveVector.y >= this->rows || fruit->y + fruit->moveVector.y < 0
            || this->tiles[fruit->x][fruit->y + fruit->moveVector.y]->status == TileStatus::Snake
            || this->tiles[fruit->x][fruit->y + fruit->moveVector.y]->status == TileStatus::Wall)
        {
            fruit->moveVector.y *= -1;
            vectorModificationDone = true;
        }

        if (!vectorModificationDone)  // Extra diagonal check
        {
            if (!checkCoordinatesInBoardRange(fruit->x + fruit->moveVector.x, fruit->y + fruit->moveVector.y)
                || this->tiles[fruit->x + fruit->moveVector.x][fruit->y + fruit->moveVector.y]->status == TileStatus::Snake
                || this->tiles[fruit->x + fruit->moveVector.x][fruit->y + fruit->moveVector.y]->status == TileStatus::Wall)
            {
                fruit->moveVector.x *= -1;
                fruit->moveVector.y *= -1;
            }
        }
            
        // Move the fruit
        fruit->x += fruit->moveVector.x;
        fruit->y += fruit->moveVector.y;

        // Revert the movement if the place the fruit has moved to is incorrect
        if (!this->checkCoordinatesInBoardRange(fruit->x, fruit->y)
            || this->tiles[fruit->x][fruit->y]->status == TileStatus::Snake
            || this->tiles[fruit->x][fruit->y]->status == TileStatus::Wall)
        {
            fruit->x -= fruit->moveVector.x;
            fruit->y -= fruit->moveVector.y;
        }
    }
    // Check if the fruits aren't colliding with other fruits
    this->checkFruitPositionsCorrect();
    for (auto fruit : this->fruits) {
        this->tiles[fruit->x][fruit->y]->placeFruitTile(fruit);
    }
}

void Board::teleportSnakeThroughPortal(int portalNumber) {
    for (auto targetPortalFruitPtr : this->fruits) {
        if (targetPortalFruitPtr != nullptr && (!(targetPortalFruitPtr->x == this->snake->headX && targetPortalFruitPtr->y == this->snake->headY)) && targetPortalFruitPtr->portalNumber == portalNumber) {
            this->tiles[targetPortalFruitPtr->x][targetPortalFruitPtr->y]->removeFruit(false);
            this->portals.push_back(new TileIndicator(this->snake->headX, this->snake->headY, this->tileWidth, this->tileHeight, sf::Color(0, 255, 255), ((int)this->snake->facingDirection + 1) * 90));
            this->snake->teleportHead(targetPortalFruitPtr->x, targetPortalFruitPtr->y, this->snake->facingDirection);
            this->lastPortalPlaced = new TileIndicator(this->snake->headX, this->snake->headY, this->tileWidth, this->tileHeight, sf::Color(250, 200, 0), ((int)this->snake->facingDirection - 1) * 90);
            this->portals.push_back(this->lastPortalPlaced);
            this->tiles[this->snake->headX][this->snake->headY]->placeSnakeTile(this->snake->length, this->snake->length, this->snake->headColor, this->snake->endingColor);
            this->moveFruitToRandomPosition(targetPortalFruitPtr);
            break;
        }
    }
}

void Board::checkPortalRotationCorrect() {
    // Update the portal's rotation if the snake has turned just after passing through it
    if (this->snake->facingDirection != this->lastSnakeFacingDirection && this->lastPortalPlaced != nullptr) {
        for (auto portal : this->portals) {
            if (portal == this->lastPortalPlaced) {
                portal->sprite.setRotation(((int)this->snake->facingDirection - 1) * 90);
                break;
            }
        }
    }
}

void Board::removePortalsOnSnakeHead() {
    for (auto it = this->portals.begin(); it != this->portals.end(); ) {
        if ((*it)->x == this->snake->headX && (*it)->y == this->snake->headY) {
            delete (*it);
            it = this->portals.erase(it);
        }
        else ++it;
    }
}

bool Board::checkPortalModeFruitPositionCorrect(int fruitX, int fruitY, int distance)
{
    bool correct = true;

    // Check if the Fruit is placed on the Snake's path, in front of it
    //switch (this->snake->facingDirection) {
    //case FacingDirection::Left:
    //    if (fruitY == this->snake->headY && fruitX >= this->snake->headX - PORTAL_MODE_MINIMUM_FRONT_FRUIT_DISTANCE) correct = false;
    //    break;
    //case FacingDirection::Right:
    //    if (fruitY == this->snake->headY && fruitX <= this->snake->headX + PORTAL_MODE_MINIMUM_FRONT_FRUIT_DISTANCE) correct = false;
    //    break;
    //case FacingDirection::Up:
    //    if (fruitX == this->snake->headX && fruitY >= this->snake->headY - PORTAL_MODE_MINIMUM_FRONT_FRUIT_DISTANCE) correct = false;
    //    break;
    //case FacingDirection::Down:
    //    if (fruitX == this->snake->headX && fruitY <= this->snake->headY + PORTAL_MODE_MINIMUM_FRONT_FRUIT_DISTANCE) correct = false;
    //    break;
    //}

    // Check the minimum about of moves the Snake would be able to reach the Fruit in
    int xDistance = abs(fruitX - this->snake->headX);
    int yDistance = abs(fruitY - this->snake->headY);
    
    if (xDistance + yDistance < distance) {
        correct = false;
    }
    
    return correct;
}

void Board::saveBoardSnakeLengths(int presentPoint) {
    int** snakeLengthsArray = new int* [this->columns];
    for (int i = 0; i < this->columns; ++i) {
        snakeLengthsArray[i] = new int[this->rows];

        for (int j = 0; j < this->rows; ++j) {
            snakeLengthsArray[i][j] = this->tiles[i][j]->snakeRemaining;
        }
    }

    // If the current present is in the latest moment (there's no saved alternate future), write the current board state as the latest save (latest point in time)
    if (this->presentPoint < 0) {
        this->savedBoardSnakeLengths.insert(this->savedBoardSnakeLengths.begin(), snakeLengthsArray);
    }
    // Otherwise, overwrite the save at the present point with the current board state
    else {
        this->savedBoardSnakeLengths[this->presentPoint] = snakeLengthsArray;
    }

    if (this->savedBoardSnakeLengths.size() > MAX_SAVED_BOARD_SNAKE_LENGTHS) {
        this->savedBoardSnakeLengths.erase(this->savedBoardSnakeLengths.end() - 1);
    }


    // Timeloop achievement - save how many times the player visited every point in time
    if (this->checkTimeloopAchievement) {
        this->timepointsVisitCounters[this->moveCounter] += 1;
        if (this->timepointsVisitCounters[this->moveCounter] >= TIMELOOP_ACHIEVEMENT_VISITS_REQUIRED) {
            this->unlockTimeloopAchievement = true;
            this->setCheckTimeloopAchievement(false);
        }
        // Delete oldest, unachievable pairs from the map
        for (auto it = this->timepointsVisitCounters.begin(); it != this->timepointsVisitCounters.end(); ) {
            if (it->first < this->moveCounter - MAX_SAVED_BOARD_SNAKE_LENGTHS) {
                it = this->timepointsVisitCounters.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

void Board::loadSavedBoardState(int vectorIndex)
{
    sf::Vector2i maxLengthTile(0, 0);
    auto it = this->savedBoardSnakeLengths.begin() + vectorIndex;
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < this->rows; j++) {
            if ((*it)[i][j] > 0) {
                TileStatus previousStatus = this->tiles[i][j]->placeSnakeTile((*it)[i][j],
                    this->snake->length, this->snake->headColor, this->snake->endingColor);
                if ((*it)[i][j] > (*it)[maxLengthTile.x][maxLengthTile.y]) {
                    maxLengthTile.x = i;
                    maxLengthTile.y = j;
                }
                if (previousStatus == TileStatus::Fruit) {
                    Fruit* fruitToMove = this->tiles[i][j]->fruitPtr;
                    this->tiles[i][j]->removeFruit(false);
                    this->moveFruitToRandomPosition(fruitToMove);
                }
            }
            else {
                this->tiles[i][j]->snakeRemaining = 0;
                if (this->tiles[i][j]->status == TileStatus::Snake) this->tiles[i][j]->status = TileStatus::Empty;
            }
        }
    }
    this->snake->teleportHead(maxLengthTile.x, maxLengthTile.y, this->getSnakeFacingDirectionByHeadTile(maxLengthTile.x, maxLengthTile.y, false));
    this->snake->length = this->tiles[maxLengthTile.x][maxLengthTile.y]->snakeRemaining;
}

void Board::manageSavedBoardSnakeLengths() {

    this->saveBoardSnakeLengths(this->presentPoint);

    if (this->presentPoint >= 0) {
        this->presentPoint--;
        for (int i = 0; i < this->fruits.size(); i++) {
            if (this->timeTravelDestinations[i] > 0) {
                this->timeTravelDestinations[i]--;
            }
            else if (this->presentPoint <= -1 && this->timeTravelDestinations[i] == 0) {
                this->reassignTimeTravelDestinationToFruit(i + 1);
            }
        }
    }
}

void Board::reassignTimeTravelDestinationToFruit(int timeTravelNumber) {
    int minValue = floor(abs((float)this->presentPoint / 2.f));  // Furthest future possible to travel to (half of the total saved future)
    int maxValue = (this->presentPoint + (int)ceil(((float)(this->savedBoardSnakeLengths.size() - 1 - this->presentPoint) / 2.f)));  // Furthest Past possible to travel to (half of the total saved past)
    this->timeTravelDestinations[timeTravelNumber - 1] = std::rand() % (maxValue + 1 - minValue) + minValue;
}

void Board::performTimeTravel(Fruit* fruitPtr) {
    // Get the fruit number
    int timeTravelNumber = 0;
    for (auto it = this->fruits.begin(); it != this->fruits.end(); ) {
        if ((*it) == fruitPtr) {
            timeTravelNumber = fruitPtr->timeTravelNumber;
            break;
        }
        ++it;
    }

    bool alternateFutureErased = false;
    int initialPresentPoint = this->presentPoint;
    int initialDestinationPoint = this->timeTravelDestinations[timeTravelNumber - 1];

    // If travelling to the past, erase the alternate future after the Present point
    if (this->presentPoint < this->timeTravelDestinations[timeTravelNumber - 1]) {
        alternateFutureErased = true;
        if (initialPresentPoint >= 1) {
            this->savedBoardSnakeLengths.erase(this->savedBoardSnakeLengths.begin(), this->savedBoardSnakeLengths.begin() + initialPresentPoint + 1);
            this->timeTravelDestinations[timeTravelNumber - 1] -= initialPresentPoint;
        }
    }
    

    // Perform the time travel
    int previousSnakeLength = this->snake->length;
    this->loadSavedBoardState(this->timeTravelDestinations[timeTravelNumber - 1]);
    this->lengthenSnake(previousSnakeLength - this->snake->length);
    this->updateTileColors();

    // Update present point
    this->presentPoint = this->timeTravelDestinations[timeTravelNumber - 1];
    this->moveCounter += initialPresentPoint - initialDestinationPoint;


    // Load another board state (snake lengths) for the used fruit
    this->reassignTimeTravelDestinationToFruit(timeTravelNumber);

    // Delete the travel destinations from the erased alternate future and assign new ones
    if (alternateFutureErased) {
        for (int i = 0; i < this->fruits.size(); i++) {
            if (this->timeTravelDestinations[i] < initialPresentPoint) {
                this->reassignTimeTravelDestinationToFruit(i + 1);
            }
        }
    }


    this->snakeMovementPauseUpdates = this->maxSnakeMovementPauseUpdates;
    this->snake->setPausedTexture(true);
    
}

void Board::replaceTimeTravelIndicators(int timeTravelNumber) {
    // Remove old indicators
    for (auto it = this->timeTravelIndicators.begin(); it != this->timeTravelIndicators.end(); ) {
        if ((*it)->timeTravelNumber == timeTravelNumber) {
            delete* it;
            it = this->timeTravelIndicators.erase(it);
        }
        else {
            it++;
        }
    }

    // Place new indicators
    sf::Color indicatorColor = this->iconColors[timeTravelNumber - 1];
    auto it = this->savedBoardSnakeLengths.begin() + this->timeTravelDestinations[timeTravelNumber - 1];
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < this->rows; j++) {
            if ((*it)[i][j] > 0) {
                this->timeTravelIndicators.push_back(new TileIndicator(i, j, this->tileWidth, this->tileHeight, indicatorColor, 0.f, timeTravelNumber));
            }
        }
    }
}

void Board::lengthenSnake(int units)
{
    this->snake->changeLength(units);
    this->lengthenSnakeOnTiles(units);
}

void Board::lengthenSnakeOnTiles(int units)
{
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < rows; j++) {
            tiles[i][j]->lengthenSnakeOnTile(units);
        }
    }
}

void Board::updateTileColors() {
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < rows; j++) {
            this->tiles[i][j]->updateColor(this->snake->length, this->snake->headColor, this->snake->endingColor);
        }
    }
}

void Board::gameOver() {
    this->isGameOver = true;
    this->lengthenSnake(1);

    if (this->gamemode == Gamemode::Invisible) {
        Tile::setSnakeInvisibility(false, 3);
        this->updateTileColors();
    }
}


void Board::moveSnakeOnTiles() {
    for (int i = 0; i < this->columns; i++) {
        for (int j = 0; j < rows; j++) {
            tiles[i][j]->moveSnakeOnTile(this->snake->length, this->snake->headColor, this->snake->endingColor);
        }
    }
}






void Board::update(int currentFrameNumber) {
    this->currentGameFrameNumber = currentFrameNumber;
    if (this->snake != nullptr) {
        this->snake->update(currentFrameNumber);
        if (this->snake->rainbow || this->snake->emergencyColors) {
            this->updateTileColors();
        }
    }
    if (this->gamemode == Gamemode::Portal) {
        for (auto portal : this->portals) {
            portal->update();
        }
    }
}

void Board::moveSnake() {
    if (this->snake == nullptr) return;
    if (this->snakeMovementPauseUpdates > 0) {
        this->snakeMovementPauseUpdates--;
        if (this->snakeMovementPauseUpdates <= 0)
            this->snake->setPausedTexture(false);
        return;
    }

    //this->moveSnakeOnTiles();
    
    if (this->gamemode == Gamemode::MovingFruits) {
        this->moveFruits();
    }
    else if (this->gamemode == Gamemode::Portal) {
        this->checkPortalRotationCorrect();
        this->lastPortalPlaced = nullptr;
    }

    this->snake->move();
    this->lastSnakeFacingDirection = this->snake->facingDirection;
    Fruit* eatenFruitPtr = nullptr;
    bool moveSnakeOnTilesDone = false;
    if (this->checkCoordinatesInBoardRange(this->snake->headX, this->snake->headY)) {
        if (this->gamemode == Gamemode::Portal) this->removePortalsOnSnakeHead();
        int snakeLengthOnHeadTile = this->snake->length;
        if (this->tiles[this->snake->headX][this->snake->headY]->status != TileStatus::Fruit) {
            this->moveSnakeOnTiles();
            moveSnakeOnTilesDone = true;
        }
        else {
            snakeLengthOnHeadTile++;
        }
        switch (this->tiles[this->snake->headX][this->snake->headY]->placeSnakeTile(snakeLengthOnHeadTile, snakeLengthOnHeadTile, this->snake->headColor, this->snake->endingColor))
        {
        case TileStatus::Fruit:
            this->points++;
            eatenFruitPtr = this->tiles[this->snake->headX][this->snake->headY]->fruitPtr;
            this->tiles[this->snake->headX][this->snake->headY]->removeFruit(true);
            this->lastFruitCollectionTime = (float)this->currentGameFrameNumber / FPS;
            if (this->gamemode == Gamemode::MovingFruits && eatenFruitPtr->moveVector.x != 0 && eatenFruitPtr->moveVector.y != 0) this->diagonalFruitsEaten++;

            this->lengthenSnake(this->snakeLengthening);
            this->moveSnakeOnTiles();

            if (this->gamemode == Gamemode::Portal) this->teleportSnakeThroughPortal(eatenFruitPtr->portalNumber);
            this->moveFruitToRandomPosition(eatenFruitPtr);
            this->snake->setRandomColors();
            if (this->gamemode == Gamemode::Inverting) this->invertSnake();
            else if (this->gamemode == Gamemode::Wall && this->snake->length % 3 != 2) this->wallToPlace++;  // Place 2 walls per 3 eaten fruits
            if (this->gamemode == Gamemode::TimeTravel) this->performTimeTravel(eatenFruitPtr);
            break;
        case TileStatus::Snake:
        case TileStatus::Wall:
            this->gameOver();
            break;
        default:
            break;
        }

        if (this->gamemode == Gamemode::Wall) this->handleWallPlacing();
        else if (this->gamemode == Gamemode::Portal) {
            for (auto it = this->portals.begin(); it != this->portals.end(); ) {
                if (this->tiles[(*it)->x][(*it)->y]->status != TileStatus::Snake) {
                    delete (*it);
                    it = this->portals.erase(it);
                }
                else ++it;
            }
        }
        else if (this->gamemode == Gamemode::TimeTravel) {
            this->manageSavedBoardSnakeLengths();
            for (int i = 0; i < this->fruits.size(); i++) {
                this->replaceTimeTravelIndicators(i + 1);
            }
        }
    }
    else {
        this->gameOver();
        this->snake->move(-1);
    }

    this->moveCounter++;
    if (this->gamemode == Gamemode::TimeTravel) {
        for (auto fruit : this->fruits) {
            fruit->updateTimeTravelText(this->presentPoint, this->timeTravelDestinations[fruit->timeTravelNumber - 1], this->snakeMovesPerSecond);
        }
    }

    if (this->getTileNumberByStatus(TileStatus::Snake) + this->getTileNumberByStatus(TileStatus::Wall) == this->rows * this->columns) {
        this->isBoardFull = true;
    }
}

void Board::updateSnakeFacingDirection(FacingDirection newFacingDirection) {
    this->snake->updateFacingDirection(newFacingDirection);
}

void Board::handleWallPlacing()
{
    if (this->wallToPlace > 0 && std::rand() % 2 == 0) { // Place wall randomly
        if (this->placeWallTile(this->snake->headX, this->snake->headY))
            this->wallToPlace--;
    }

    // Handle indicators
    for (int x = 0; x < this->columns; x++) {
        for (int y = 0; y < this->rows; y++) {
            if (this->tiles[x][y]->markedForWallPlacement) {
                TileIndicator* foundIndicatorPtr = nullptr;
                for (TileIndicator* indicatorPtr : this->wallIndicators) {
                    if (indicatorPtr->x == x && indicatorPtr->y == y) {
                        foundIndicatorPtr = indicatorPtr;
                        break;
                    }
                }
                if (foundIndicatorPtr == nullptr && this->tiles[x][y]->status != TileStatus::Wall) {
                    // Place new indicator
                    this->wallIndicators.push_back(new TileIndicator(x, y, this->tileWidth, this->tileHeight, sf::Color::White, 0.f, 1));
                }
                else if (foundIndicatorPtr != nullptr && this->tiles[x][y]->status == TileStatus::Wall) {
                    // Remove old indicator
                    for (auto it = this->wallIndicators.begin(); it != this->wallIndicators.end(); ) {
                        if ((*it) == foundIndicatorPtr) {
                            delete *it;
                            it = this->wallIndicators.erase(it);
                        }
                        else {
                            it++;
                        }
                    }
                }
            }
        }
    }
}



sf::Vector2f Board::getSize()
{
    return sf::Vector2f(this->width, this->height);
}

bool Board::getGameOver()
{
    return this->isGameOver;
}

bool Board::getBoardFull() {
    return this->isBoardFull;
}

int Board::getPoints() {
    return this->points;
}

int Board::getMoveCounter()
{
    return this->moveCounter;
}

GameData Board::getGameData()
{
    GameData dataToReturn;
    dataToReturn.points = this->points;
    dataToReturn.occupiedTiles = this->getTileNumberByStatus(TileStatus::Snake);
    if (this->gamemode == Gamemode::Wall) dataToReturn.occupiedTiles += this->getTileNumberByStatus(TileStatus::Wall);
    dataToReturn.movesDone = this->moveCounter;
    dataToReturn.timer = 1.f;
    dataToReturn.lastFruitCollectionTime = this->lastFruitCollectionTime;
    dataToReturn.gameOver = this->isGameOver;
    dataToReturn.diagonalFruitsEaten = this->diagonalFruitsEaten;
    dataToReturn.unlockTimeloopAchievement = this->unlockTimeloopAchievement;
    this->diagonalFruitsEaten = 0;

    return dataToReturn;
}

bool Board::checkCoordinatesInBoardRange(sf::Vector2i coordinates)
{
    return (coordinates.x >= 0 && coordinates.x < this->columns&& coordinates.y >= 0 && coordinates.y < this->rows);
}

bool Board::checkCoordinatesInBoardRange(int x, int y)
{
    return (x >= 0 && x < this->columns&& y >= 0 && y < this->rows);
}

bool Board::checkGameOverInXMoves(int moves) {
    // Check if the player would lose in the next [moves] moves if the player didn't change the Snake's facing direction
    if (moves <= 0) return false;

    // Condition 1 - check if the snake head would hit the board edge
    bool condition1 = (this->snake->headX <= (moves - 1) && this->snake->facingDirection == FacingDirection::Left)
        || (this->snake->headX >= (this->columns - moves) && this->snake->facingDirection == FacingDirection::Right)
        || (this->snake->headY <= (moves - 1) && this->snake->facingDirection == FacingDirection::Up)
        || (this->snake->headY >= (this->rows - moves) && this->snake->facingDirection == FacingDirection::Down);
    if (condition1) return condition1;

    // Condition 2 - check if the snake head would hit another snake tile
    bool condition2 = false;
    for (int x = -moves; x <= moves; x++) {
        if (x == 0) continue;
        if (condition2) break;
        bool a = (this->checkCoordinatesInBoardRange(this->snake->headX + x, this->snake->headY)
            && this->tiles[this->snake->headX + x][this->snake->headY]->status == TileStatus::Snake);
        bool b;
        if (x < 0)
            b = this->snake->facingDirection == FacingDirection::Left;
        else
            b = this->snake->facingDirection == FacingDirection::Right;

        condition2 = (condition2) || (a & b);
    }
    for (int y = -moves; y <= moves; y++) {
        if (y == 0) continue;
        if (condition2) break;
        bool a = (this->checkCoordinatesInBoardRange(this->snake->headX, this->snake->headY + y)
            && this->tiles[this->snake->headX][this->snake->headY + y]->status == TileStatus::Snake);
        bool b;
        if (y < 0)
            b = this->snake->facingDirection == FacingDirection::Up;
        else
            b = this->snake->facingDirection == FacingDirection::Down;

        condition2 = (condition2) || (a & b);
    }

    return condition2 || condition1;
}

FacingDirection Board::getSnakeFacingDirectionByHeadTile(int tileX, int tileY, bool reversed)
{
    int snakeRemainingOnMainTile = this->tiles[tileX][tileY]->snakeRemaining;
    FacingDirection toReturn;

    if (this->checkCoordinatesInBoardRange(tileX + 1, tileY)
        && this->tiles[tileX + 1][tileY]->snakeRemaining == snakeRemainingOnMainTile - 1)
        toReturn = FacingDirection::Left;
    else if (this->checkCoordinatesInBoardRange(tileX - 1, tileY)
        && this->tiles[tileX - 1][tileY]->snakeRemaining == snakeRemainingOnMainTile - 1)
        toReturn = FacingDirection::Right;
    else if (this->checkCoordinatesInBoardRange(tileX, tileY + 1)
        && this->tiles[tileX][tileY + 1]->snakeRemaining == snakeRemainingOnMainTile - 1)
        toReturn = FacingDirection::Up;
    else if (this->checkCoordinatesInBoardRange(tileX, tileY - 1)
        && this->tiles[tileX][tileY - 1]->snakeRemaining == snakeRemainingOnMainTile - 1)
        toReturn = FacingDirection::Down;
    else
        toReturn = FacingDirection::Right;

    if (reversed) {
        toReturn = (FacingDirection)(((int)toReturn + 1) % 4 + 1);
    }

    return toReturn;
}

int Board::getFreeSpacesAroundTile(int tileX, int tileY)
{
    int freeSpacesAround = 4;
    if (!this->checkCoordinatesInBoardRange(tileX + 1, tileY) || this->tiles[tileX + 1][tileY]->markedForWallPlacement)
        freeSpacesAround--;
    if (!this->checkCoordinatesInBoardRange(tileX, tileY + 1) || this->tiles[tileX][tileY + 1]->markedForWallPlacement)
        freeSpacesAround--;
    if (!this->checkCoordinatesInBoardRange(tileX - 1, tileY) || this->tiles[tileX - 1][tileY]->markedForWallPlacement)
        freeSpacesAround--;
    if (!this->checkCoordinatesInBoardRange(tileX, tileY - 1) || this->tiles[tileX][tileY - 1]->markedForWallPlacement)
        freeSpacesAround--;

    return freeSpacesAround;
}

int Board::getTileNumberByStatus(TileStatus status)
{
    int counter = 0;
    for (int x = 0; x < this->columns; x++) {
        for (int y = 0; y < this->rows; y++) {
            if (this->tiles[x][y]->status == status) {
                counter++;
            }
        }
    }

    return counter;
}

//void Board::draw(sf::RenderWindow* renderWindowPtr) {
//    this->drawRenderTexture();
//    this->sprite.setTexture(this->renderTexture.getTexture());
//    renderWindowPtr->draw(this->sprite);
//}
//void Board::draw(sf::RenderTexture* renderTexturePtr) {
//    this->drawRenderTexture();
//    this->sprite.setTexture(this->renderTexture.getTexture());
//    renderTexturePtr->draw(this->sprite);
//}

