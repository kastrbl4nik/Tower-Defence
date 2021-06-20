#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include "Colours.h"
#include "Object.h"

class Map {
private:
	int width, height;
	CHAR_INFO* screen;
	CHAR_INFO* layout;
	Object** objects;
	CHAR_INFO getTile(wchar_t ch) {
		switch (ch) {
		case ' ': return { SPRITE::AIR, SPRITE::AIR_ATTR };
		case '#': return { SPRITE::WALL, SPRITE::WALL_ATTR };
		case '.': return { SPRITE::ROAD, SPRITE::ROAD_ATTR };
		case 'T': return { SPRITE::BASE, SPRITE::BASE_ATTR };
		}
		return { ch, 0 };
	}
public:
	Map(int width, int height, const char* layoutFile) {

		screen		= new CHAR_INFO[width * height];
		objects		= new Object * [width * height];
		layout		= new CHAR_INFO[width * height];

		std::memset(objects, 0, width * height * sizeof(Object*));

		this->width		= width;
		this->height	= height;

		std::wifstream asciiLayout(layoutFile);
		std::wstring line;
		for (size_t y = 0; y < height; y++) {
			getline(asciiLayout, line);
			for (size_t x = 0; x < width; x++) {
				screen[x + y * width] = layout[x + y * width] = getTile(line[x]);
			}
		}
		asciiLayout.close();
	}
	~Map() {
		delete[] screen, objects, layout;
	}

	void display(int value, CHAR_INFO icon, int y) {
		for (size_t x = 0; x < width; x++) {
			if (value != 0) {
				screen[x + y * width] = icon;
				value--;
			}
			else
				erase(x, y);
		}
	}

	bool isInside(int x, int y) {
		if (x >= 0 && x < width && y > 3 && y < height)
			return 1;
		else
			return 0;
	}

	bool isEmpty(int x, int y) {
		if (isInside(x, y) && objects[x + y * width] == NULL)
			return 1;
		else
			return 0;
	}

	bool isEmpty(int x1, int y1, int x2, int y2) {
		for (int y = y1; y <= y2; y++) {
			for (int x = x1; x <= x2; x++) {
				if (!isEmpty(x, y) || screen[x + y * width].Char.UnicodeChar == SPRITE::ROAD)
					return 0;
			}
		}
		return 1;
	}

	void erase(int x, int y) {
		screen[x + y * width]	= layout[x + y * width];
		objects[x + y * width]	= nullptr;
	}

	void erase(int pos) {
		screen[pos]		= layout[pos];
		objects[pos]	= nullptr;
	}

	void destroy(int x, int y) {
		delete objects[x + y * width];
		erase(x, y);
	}

	void draw(int x, int y, CHAR_INFO c) {
		screen[x + y * width] = c;
	}

	void add(Object* obj);

	int getWidth() { return width; }
	int getHeight() { return height; }

	CHAR_INFO* getScreen() {
		return screen;
	}
	Object* getObject(int x, int y) {
		return objects[x + y * width];
	}
	CHAR_INFO getScreen(int x, int y) {
		return screen[x + y * width];
	}
};