#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <thread>

#include "Wave.h"

#include "ConsoleGameEngine.h"

#include "Map.h"

#include "Player.h"
#include "Entity.h"

#include "Wall.h"
#include "Tower.h"
#include "Base.h"

#include "IEnemyFactory.h"

#define LOST	1
#define WON		2

enum GAME { CONTINUE, YOU_WIN, YOU_LOSE };

enum BUTTON {
	W = 0x57,
	A = 0x41,
	S = 0x53,
	D = 0x44,
	E = 0x45,
	R = 0x52,
	F = 0x46
};

enum GUI {
	HEART = u'\u2665',
	COIN = u'\u2776',
	AMMO = u'\u258C',
	BASE_HEALTH = u'\u263B',

	HEART_ATTR = COLOUR::FG_RED,
	COIN_ATTR = COLOUR::FG_YELLOW,
	AMMO_ATTR = COLOUR::FG_DARK_MAGENTA,
	BASE_HEALTH_ATTR = COLOUR::FG_GREEN
};

class Game : public ConsoleGameEngine{
private:
	Map*	map;
	Player* hero;
	Base*	castle;

	std::vector<Tower*>			towers;
	std::vector<Projectile*>	projectiles;
	std::vector<Wave*>			waves;

	int enemySpawnX,
		enemySpawnY;

	void spawn(Object* obj) {
		int x = obj->getX(),
			y = obj->getY();
		if (map->isEmpty(x, y))
			map->add(obj);
	}

	void kill(Object* obj) {
		int x = obj->getX(),
			y = obj->getY();
		obj->die();
		map->erase(x, y);
	}

	void move(Object* obj, int x, int y) {
		int oldX = obj->getX(),
			oldY = obj->getY();
		obj->setCoords(x, y);
		map->erase(oldX, oldY);
		map->add(obj);
	}

	/*PLAYER, ENEMY, PROJECTILE, COMRADE, WALL, BASE*/

	void handleCollision(Entity* ent, Object* obj) {
		switch (ent->getType()) {
		case OBJECT::PLAYER:
			switch (obj->getType()) {
			case OBJECT::PLAYER:
				break;
			case OBJECT::ENEMY:
			{
				//mciSendString(TEXT("play \"Sounds/player_hit.wav\""), NULL, 0, NULL);
				Enemy* en = static_cast<Enemy*>(obj);
				en->hit(ent);
			}
				break;
			case OBJECT::PROJECTILE:
				break;
			case OBJECT::COMRADE:
				break;
			case OBJECT::WALL:
				break;
			case OBJECT::BASE:
				break;
			}
			break;
		case OBJECT::ENEMY:
			switch (obj->getType()) {
			case OBJECT::PLAYER:
			{
				mciSendString(TEXT("play \"Sounds/player_hit.wav\""), NULL, 0, NULL);
				Enemy* en = static_cast<Enemy*>(ent);
				en->hit(obj);
			}
				break;
			case OBJECT::ENEMY:
				break;
			case OBJECT::PROJECTILE:
			{
				Projectile* pr = static_cast<Projectile*>(obj);
				pr->hit(ent);
				if (ent->isDead()) {
					hero->gainCoins(1);
					hero->gainAmmo(1);
					mciSendString(TEXT("play \"Sounds/boom.wav\""), NULL, 0, NULL);
				}
				else
					mciSendString(TEXT("play \"Sounds/hit.wav\""), NULL, 0, NULL);
			}
				break;
			case OBJECT::COMRADE:
				kill(obj);
				break;
			case OBJECT::WALL:
				break;
			case OBJECT::BASE: 
			{
				mciSendString(TEXT("play \"Sounds/base_hit.wav\""), NULL, 0, NULL);
				Enemy* en = static_cast<Enemy*>(ent);
				en->hit(obj);
				kill(ent);
			}
				break;
			}
			break;
		case OBJECT::PROJECTILE:
			switch (obj->getType()) {
			case OBJECT::PLAYER:
				kill(ent);
				break;
			case OBJECT::ENEMY:
			{
				Projectile* pr = static_cast<Projectile*>(ent);
				pr->hit(obj);
				if (obj->isDead()) {
					hero->gainCoins(1);
					hero->gainAmmo(1);
					mciSendString(TEXT("play \"Sounds/boom.wav\""), NULL, 0, NULL);
				}
				else
					mciSendString(TEXT("play \"Sounds/hit.wav\""), NULL, 0, NULL);
			}
			break;
			case OBJECT::PROJECTILE:
				kill(ent);
				kill(obj);
				break;
			case OBJECT::COMRADE:
				kill(ent);
				break;
			case OBJECT::WALL:
				kill(ent);
				break;
			case OBJECT::BASE:
				kill(ent);
				break;
			}
			break;
		case OBJECT::COMRADE:
			switch (obj->getType()) {
			case OBJECT::PLAYER:
				break;
			case OBJECT::ENEMY:
				kill(ent);
				break;
			case OBJECT::PROJECTILE:
				kill(obj);
				break;
			case OBJECT::COMRADE:
				break;
			case OBJECT::WALL:
				break;
			case OBJECT::BASE:
				kill(ent);
				castle->addComrade();
				mciSendString(TEXT("play \"Sounds/success.wav\""), NULL, 0, NULL);
				break;
			}
			break;
		}
	}

public:
	void init(const char* layoutFile, const char* levelDataFile, IEnemyFactory* factory) {
		std::ifstream levelData(levelDataFile);
		int playerX, playerY,
			mapWidth, mapHeight,
			pathLength;

		levelData >> mapWidth >> mapHeight >> playerX >> playerY >> pathLength;
		map = new Map(mapWidth, mapHeight, layoutFile);
		for (size_t y = 0; y < map->getHeight(); y++) {
			for (size_t x = 0; x < map->getWidth(); x++) {
				switch (map->getScreen()[x + y * map->getWidth()].Char.UnicodeChar) {
				case WALL:
					spawn(new Wall(x, y));
					break;
				}
			}
		}

		COORD* path = new COORD[pathLength];
		for (size_t i = 0; i < pathLength; i++) {
			levelData >> path[i].X >> path[i].Y;
		}

		enemySpawnX = path[0].X;
		enemySpawnY = path[0].Y;

		castle = new Base(path[pathLength - 1].X, path[pathLength - 1].Y);
		spawn(castle);
		int numOfEnemies;
		while (levelData >> numOfEnemies) {
			waves.push_back(new Wave(factory, numOfEnemies, path));
		}

		levelData.close();
		constructConsole(map->getWidth(), map->getHeight(), 12, 12);
		setScreen(map->getScreen());

		hero = new Player(playerX, playerY, DIRECTION::SOUTH);
		spawn(hero);
		PlaySound(TEXT("Sounds/beep_boop228.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	}
	Game(const char* layoutFile, const char* levelDataFile, IEnemyFactory* factory) {
		srand((unsigned)time(0));
		init(layoutFile, levelDataFile, factory);
	}
	void update() override {

		if (castle->isDead() || hero->isDead()) {
			PlaySound(TEXT("Sounds/game_over.wav"), NULL, SND_ASYNC);
			retval = LOST;
			stopThread();
		}
		else if (isHeld(VK_ESCAPE)) {
			stopThread();
		}
		else if (waves.empty()) {
			PlaySound(TEXT("Sounds/you_win.wav"), NULL, SND_ASYNC);
			retval = WON;
			stopThread();
		}

		map->display(hero->getAmmo(),		{ GUI::AMMO, GUI::AMMO_ATTR }, 0);
		map->display(hero->getHealth(),		{ GUI::HEART, GUI::HEART_ATTR }, 1);
		map->display(hero->getCoins(),		{ GUI::COIN, GUI::COIN_ATTR }, 2);
		map->display(castle->getHealth(),	{ GUI::BASE_HEALTH, GUI::BASE_HEALTH_ATTR }, 3);

		int x = hero->getX(),
			y = hero->getY();

		if (isHeld(BUTTON::W)) {
			y--;
			hero->setFacing(DIRECTION::NORTH);
		}
		if (isHeld(BUTTON::S)) {
			y++;
			hero->setFacing(DIRECTION::SOUTH);
		}
		if (isHeld(BUTTON::D)) {
			x++;
			hero->setFacing(DIRECTION::EAST);
		}
		if (isHeld(BUTTON::A)) {
			x--;
			hero->setFacing(DIRECTION::WEST);
		}
		if (isHeld(VK_SPACE)) {
			hero->shoot(map, projectiles);
		}


		if (isPressed(BUTTON::E)) {
			hero->buildTower(map, towers);
		}
		if (isPressed(BUTTON::R)) {
			hero->sellTower(map);
		}
		if (isPressed(BUTTON::F)) {
			hero->upgradeTower(map);
		}

		hero->setNextCoords({ (SHORT)x, (SHORT)y });

		if (waves.size() > 0) {
			if (!waves.back()->isEmpty()) {
				waves.back()->spawn(map);
			}
			else {
				hero->gainAmmo(4);
				hero->gainCoins(2);
				mciSendString(TEXT("play \"Sounds/coin.wav\""), NULL, 0, NULL);
				waves.pop_back();
			}
		}

		for (size_t y = 0; y < map->getHeight(); y++) {
			for (size_t x = 0; x < map->getWidth(); x++) {
				if (!map->isEmpty(x, y)) {
					if (Entity* ent = dynamic_cast<Entity*>(map->getObject(x, y))) {
						if (ent->isDead()) {
							kill(ent);
						}
						if (!ent->walk(map)) {
							if (!map->isInside(ent->getNextX(), ent->getNextY())) {
								handleCollision(ent, new Wall(ent->getNextX(), ent->getNextY()));
							}
							else
								handleCollision(ent, map->getObject(ent->getNextX(), ent->getNextY()));
						}
					}
					else if (Tower* tow = dynamic_cast<Tower*>(map->getObject(x, y))) {
						if (waves.empty() || waves.back()->isRunning()) {
							tow->shoot(map, projectiles);
						}
					}
				}
			}
		}

		for (size_t i = 0; i < projectiles.size(); i++) {
			if (projectiles.at(i)->isDead()) {
				projectiles.erase(projectiles.begin() + i);
			}
		}
		for (size_t i = 0; i < towers.size(); i++) {
			if (towers.at(i)->isDead()) {
				towers.erase(towers.begin() + i);
			}
		}
	}
};