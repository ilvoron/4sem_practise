#include "arrows.hpp"
#include <windows.h>

extern HWND hwnd;      // Глобальный дескриптор окна
extern HDC  bufferDC;  // Глобальный HDC для двойной буферизации

ClockwiseArrow::ClockwiseArrow(int initX, int initY) : AbstractArrow(initX, initY) {}

void ClockwiseArrow::show() {
    visible     = true;
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(0, 200, 0));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    // Simple horizontal arrow pointing right
    MoveToEx(bufferDC, x - 15, y, NULL);
    LineTo(bufferDC, x + 15, y);

    // Arrow head pointing right
    MoveToEx(bufferDC, x + 15, y, NULL);
    LineTo(bufferDC, x + 10, y - 5);
    MoveToEx(bufferDC, x + 15, y, NULL);
    LineTo(bufferDC, x + 10, y + 5);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void ClockwiseArrow::hide() {
    visible         = false;
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 25, y - 25, x + 25, y + 25);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

bool ClockwiseArrow::isClockwise() { return true; }

CounterClockwiseArrow::CounterClockwiseArrow(int initX, int initY) : AbstractArrow(initX, initY) {
    updateHitbox();  // Инициализируем хитбокс после полной конструкции объекта
}

void CounterClockwiseArrow::show() {
    visible     = true;
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(200, 0, 200));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    // Simple horizontal arrow pointing left
    MoveToEx(bufferDC, x + 15, y, NULL);
    LineTo(bufferDC, x - 15, y);

    // Arrow head pointing left
    MoveToEx(bufferDC, x - 15, y, NULL);
    LineTo(bufferDC, x - 10, y - 5);
    MoveToEx(bufferDC, x - 15, y, NULL);
    LineTo(bufferDC, x - 10, y + 5);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void CounterClockwiseArrow::hide() {
    visible         = false;
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 25, y - 25, x + 25, y + 25);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

bool CounterClockwiseArrow::isClockwise() { return false; }