#include "Entity.h"
#include "Map.h"

void Map::add(Object* obj) {
	screen[obj->getX() + obj->getY() * width] = obj->getSprite();
	objects[obj->getX() + obj->getY() * width] = obj;
}