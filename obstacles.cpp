#include "obstacles.hpp"
#include <windows.h>
#include <random>

extern HWND         hwnd;       // Глобальный дескриптор окна
extern HDC          bufferDC;   // Глобальный HDC для двойной буферизации
extern std::mt19937 generator;  // Генератор случайных чисел

Rock::Rock(int initX, int initY) : AbstractObstacle(initX, initY) {}

void Rock::show() {
    visible         = true;
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HBRUSH brush    = CreateSolidBrush(RGB(120, 120, 120));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Неровный камень
    POINT rockPoints[6] = {
        {x - 15, y + 10},
        {x - 10, y - 15},
        {x + 5, y - 18},
        {x + 18, y - 5},
        {x + 12, y + 12},
        {x - 8, y + 15}};
    Polygon(bufferDC, rockPoints, 6);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Rock::hide() {
    visible         = false;
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 25, y - 25, x + 25, y + 25);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

void Rock::updateHitbox() {
    hitbox.left   = x - 20;
    hitbox.right  = x + 20;
    hitbox.top    = y - 20;
    hitbox.bottom = y + 20;
}

int Rock::getDamage() {
    // Камни наносят умеренный урон
    std::uniform_int_distribution<> dist(8, 20);
    return dist(generator);
}

Pothole::Pothole(int initX, int initY) : AbstractObstacle(initX, initY) {}

void Pothole::show() {
    visible         = true;
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HBRUSH brush    = CreateSolidBrush(RGB(30, 30, 30));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Овальная яма
    Ellipse(bufferDC, x - 20, y - 15, x + 20, y + 15);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Pothole::hide() {
    visible         = false;
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 25, y - 20, x + 25, y + 20);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

void Pothole::updateHitbox() {
    hitbox.left   = x - 22;
    hitbox.right  = x + 22;
    hitbox.top    = y - 17;
    hitbox.bottom = y + 17;
}

int Pothole::getDamage() {
    // Ямы наносят серьезный урон
    std::uniform_int_distribution<> dist(25, 45);
    return dist(generator);
}