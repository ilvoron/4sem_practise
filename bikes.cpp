#include "bikes.hpp"
#include <windows.h>
#include <cmath>

extern HWND hwnd;      // Глобальный дескриптор окна
extern HDC  bufferDC;  // Глобальный HDC для двойной буферизации

SportBike::SportBike(int initX, int initY) : AbstractMotorcycle(initX, initY) {}

void SportBike::showBody() {
    // Обтекаемый спортивный корпус (красный)
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
    HBRUSH brush    = CreateSolidBrush(RGB(255, 50, 50));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Основной корпус - обтекаемая форма
    POINT bodyPoints[6] = {
        {x - 40, y},
        {x - 20, y - 20},
        {x + 30, y - 15},
        {x + 40, y + 5},
        {x + 10, y + 20},
        {x - 30, y + 15}};
    Polygon(bufferDC, bodyPoints, 6);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void SportBike::showWheels() {
    // Тонкие спортивные колеса (черные)
    HPEN   pen      = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(50, 50, 50));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Ellipse(bufferDC, x + 25, y + 5, x + 45, y + 25);  // Переднее колесо
    Ellipse(bufferDC, x - 45, y + 5, x - 25, y + 25);  // Заднее колесо

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void SportBike::showHandlebar() {
    // Низкий спортивный руль
    HPEN pen    = CreatePen(PS_SOLID, 3, RGB(100, 100, 100));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    MoveToEx(bufferDC, x + 15, y - 15, NULL);
    LineTo(bufferDC, x + 25, y - 25);
    MoveToEx(bufferDC, x + 15, y - 25, NULL);
    LineTo(bufferDC, x + 35, y - 25);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void SportBike::showDetails() {
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(200, 200, 200));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    // Выхлопная труба
    MoveToEx(bufferDC, x - 15, y + 5, NULL);  // Начало от задней части корпуса
    LineTo(bufferDC, x - 50, y + 5);          // Горизонтальная труба назад

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void SportBike::hideBody() { hideComponent(-50, -25, 50, 25); }
void SportBike::hideWheels() { hideComponent(-50, -10, 50, 20); }
void SportBike::hideHandlebar() { hideComponent(10, -30, 40, -10); }
void SportBike::hideDetails() { hideComponent(-60, 5, -30, 20); }

Cruiser::Cruiser(int initX, int initY) : AbstractMotorcycle(initX, initY) {}

void Cruiser::showBody() {
    // Массивный корпус круизера (синий)
    HPEN   pen      = CreatePen(PS_SOLID, 3, RGB(50, 50, 255));
    HBRUSH brush    = CreateSolidBrush(RGB(50, 50, 255));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Rectangle(bufferDC, x - 35, y - 15, x + 35, y + 15);  // Прямоугольный корпус
    Ellipse(bufferDC, x - 10, y - 25, x + 25, y - 5);     // Топливный бак

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Cruiser::showWheels() {
    // Широкие колеса круизера
    HPEN   pen      = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(70, 70, 70));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Ellipse(bufferDC, x + 20, y - 10, x + 50, y + 20);  // Переднее колесо
    Ellipse(bufferDC, x - 50, y - 10, x - 20, y + 20);  // Заднее колесо

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Cruiser::showHandlebar() {
    // Высокий комфортный руль
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(120, 120, 120));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    MoveToEx(bufferDC, x + 10, y - 15, NULL);
    LineTo(bufferDC, x + 15, y - 35);
    MoveToEx(bufferDC, x + 5, y - 35, NULL);
    LineTo(bufferDC, x + 25, y - 35);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void Cruiser::showDetails() {
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
    HBRUSH brush    = CreateSolidBrush(RGB(220, 220, 220));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Rectangle(bufferDC, x - 30, y - 6, x - 50, y - 12);  // Выхлопная труба
    Ellipse(bufferDC, x - 5, y - 20, x + 15, y - 10);    // Накладка на бак

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Cruiser::hideBody() { hideComponent(-40, -30, 40, 20); }
void Cruiser::hideWheels() { hideComponent(-55, -15, 55, 25); }
void Cruiser::hideHandlebar() { hideComponent(0, -40, 30, -10); }
void Cruiser::hideDetails() { hideComponent(-55, 0, -35, 30); }

Chopper::Chopper(int initX, int initY) : AbstractMotorcycle(initX, initY) {}

void Chopper::showBody() {
    // Низкая удлиненная рама чоппера (черная с оранжевыми полосами)
    HPEN   pen      = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(50, 50, 50));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Основная рама - длинная и низкая
    Rectangle(bufferDC, x - 35, y - 5, x + 15, y + 8);

    // Топливный бак каплевидной формы
    Ellipse(bufferDC, x - 15, y - 15, x + 20, y - 2);

    // Сиденье чоппера (низкое)
    HBRUSH seatBrush = CreateSolidBrush(RGB(80, 40, 20));  // Коричневая кожа
    SelectObject(bufferDC, seatBrush);
    Ellipse(bufferDC, x - 30, y - 8, x - 15, y + 3);

    // Декоративные оранжевые полосы на баке
    HPEN flamePen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
    SelectObject(bufferDC, flamePen);
    MoveToEx(bufferDC, x - 10, y - 12, NULL);
    LineTo(bufferDC, x + 15, y - 8);
    MoveToEx(bufferDC, x - 8, y - 8, NULL);
    LineTo(bufferDC, x + 17, y - 4);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(seatBrush);
    DeleteObject(flamePen);
}

void Chopper::showWheels() {
    HPEN   pen      = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(60, 60, 60));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Ellipse(bufferDC, x + 35, y - 20, x + 75, y + 20);  // Большое переднее колесо
    Ellipse(bufferDC, x - 45, y, x - 25, y + 20);       // Маленькое заднее колесо

    // Спицы на большом переднем колесе
    HPEN spokePen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    SelectObject(bufferDC, spokePen);

    int centerX = x + 55;
    int centerY = y;
    // Рисуем спицы как линии от центра к краю
    for (int i = 0; i < 8; i++) {
        double angle = i * 3.14159 / 4;  // 8 спиц
        int    endX  = centerX + (int)(15 * cos(angle));
        int    endY  = centerY + (int)(15 * sin(angle));
        MoveToEx(bufferDC, centerX, centerY, NULL);
        LineTo(bufferDC, endX, endY);
    }

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(spokePen);
}

void Chopper::showHandlebar() {
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(100, 100, 100));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    // Длинная передняя вилка
    MoveToEx(bufferDC, x + 10, y - 5, NULL);
    LineTo(bufferDC, x + 55, y - 25);  // Наклонная вилка к переднему колесу

    // Высокий руль "обезьяньи лапы"
    MoveToEx(bufferDC, x + 5, y - 8, NULL);
    LineTo(bufferDC, x + 8, y - 45);  // Очень высокий подъем

    // Горизонтальная часть руля
    MoveToEx(bufferDC, x - 5, y - 45, NULL);
    LineTo(bufferDC, x + 21, y - 45);

    // Рукоятки руля
    HBRUSH gripBrush = CreateSolidBrush(RGB(80, 40, 20));  // Коричневые рукоятки
    HBRUSH oldBrush  = (HBRUSH)SelectObject(bufferDC, gripBrush);
    Ellipse(bufferDC, x - 7, y - 47, x - 3, y - 43);
    Ellipse(bufferDC, x + 19, y - 47, x + 23, y - 43);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(gripBrush);
}

void Chopper::showDetails() {
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(150, 150, 150));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    // Выхлопная труба
    MoveToEx(bufferDC, x - 12, y + 6, NULL);
    LineTo(bufferDC, x - 20, y + 10);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void Chopper::hideBody() { hideComponent(-35, -20, 25, 15); }
void Chopper::hideWheels() { hideComponent(-45, -20, 75, 30); }
void Chopper::hideHandlebar() { hideComponent(0, -45, 55, 0); }
void Chopper::hideDetails() { hideComponent(-65, 0, -15, 30); }

Enduro::Enduro(int initX, int initY) : AbstractMotorcycle(initX, initY) {}

void Enduro::showBody() {
    // Высокий корпус эндуро (зеленый)
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(50, 150, 50));
    HBRUSH brush    = CreateSolidBrush(RGB(50, 150, 50));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Высокий угловатый корпус
    POINT bodyPoints[5] = {
        {x - 25, y + 5},
        {x - 20, y - 20},
        {x + 20, y - 15},
        {x + 25, y + 5},
        {x - 25, y + 5}};
    Polygon(bufferDC, bodyPoints, 5);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Enduro::showWheels() {
    HPEN   pen      = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(80, 80, 80));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Ellipse(bufferDC, x + 15, y - 5, x + 45, y + 25);  // Переднее колесо
    Ellipse(bufferDC, x - 45, y - 5, x - 15, y + 25);  // Заднее колесо

    // Протектор
    HPEN treadPen = CreatePen(PS_SOLID, 2, RGB(120, 120, 120));
    SelectObject(bufferDC, treadPen);

    // Горизонтальные линии протектора на переднем колесе
    MoveToEx(bufferDC, x + 18, y + 5, NULL);
    LineTo(bufferDC, x + 42, y + 5);
    MoveToEx(bufferDC, x + 18, y + 15, NULL);
    LineTo(bufferDC, x + 42, y + 15);

    // Горизонтальные линии протектора на заднем колесе
    MoveToEx(bufferDC, x - 42, y + 5, NULL);
    LineTo(bufferDC, x - 18, y + 5);
    MoveToEx(bufferDC, x - 42, y + 15, NULL);
    LineTo(bufferDC, x - 18, y + 15);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(treadPen);
}

void Enduro::showHandlebar() {
    // Прямой внедорожный руль
    HPEN pen    = CreatePen(PS_SOLID, 4, RGB(100, 100, 100));
    HPEN oldPen = (HPEN)SelectObject(bufferDC, pen);

    MoveToEx(bufferDC, x + 5, y - 15, NULL);
    LineTo(bufferDC, x + 10, y - 30);
    MoveToEx(bufferDC, x, y - 30, NULL);
    LineTo(bufferDC, x + 20, y - 30);

    SelectObject(bufferDC, oldPen);
    DeleteObject(pen);
}

void Enduro::showDetails() {
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HBRUSH brush    = CreateSolidBrush(RGB(150, 150, 150));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Rectangle(bufferDC, x - 15, y + 5, x + 15, y + 12);  // Защита двигателя
    Rectangle(bufferDC, x - 30, y - 18, x - 20, y - 8);  // Багажник сзади

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void Enduro::hideBody() { hideComponent(-30, -25, 30, 10); }
void Enduro::hideWheels() { hideComponent(-50, -10, 50, 30); }
void Enduro::hideHandlebar() { hideComponent(-5, -35, 25, -10); }
void Enduro::hideDetails() { hideComponent(-45, -30, 20, 20); }
