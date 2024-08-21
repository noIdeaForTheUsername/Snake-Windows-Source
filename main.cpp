#include <iostream>
#include "SETTINGS.h"
#include "Game.h"
#include <windows.h>

// DEV NOTE:
// 
// It is very likely that this code cannot be called clear, logical and well-described at all.
// Especially considering the facts, that I barely use comments at all, the Menu.cpp file reached over 1800 lines
// and I didn't split it into multiple classes, some variable names reach like 30-40 characters,
// and I was still learning C++ and SFML when I began the work on this project.
// So, if for some reason someone is actually viewing this code and reading this note (though I highly doubt this ever happens),
// I just want to tell you that..... I wish you a very good luck!!!
// 
// ~ noIdeaForTheUsername, 21.08.2024
// 

int main()
{
	try
	{
		Game game;
		game.processGame();
		return 0;
	}
	catch (const std::runtime_error& e) {
		std::string errorMessage = "An error has been detected:\n\n";

		errorMessage += e.what();

		MessageBoxA(NULL, errorMessage.c_str(), "Error detected", MB_ICONERROR | MB_OK);
		return 0;
	}
	catch (const std::exception& e)
	{
		std::string errorMessage = "An unpredicted error has occured. "
			"If this continues to happen constantly when trying to start the game, delete the game data (gameData.bin), or reinstall the game. "
			"If you want to report this as a bug (GitHub username: noIdeaForTheUsername), "
			"please provide the description below, as well as the chosen gamemode and a short description of what happened:\n\n";

		errorMessage += e.what();

		MessageBoxA(NULL, errorMessage.c_str(), "Unexpected Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	catch (...)
	{
		std::string errorMessage = "An unknown error has occured.\n\n"
			"If this continues to happen constantly when trying to start the game, delete the game data (gameData.bin), or reinstall "
			"the game. If you want to report a bug (GitHub username: noIdeaForTheUsername), provide basic info, "
			"such as the gamemode and what happened just before this error message appeared.";

		MessageBoxA(NULL, errorMessage.c_str(), "Unknown error", MB_ICONERROR | MB_OK);
		return -1;
	}
	
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	main();
	return 0;
}
