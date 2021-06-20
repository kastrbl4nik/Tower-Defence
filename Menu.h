#pragma once
#include "ConsoleGameEngine.h"

#include <array>

#include "Game.h"
#include "EasyEnemyFactory.h"
#include "NormalEnemyFactory.h"
#include "HardEnemyFactory.h"

#define MAIN_MENU	1
#define YOU_DIED	2
#define YOU_WIN		3
#define DIFFICULTY	4

#define EXIT_GAME  -1

class Menu : public ConsoleGameEngine {

	CHAR_INFO	*mainMenu,
				*difficultyMenu,
				*youDied,
				*youWin;

	int choosenDifficulty;

	std::chrono::system_clock::time_point lastMove;
	float pause;

	size_t currentScreen, option;

	int width, height;

	void makeScreen(const char* file, CHAR_INFO* scrn) {
		std::wifstream msg(file);
		std::wstring line;
		for (size_t y = 0; y < height; y++) {
			getline(msg, line);
			for (size_t x = 0; x < width; x++) {
				scrn[x + y * width] = convertToCharInfo(line[x]);
			}
		}
		msg.close();
	}

	CHAR_INFO convertToCharInfo(wchar_t w_ch) {
		switch (w_ch) {
		case '$': return { u' ', COLOUR::BG_RED };
		case '@': return { u' ', COLOUR::BG_GREEN };
		case '.': return { u' ', COLOUR::BG_BLACK };
		default: return { w_ch, COLOUR::FG_WHITE | COLOUR::BG_BLACK };
		}
	}

	bool pauseOver() {
		auto current = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastMove;
		if (elapsedTime.count() >= pause)
			return 1;
		else
			return 0;
	}

public:
	Menu() {
		width	= 105;
		height	= 54;

		pause	 = 0.2;
		lastMove = std::chrono::system_clock::now();

		mainMenu		= new CHAR_INFO[width * height];
		youDied			= new CHAR_INFO[width * height];
		youWin			= new CHAR_INFO[width * height];
		difficultyMenu	= new CHAR_INFO[width * height];

		constructConsole(width, height, 12, 12);

		makeScreen("main_menu.txt", mainMenu);
		makeScreen("you_died.txt", youDied);
		makeScreen("you_win.txt", youWin);
		makeScreen("difficulty_menu.txt", difficultyMenu);

		currentScreen = MAIN_MENU;

		choosenDifficulty = 1;
		difficultyMenu[46 + (26 + choosenDifficulty) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
		difficultyMenu[59 + (26 + choosenDifficulty) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };

		show(MAIN_MENU);
	}
	~Menu() {
		delete[]	mainMenu,
					difficultyMenu,
					youDied,
					youWin;
	}
	void show(size_t scrn) {
		switch (scrn) {
		case MAIN_MENU:
			option = 1;
			setScreen(mainMenu);
			currentScreen = MAIN_MENU;
			mainMenu[42 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
			mainMenu[63 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
			break;
		case YOU_DIED:
			setScreen(youDied);
			currentScreen = YOU_DIED;
			break;
		case YOU_WIN:
			setScreen(youWin);
			currentScreen = YOU_WIN;
			break;
		case DIFFICULTY:
			option = 1;
			setScreen(difficultyMenu);
			currentScreen = DIFFICULTY;
			difficultyMenu[46 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
			difficultyMenu[59 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
			difficultyMenu[44 + (26 + choosenDifficulty) * width] = { u'\u25BA', COLOUR::FG_YELLOW | COLOUR::BG_BLACK };
			difficultyMenu[61 + (26 + choosenDifficulty) * width] = { u'\u25C4', COLOUR::FG_YELLOW | COLOUR::BG_BLACK };
			break;
		}
	}
	void update() override {
		switch (currentScreen) {
		case MAIN_MENU:
			if (isHeld(VK_UP) && option > 1 && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/select.wav\""), NULL, 0, NULL);
				mainMenu[42 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				mainMenu[63 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				option--;
				mainMenu[42 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				mainMenu[63 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				lastMove = std::chrono::system_clock::now();
			}
			if (isHeld(VK_DOWN) && option < 3 && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/select.wav\""), NULL, 0, NULL);
				mainMenu[42 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				mainMenu[63 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				option++;
				mainMenu[42 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				mainMenu[63 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				lastMove = std::chrono::system_clock::now();
			}
			if (isHeld(VK_RETURN) && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/selected.wav\""), NULL, 0, NULL);
				lastMove = std::chrono::system_clock::now(); 
				mainMenu[42 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				mainMenu[63 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				switch (option) {
				case 1:
					stopThread();
					break;
				case 2:
					show(DIFFICULTY);
					break;
				case 3:
					retval = EXIT_GAME;
					stopThread();
				}
			}
			break;
		case DIFFICULTY:
			if (isHeld(VK_UP) && option > 1 && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/select.wav\""), NULL, 0, NULL);
				difficultyMenu[46 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				difficultyMenu[59 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				option--;
				difficultyMenu[46 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				difficultyMenu[59 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				lastMove = std::chrono::system_clock::now();
			}
			if (isHeld(VK_DOWN) && option < 4 && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/select.wav\""), NULL, 0, NULL);
				difficultyMenu[46 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				difficultyMenu[59 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
				option++;
				difficultyMenu[46 + (26 + option) * width] = { u'\u25BA', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				difficultyMenu[59 + (26 + option) * width] = { u'\u25C4', COLOUR::FG_GREEN | COLOUR::BG_BLACK };
				lastMove = std::chrono::system_clock::now();
			}
			if (isHeld(VK_RETURN) && pauseOver()) {
				mciSendString(TEXT("play \"Sounds/selected.wav\""), NULL, 0, NULL);
				lastMove = std::chrono::system_clock::now();
				difficultyMenu[44 + (26 + choosenDifficulty) * width] = { u' ', COLOUR::BG_BLACK };
				difficultyMenu[61 + (26 + choosenDifficulty) * width] = { u' ', COLOUR::BG_BLACK };
				switch (option) {
				case 1:
					choosenDifficulty = 1;
					break;
				case 2:
					choosenDifficulty = 2;
					break;
				case 3:
					choosenDifficulty = 3;
					break;
				}
				difficultyMenu[44 + (26 + choosenDifficulty) * width] = { u'\u25BA', COLOUR::FG_YELLOW | COLOUR::BG_BLACK };
				difficultyMenu[61 + (26 + choosenDifficulty) * width] = { u'\u25C4', COLOUR::FG_YELLOW | COLOUR::BG_BLACK };
				if (option == 4) {
					difficultyMenu[46 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
					difficultyMenu[59 + (26 + option) * width] = { u' ', COLOUR::BG_BLACK };
					show(MAIN_MENU);
				}
			}
			break;
		case YOU_DIED:
			if (isHeld(VK_RETURN)) {
				retval = 1;
				stopThread();
			}
			break;
		case YOU_WIN:
			if (isHeld(VK_RETURN)) {
				retval = 1;
				stopThread();
			}
			break;
		}
		if (isHeld(VK_ESCAPE)) {
			retval = EXIT_GAME;
			stopThread();
		}
	}
	int getChoosenOption() {
		return option;
	}
	IEnemyFactory* getChoosenFactory() {

		std::array<IEnemyFactory*, 3> factories = {
			new EasyEnemyFactory(),
			new NormalEnemyFactory(),
			new HardEnemyFactory()
		};

		IEnemyFactory* factory;

		switch (choosenDifficulty) {
		case 1:
			factory = factories[0];
			break;
		case 2:
			factory = factories[1];
			break;
		case 3:
			factory = factories[2];
			break;
		default:
			factory = factories[0];
		}

		return factory;
	}
};