#include "bonuses.hpp"
#include <windows.h>

extern HWND hwnd;      // Глобальный дескриптор окна
extern HDC  bufferDC;  // Глобальный HDC для двойной буферизации

RepairKit::RepairKit(int initX, int initY) : AbstractBonus(initX, initY) {}

void RepairKit::show() {
    visible = true;
    // Красный крест
    HPEN   pen      = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    // Белый фон
    Rectangle(bufferDC, x - 15, y - 15, x + 15, y + 15);

    // Красный крест
    SelectObject(bufferDC, pen);
    MoveToEx(bufferDC, x, y - 10, NULL);
    LineTo(bufferDC, x, y + 10);
    MoveToEx(bufferDC, x - 10, y, NULL);
    LineTo(bufferDC, x + 10, y);

    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void RepairKit::hide() {
    visible         = false;
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 20, y - 20, x + 20, y + 20);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

void RepairKit::updateHitbox() {
    hitbox.left   = x - 17;
    hitbox.right  = x + 17;
    hitbox.top    = y - 17;
    hitbox.bottom = y + 17;
}

void RepairKit::applyBonus(AbstractMotorcycle* motorcycle) {
    motorcycle->repairHealth(50);
}

FuelCanister::FuelCanister(int initX, int initY) : AbstractBonus(initX, initY) {
    updateHitbox();  // Инициализируем хитбокс после полной конструкции объекта
}

void FuelCanister::show() {
    visible = true;

    // Основное тело канистры (красное) - классическая прямоугольная форма
    HPEN   pen      = CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
    HBRUSH brush    = CreateSolidBrush(RGB(255, 50, 50));
    HPEN   oldPen   = (HPEN)SelectObject(bufferDC, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);

    Rectangle(bufferDC, x - 15, y - 20, x + 15, y + 20);

    // Характерный крестообразный рельеф на передней стенке канистры
    HPEN reliefPen = CreatePen(PS_SOLID, 1, RGB(180, 30, 30));  // Темнее основного цвета
    SelectObject(bufferDC, reliefPen);

    // Четыре линии от углов к центру, создающие характерный X-образный рельеф
    MoveToEx(bufferDC, x - 12, y - 15, NULL);  // Левый верхний угол
    LineTo(bufferDC, x, y);                    // К центру

    MoveToEx(bufferDC, x + 12, y - 15, NULL);  // Правый верхний угол
    LineTo(bufferDC, x, y);                    // К центру

    MoveToEx(bufferDC, x - 12, y + 15, NULL);  // Левый нижний угол
    LineTo(bufferDC, x, y);                    // К центру

    MoveToEx(bufferDC, x + 12, y + 15, NULL);  // Правый нижний угол
    LineTo(bufferDC, x, y);                    // К центру

    // Ручка канистры сверху (серая)
    HPEN   handlePen   = CreatePen(PS_SOLID, 3, RGB(100, 100, 100));
    HBRUSH handleBrush = CreateSolidBrush(RGB(150, 150, 150));
    SelectObject(bufferDC, handlePen);
    SelectObject(bufferDC, handleBrush);

    // Прямоугольная ручка сверху канистры
    Rectangle(bufferDC, x - 8, y - 25, x + 8, y - 18);

    // Создаем иллюзию объемной ручки, "вырезая" внутреннюю часть
    HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));  // Цвет фона
    SelectObject(bufferDC, bgBrush);
    Rectangle(bufferDC, x - 5, y - 23, x + 5, y - 20);

    // Восстанавливаем контур внутренней части ручки
    SelectObject(bufferDC, handlePen);
    SelectObject(bufferDC, (HBRUSH)GetStockObject(NULL_BRUSH));
    Rectangle(bufferDC, x - 5, y - 23, x + 5, y - 20);

    // Горловина канистры (желтая) - характерная деталь топливных канистр
    HPEN   spoutPen   = CreatePen(PS_SOLID, 2, RGB(200, 150, 0));
    HBRUSH spoutBrush = CreateSolidBrush(RGB(255, 200, 50));
    SelectObject(bufferDC, spoutPen);
    SelectObject(bufferDC, spoutBrush);

    // Цилиндрическая горловина сверху канистры
    Ellipse(bufferDC, x + 8, y - 30, x + 18, y - 15);

    // Крышка горловины (более темная желтая)
    HBRUSH capBrush = CreateSolidBrush(RGB(220, 170, 30));
    SelectObject(bufferDC, capBrush);
    Ellipse(bufferDC, x + 9, y - 28, x + 17, y - 20);

    // Освобождаем все ресурсы
    SelectObject(bufferDC, oldPen);
    SelectObject(bufferDC, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
    DeleteObject(reliefPen);
    DeleteObject(handlePen);
    DeleteObject(handleBrush);
    DeleteObject(bgBrush);
    DeleteObject(spoutPen);
    DeleteObject(spoutBrush);
    DeleteObject(capBrush);
}

void FuelCanister::hide() {
    visible = false;
    // Очищаем всю область канистры, включая ручку и горловину
    HBRUSH brush    = CreateSolidBrush(RGB(255, 255, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(bufferDC, brush);
    Rectangle(bufferDC, x - 20, y - 35, x + 25, y + 25);  // Расширенная область для новых элементов
    SelectObject(bufferDC, oldBrush);
    DeleteObject(brush);
}

void FuelCanister::updateHitbox() {
    // Хитбокс учитывает новую форму с ручкой сверху и горловиной
    hitbox.left   = x - 17;  // Левая граница основного тела
    hitbox.right  = x + 20;  // Правая граница (включая горловину)
    hitbox.top    = y - 32;  // Верхняя граница (включая ручку)
    hitbox.bottom = y + 22;  // Нижняя граница основного тела
}

void FuelCanister::applyBonus(AbstractMotorcycle* motorcycle) {
    motorcycle->refuel(50);
}