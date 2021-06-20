#include <chrono>
#include <vector>

#include "IEnemyFactory.h"
#include "Enemy.h"
#include "Comrade.h"

class Wave {
	float delay, startDelay; //in seconds
	bool enemySpawned, comradeSpawned, running;
	Comrade* comrade;
	std::vector<Enemy*> enemies;
	std::vector<Enemy*>::iterator it;
	std::chrono::system_clock::time_point lastSpawned;
	bool isReadyToSpawn() {
		auto current = std::chrono::system_clock::now();
		if (!running) {
			lastSpawned = current;
			running = true;
		}
		std::chrono::duration<float> elapsedTime;
		elapsedTime = current - lastSpawned;
		if (enemySpawned)
			return elapsedTime.count() >= startDelay;
		else
			return elapsedTime.count() >= delay;
	}
public:
	Wave(IEnemyFactory* factory, int num, COORD* path) {
		delay = 1.0f;
		startDelay = 7.0f;
		enemySpawned = true;
		comradeSpawned = false;
		running = false;
		lastSpawned = std::chrono::system_clock::now();

		for(size_t i = 0; i < num; i++)
			enemies.push_back(factory->create(path));
		comrade = new Comrade(path);

		it = enemies.begin();
	}
	Enemy* getEnemy(int index) {
		return enemies.at(index);
	}
	void spawn(Map* map) {
		if (!comradeSpawned) {
			map->add(comrade);
			comradeSpawned = true;
		}
		if(isReadyToSpawn() && !enemies.empty() && it != enemies.end()){
			map->add(*it);
			it++;
			lastSpawned = std::chrono::system_clock::now();

			if(enemySpawned)
				enemySpawned = false;
		}
	}
	bool isEmpty() {
		for (size_t i = 0; i < enemies.size(); i++)
			if (enemies.at(i)->isDead()) 
				it = enemies.erase(enemies.begin() + i);
		return enemies.empty();
	}
	bool isRunning() {
		return enemySpawned ? 0 : 1;
	}
};