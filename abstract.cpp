#include "abstract.hpp"
#include <windows.h>
#include <random>

extern HWND         hwnd;       // Глобальный дескриптор окна
extern HDC          bufferDC;   // Глобальный HDC для двойной буферизации
extern std::mt19937 generator;  // Генератор случайных чисел

AbstractMotorcycle::AbstractMotorcycle(int initX, int initY) : Location(initX, initY), health(100), fuel(100), visible(true) {}

int  AbstractMotorcycle::getHealth() const { return health; }
int  AbstractMotorcycle::getFuel() const { return (int)fuel; }  // Приводим к int для отображения
bool AbstractMotorcycle::isVisible() const { return visible; }
bool AbstractMotorcycle::canMove() const { return health > 0 && fuel > 0; }  // Может ли двигаться
int  AbstractMotorcycle::getSpeed() const { return (health * 10) / 100; }    // Скорость зависит от здоровья (100% здоровья = 10px скорости)

void AbstractMotorcycle::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void AbstractMotorcycle::consumeFuel(float amount) {
    fuel -= amount;
    if (fuel < 0) fuel = 0;
}

void AbstractMotorcycle::repairHealth(int amount) {
    health += amount;
    if (health > 100) health = 100;
}

void AbstractMotorcycle::refuel(int amount) {
    fuel += amount;
    if (fuel > 100) fuel = 100;
}

Hitbox AbstractMotorcycle::getHitbox() const { return hitbox; }

void AbstractMotorcycle::updateHitbox() {
    hitbox.left   = x - 60;
    hitbox.right  = x + 60;
    hitbox.top    = y - 40;
    hitbox.bottom = y + 40;
}

// Реализация IMovable
void AbstractMotorcycle::moveTo(int newX, int newY) {
    if (!canMove()) return;
    hide();
    x = newX;
    y = newY;
    updateHitbox();
    show();
    consumeFuel();  // Тратим топливо при движении
}

void AbstractMotorcycle::moveBy(int deltaX, int deltaY) {
    moveTo(x + deltaX, y + deltaY);
}

// Общие методы отображения
void AbstractMotorcycle::show() {
    visible = true;
    showBody();       // Отрисовка корпуса
    showWheels();     // Отрисовка колес
    showHandlebar();  // Отрисовка руля
    showDetails();    // Отрисовка деталей
    showStatus();     // Отрисовка статуса (здоровье/топливо)
}

void AbstractMotorcycle::hide() {
    visible = false;
    hideBody();
    hideWheels();
    hideHandlebar();
    hideDetails();
    hideStatus();
}

void AbstractMotorcycle::hideComponent(int deltaLeft, int deltaTop, int deltaRight, int deltaBottom) {
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x + deltaLeft, y + deltaTop, x + deltaRight, y + deltaBottom);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

void AbstractMotorcycle::showStatus() {
    const int barWidth   = 100;
    const int barHeight  = 8;
    const int spacing    = 15;
    const int safeOffset = 80;  // Безопасное расстояние от мотоцикла

    // Полоска здоровья
    // Фон полоски здоровья (светло-серый)
    HBRUSH bgBrush  = CreateSolidBrush(RGB(220, 220, 220));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, bgBrush);
    Rectangle(bufferDC, x - 50, y - safeOffset, x - 50 + barWidth, y - safeOffset + barHeight);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(bgBrush);

    // Заполнение здоровья (красное)
    if (health > 0) {
        HBRUSH healthBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush           = (HBRUSH)SelectObject(bufferDC, healthBrush);
        int healthWidth    = (health * barWidth) / 100;
        Rectangle(bufferDC, x - 50, y - safeOffset, x - 50 + healthWidth, y - safeOffset + barHeight);
        SelectObject(bufferDC, oldBrush);
        DeleteObject(healthBrush);
    }

    // Рамка полоски здоровья (черная)
    HPEN   framePen  = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN   oldPen    = (HPEN)SelectObject(bufferDC, framePen);
    HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    oldBrush         = (HBRUSH)SelectObject(bufferDC, nullBrush);
    Rectangle(bufferDC, x - 50, y - safeOffset, x - 50 + barWidth, y - safeOffset + barHeight);

    // Полоска топлива
    // Фон полоски топлива (светло-серый)
    bgBrush = CreateSolidBrush(RGB(220, 220, 220));
    SelectObject(bufferDC, bgBrush);
    Rectangle(bufferDC, x - 50, y - safeOffset + spacing, x - 50 + barWidth, y - safeOffset + spacing + barHeight);
    DeleteObject(bgBrush);

    // Заполнение топлива (синее)
    if (fuel > 0) {
        HBRUSH fuelBrush = CreateSolidBrush(RGB(0, 0, 255));
        SelectObject(bufferDC, fuelBrush);
        int fuelWidth = ((int)fuel * barWidth) / 100;
        Rectangle(bufferDC, x - 50, y - safeOffset + spacing, x - 50 + fuelWidth, y - safeOffset + spacing + barHeight);
        DeleteObject(fuelBrush);
    }

    // Рамка полоски топлива (черная)
    SelectObject(bufferDC, nullBrush);
    Rectangle(bufferDC, x - 50, y - safeOffset + spacing, x - 50 + barWidth, y - safeOffset + spacing + barHeight);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(framePen);
}

void AbstractMotorcycle::hideStatus() {
    // Очищаем область статуса с учетом нового расположения
    const int safeOffset = 80;
    HBRUSH    whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH    oldBrush   = (HBRUSH)SelectObject(bufferDC, whiteBrush);
    Rectangle(bufferDC, x - 55, y - safeOffset - 5, x + 55, y - safeOffset + 25);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(whiteBrush);
}

AbstractObstacle::AbstractObstacle(int initX, int initY) : Location(initX, initY), visible(true) {}

void AbstractObstacle::respawn(int screenWidth, int screenHeight) {
    std::uniform_int_distribution<> distX(50, screenWidth - 50);
    std::uniform_int_distribution<> distY(50, screenHeight - 50);
    hide();
    x = distX(generator);
    y = distY(generator);
    updateHitbox();
    show();
}

Hitbox AbstractObstacle::getHitbox() const { return hitbox; }

AbstractBonus::AbstractBonus(int initX, int initY) : Location(initX, initY), visible(true) {}

void AbstractBonus::respawn(int screenWidth, int screenHeight) {
    std::uniform_int_distribution<> distX(50, screenWidth - 50);
    std::uniform_int_distribution<> distY(50, screenHeight - 50);
    hide();
    x = distX(generator);
    y = distY(generator);
    updateHitbox();
    show();
}

Hitbox AbstractBonus::getHitbox() const { return hitbox; }

AbstractArrow::AbstractArrow(int initX, int initY) : Location(initX, initY), visible(true) {}

void AbstractArrow::respawn(int screenWidth, int screenHeight) {
    std::uniform_int_distribution<> distX(50, screenWidth - 50);
    std::uniform_int_distribution<> distY(50, screenHeight - 50);
    hide();
    x = distX(generator);
    y = distY(generator);
    updateHitbox();
    show();
}

Hitbox AbstractArrow::getHitbox() const { return hitbox; }

void AbstractArrow::updateHitbox() {
    hitbox.left   = x - 20;
    hitbox.right  = x + 20;
    hitbox.top    = y - 20;
    hitbox.bottom = y + 20;
}