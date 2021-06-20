#include <array>

#include "Game.h"
#include "Menu.h"

#pragma comment(lib, "Winmm.lib")

int main() {
	std::array<Game*, 1> lvl;

	Menu menu;
	bool tryAgain = false;


	if(menu.start() == EXIT_GAME) return EXIT_SUCCESS;

	if (menu.getChoosenOption()) {

		for (size_t i = 0; i < lvl.size(); i++) {

			lvl[i] = new Game("layout1.txt", "lvl1_data.txt", menu.getChoosenFactory());

			do {
				switch (lvl[i]->start()) {
				case LOST:
					menu.show(YOU_DIED);
					if (menu.start())
						tryAgain = true;
					break;
				case WON:
					menu.show(YOU_WIN);
					if (menu.start())
						tryAgain = false;
					break;
				}
				lvl[i]->init("layout1.txt", "lvl1_data.txt", menu.getChoosenFactory());
			} while (tryAgain);

		}
	}
	return EXIT_SUCCESS;
}
