#pragma once

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>

constexpr auto MS_PER_UPDATE = 0.003;

class ConsoleGameEngine {
	CHAR_INFO* screen;

	HANDLE				hConsole;
	CONSOLE_FONT_INFOEX cfi;
	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT			window;
	HWND				consoleWindow;

	int screenWidth,
		screenHeight;

	float fElapsedTime;

	bool stop;

	wchar_t title[256];

	struct keyState {
		bool pressed;
		bool released;
		bool held;
	} keys[256];

	int newKeyState[256];
	int oldKeyState[256];

	void engineGoBrrrr() {

		auto previous	= std::chrono::system_clock::now();
		auto current	= std::chrono::system_clock::now();

		std::chrono::duration<float> elapsedTime;
		float lag = 0.0f;

		while (!stop) {
			current = std::chrono::system_clock::now();
			elapsedTime = current - previous;
			previous = current;
			lag += elapsedTime.count();

			for (int i = 0; i < 256; i++) {

				keys[i].pressed = false;
				keys[i].released = false;

				newKeyState[i] = GetAsyncKeyState(i);

				if (newKeyState[i] != oldKeyState[i]) {
					if (newKeyState[i] & 0x8000)
					{
						keys[i].pressed = !keys[i].held;
						keys[i].held = true;
					}
					else
					{
						keys[i].released = true;
						keys[i].held = false;
					}
				}

				oldKeyState[i] = newKeyState[i];

			}

			while (lag >= MS_PER_UPDATE) {
				update();
				lag -= MS_PER_UPDATE;
			}

			swprintf_s(title, 256, L"FPS: %3.2f", 1.0f / elapsedTime.count());

			WriteConsoleOutputW(
				hConsole,
				screen,
				{ (SHORT)screenWidth, (SHORT)screenHeight },
				{ 0,0 },
				&window
			);

			SetConsoleTitle(title);
		}
	}

public:
	ConsoleGameEngine() {
		retval = EXIT_SUCCESS;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		std::memset(newKeyState, 0, 256 * sizeof(short));
		std::memset(oldKeyState, 0, 256 * sizeof(short));
		std::memset(keys, 0, 256 * sizeof(keyState));
	}
	void constructConsole(int width, int height, int fontWidth, int fontHeight) {
		screenWidth		= width;
		screenHeight	= height;

		window = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(hConsole, TRUE, &window);

		SetConsoleScreenBufferSize(hConsole, { (SHORT)screenWidth, (SHORT)screenHeight });
		SetConsoleActiveScreenBuffer(hConsole);

		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(hConsole, &cci);

		cfi.cbSize			= sizeof(cfi);
		cfi.nFont			= 0;
		cfi.dwFontSize.X	= fontWidth;
		cfi.dwFontSize.Y	= fontHeight;
		cfi.FontFamily		= FF_DONTCARE;
		cfi.FontWeight		= FW_NORMAL;

		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

		window = { 0, 0, (SHORT)screenWidth - 1, (SHORT)screenHeight - 1};
		SetConsoleWindowInfo(hConsole, TRUE, &window);

		consoleWindow = GetConsoleWindow();
		SetWindowLongPtr(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); //disabling window resize
	}

	virtual void update() = 0;

	int start() {
		stop = false;
		std::thread t = std::thread(&ConsoleGameEngine::engineGoBrrrr, this);
		t.join();
		return retval;
	}

	void setScreen(CHAR_INFO* screen) {
		this->screen = screen;
	}

	bool isHeld(int key) {
		return keys[key].held;
	}
	bool isPressed(int key) {
		return keys[key].pressed;
	}
	bool isReleased(int key) {
		return keys[key].released;
	}
	void stopThread() {
		stop = true;
	};
protected:
	int retval;
};