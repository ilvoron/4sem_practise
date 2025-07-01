#include "base.hpp"

Location::Location(int initX, int initY) : x(initX), y(initY) {}

int  Location::getX() const { return x; }
int  Location::getY() const { return y; }
void Location::setX(int newX) { x = newX; }
void Location::setY(int newY) { y = newY; }
void Location::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}