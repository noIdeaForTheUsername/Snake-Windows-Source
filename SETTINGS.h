#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum class Gamemode { Normal = 0, Invisible, Inverting, Wall, MovingFruits, Portal, TimeTravel};

// All settings const variables are written with capital letters
// Time is always given in seconds

const int FPS = 30;
const int BOARD_UPDATE_FRAMES = 6;  // Should be in range of 2-15 for normal gameplay (default: 6)
const float MIN_PAUSE_TIME = 0.5f;

const float WINDOW_WIDTH = 820.f;
const float WINDOW_HEIGHT = 946.f;

const float BOARD_WIDTH = 820.f;
const float BOARD_HEIGHT = 820.f;

const std::string SAVE_FILE_DIRECTORY = "gameData.bin";

const int SNAKE_START_LENGTH = 4;
const int FRUIT_COLLECTION_LENGTHENING = 1;

const float SNAKE_MOVEMENT_PAUSE_TIME = 0.333f;
const int MAX_SAVED_BOARD_SNAKE_LENGTHS = 70;  // For the time travel mode

const float MUSIC_FADEOUT_TIME = 1.f;
const float MUSIC_FADEIN_TIME = 1.5f;
const int SNAKE_GRADIENT_TRANSPARENCY = 120; // For the semi-transparent gradient
const float DEFAULT_MUSIC_PITCH = 1.f;
const float MUSIC_PITCH_ON_PAUSE = 0.2f;
const float DEFAULT_VOLUME = 80.f;
const float MUSIC_VOLUME_ON_PAUSE = 0.f;

const float ENDING_SEQUENCE_DURATION = 4.f;
const float ENDING_RESTART_PERIOD_DURATION = 4.f;

const float ACHIEVEMENT_NOTIFCATION_DISPLAY_TIME = 7.f;
const int TIMELOOP_ACHIEVEMENT_VISITS_REQUIRED = 12;

// The minimum distance a newly spawned Fruit (in Portal mode) needs to have between itself and the Snake's head:
// if the Snake is facing the fruit
const int PORTAL_MODE_MINIMUM_FRUIT_DISTANCE = 5;

const sf::Color SNAKE_COLOR_HEAD(220, 45, 0, 255);
const sf::Color SNAKE_COLOR_ENDING(255, 200, 190, 255);
const int MIN_SNAKE_COLOR_TRANSITION = 50; // Minimum snake length required for FULL transition between the 2 colors above - below this length a partial transition will be done
// This variable exists so the snake tiles don't have too fast transition and too high color differences between each other at low snake lengths
// The lower the variable is, the higher differences will be visible at Snake lengths below its value
const int MIN_SNAKE_COLOR_TRANSITION_2 = 15;  // Transition length with custom gradient enabled
