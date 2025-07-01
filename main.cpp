/************************************************************
 *                  Курс ПРОГРАММИРОВАНИЕ                   *
 *----------------------------------------------------------*
 * Project Type  : Win64 Console Application                *
 * Project Name  : motorcycle_oop_system                    *
 * File Name     : main.cpp                                 *
 * Language      : CPP, MSVC 2022                           *
 * Programmer    : Студент                                  *
 * Created       : 01.07.2025                               *
 * Comment(s)    : Демонстрация всех 4 заданий ООП:        *
 *                 1. Наследование классов                  *
 *                 2. Наследование от интерфейса            *
 *                 3. Использование полиморфизма            *
 *                 4. Наследование от абстрактных классов   *
 ************************************************************/

#include <windows.h>
#include <random>
#include "arrows.hpp"
#include "bikes.hpp"
#include "bonuses.hpp"
#include "obstacles.hpp"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HWND hwnd;      // Глобальный дескриптор окна
HDC  bufferDC;  // Глобальный HDC для двойной буферизации

//=============================================================================
// ИГРОВАЯ ЛОГИКА
//=============================================================================

// Типы мотоциклов для матрицы переходов
enum MotorcycleType {
    SPORT_BIKE       = 0,
    CRUISER          = 1,
    CHOPPER          = 2,
    ENDURO           = 3,
    MOTORCYCLE_COUNT = 4
};

// Матрица переходов по часовой стрелке
const MotorcycleType clockwiseTransitions[MOTORCYCLE_COUNT] = {
    CRUISER,    // SportBike -> Cruiser
    CHOPPER,    // Cruiser -> Chopper
    ENDURO,     // Chopper -> Enduro
    SPORT_BIKE  // Enduro -> SportBike
};

// Матрица переходов против часовой стрелки
const MotorcycleType counterClockwiseTransitions[MOTORCYCLE_COUNT] = {
    ENDURO,      // SportBike -> Enduro
    SPORT_BIKE,  // Cruiser -> SportBike
    CRUISER,     // Chopper -> Cruiser
    CHOPPER      // Enduro -> Chopper
};

// Глобальные объекты
SportBike sportBike(400, 300);
Cruiser   cruiser(400, 300);
Chopper   chopper(400, 300);
Enduro    enduro(400, 300);

Rock    rock1(150, 150);
Rock    rock2(600, 400);
Pothole pothole1(300, 500);
Pothole pothole2(700, 200);

RepairKit    repairKit(200, 300);
FuelCanister fuelCanister(500, 150);

ClockwiseArrow        clockwiseArrow(100, 500);
CounterClockwiseArrow counterClockwiseArrow(700, 500);

// Массив указателей на мотоциклы
AbstractMotorcycle* motorcycles[MOTORCYCLE_COUNT] = {&sportBike, &cruiser, &chopper, &enduro};

// Массивы указателей на препятствия и бонусы
AbstractObstacle* obstacles[] = {&rock1, &rock2, &pothole1, &pothole2};
AbstractBonus*    bonuses[]   = {&repairKit, &fuelCanister};
AbstractArrow*    arrows[]    = {&clockwiseArrow, &counterClockwiseArrow};

// Текущий тип мотоцикла
MotorcycleType      currentMotorcycleType = SPORT_BIKE;
AbstractMotorcycle* currentMotorcycle     = motorcycles[currentMotorcycleType];

// Функция проверки коллизий между хитбоксами
bool checkCollision(const Hitbox& a, const Hitbox& b) {
    return (a.left <= b.right &&
            a.right >= b.left &&
            a.top <= b.bottom &&
            a.bottom >= b.top);
}

// Функция смены типа мотоцикла
void changeMotorcycleType(MotorcycleType newType) {
    int x      = currentMotorcycle->getX();
    int y      = currentMotorcycle->getY();
    int health = currentMotorcycle->getHealth();
    int fuel   = currentMotorcycle->getFuel();

    currentMotorcycle->hide();

    currentMotorcycleType = newType;
    currentMotorcycle     = motorcycles[currentMotorcycleType];

    // Переносим характеристики
    currentMotorcycle->setPosition(x, y);
    currentMotorcycle->repairHealth(health - currentMotorcycle->getHealth());
    currentMotorcycle->refuel(fuel - currentMotorcycle->getFuel());

    currentMotorcycle->updateHitbox();
    currentMotorcycle->show();
}

// Проверка всех коллизий
void checkAllCollisions() {
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width  = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    Hitbox motorcycleHitbox = currentMotorcycle->getHitbox();

    // Проверяем коллизии с препятствиями
    for (int i = 0; i < 4; i++) {
        if (checkCollision(motorcycleHitbox, obstacles[i]->getHitbox())) {
            int damage = obstacles[i]->getDamage();
            currentMotorcycle->takeDamage(damage);
            obstacles[i]->respawn(width, height);
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }

    // Проверяем коллизии с бонусами
    for (int i = 0; i < 2; i++) {
        if (checkCollision(motorcycleHitbox, bonuses[i]->getHitbox())) {
            bonuses[i]->applyBonus(currentMotorcycle);
            bonuses[i]->respawn(width, height);
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }

    // Проверяем коллизии со стрелками
    for (int i = 0; i < 2; i++) {
        if (checkCollision(motorcycleHitbox, arrows[i]->getHitbox())) {
            MotorcycleType newType;
            if (arrows[i]->isClockwise()) {
                newType = clockwiseTransitions[currentMotorcycleType];
            } else {
                newType = counterClockwiseTransitions[currentMotorcycleType];
            }
            changeMotorcycleType(newType);
            arrows[i]->respawn(width, height);
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }
}

// Обработка клавиатуры
void checkKeyboardInput() {
    bool needRedraw = false;

    RECT rect;
    GetClientRect(hwnd, &rect);
    int width  = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    int speed = currentMotorcycle->getSpeed();

    // Движение мотоцикла
    if (currentMotorcycle->canMove() && speed > 0) {
        if (KEY_DOWN('W') || KEY_DOWN('w') || KEY_DOWN(VK_UP)) {
            int newY = currentMotorcycle->getY() - speed;
            if (newY >= 50) {
                currentMotorcycle->moveTo(currentMotorcycle->getX(), newY);
                needRedraw = true;
            }
        }
        if (KEY_DOWN('S') || KEY_DOWN('s') || KEY_DOWN(VK_DOWN)) {
            int newY = currentMotorcycle->getY() + speed;
            if (newY <= height - 50) {
                currentMotorcycle->moveTo(currentMotorcycle->getX(), newY);
                needRedraw = true;
            }
        }
        if (KEY_DOWN('A') || KEY_DOWN('a') || KEY_DOWN(VK_LEFT)) {
            int newX = currentMotorcycle->getX() - speed;
            if (newX >= 50) {
                currentMotorcycle->moveTo(newX, currentMotorcycle->getY());
                needRedraw = true;
            }
        }
        if (KEY_DOWN('D') || KEY_DOWN('d') || KEY_DOWN(VK_RIGHT)) {
            int newX = currentMotorcycle->getX() + speed;
            if (newX <= width - 50) {
                currentMotorcycle->moveTo(newX, currentMotorcycle->getY());
                needRedraw = true;
            }
        }
    }

    // Хоткеи для восстановления
    if (KEY_DOWN('H') || KEY_DOWN('h')) {
        currentMotorcycle->repairHealth(25);
        needRedraw = true;
    }
    if (KEY_DOWN('F') || KEY_DOWN('f')) {
        currentMotorcycle->refuel(25);
        needRedraw = true;
    }

    checkAllCollisions();
    if (needRedraw) {
        InvalidateRect(hwnd, NULL, FALSE);
    }
}

// Обработчик сообщений окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;

    switch (msg) {
        case WM_PAINT: {
            HDC hdcLocal = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);
            int width  = rect.right - rect.left;
            int height = rect.bottom - rect.top;

            // Двойная буферизация
            HDC     memDC     = CreateCompatibleDC(hdcLocal);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdcLocal, width, height);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

            // Заполняем фон
            HBRUSH bgBrush  = CreateSolidBrush(RGB(240, 248, 255));
            HBRUSH oldBrush = (HBRUSH)SelectObject(memDC, bgBrush);
            FillRect(memDC, &rect, bgBrush);
            SelectObject(memDC, oldBrush);
            DeleteObject(bgBrush);

            bufferDC = memDC;

            // Отрисовываем все объекты
            currentMotorcycle->show();

            for (int i = 0; i < 4; i++) {
                obstacles[i]->show();
            }

            for (int i = 0; i < 2; i++) {
                bonuses[i]->show();
                arrows[i]->show();
            }

            // Отрисовываем справку
            SetTextColor(memDC, RGB(0, 0, 0));
            SetBkMode(memDC, TRANSPARENT);

            char helpText[500];
            sprintf_s(helpText,
                      "WASD/Arrows - movement | H - heal | F - refuel\n"
                      "Health: %d%% | Fuel: %d%% | Speed: %dpx/sec\n"
                      "Motorcycle type: %s",
                      currentMotorcycle->getHealth(),
                      currentMotorcycle->getFuel(),
                      currentMotorcycle->getSpeed(),
                      (currentMotorcycleType == SPORT_BIKE) ? "Sport Bike" : (currentMotorcycleType == CRUISER) ? "Cruiser"
                                                                         : (currentMotorcycleType == CHOPPER)   ? "Chopper"
                                                                                                                : "Enduro");

            RECT textRect = {10, 10, width - 10, 100};
            DrawText(memDC, helpText, -1, &textRect, DT_LEFT | DT_TOP | DT_WORDBREAK);

            BitBlt(hdcLocal, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);

            EndPaint(hwnd, &ps);
        } break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Регистрация класса окна
    WNDCLASSEX wc    = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "MotorcycleOOP";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window registration error!", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    // Создание окна
    hwnd = CreateWindowEx(
        0,
        "MotorcycleOOP",
        "Motorcycle OOP System - Complete OOP Demonstration",
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, hInstance, nullptr);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window creation error!", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);

    // Инициализация позиций объектов
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width  = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Начальные позиции объектов
    for (int i = 0; i < 4; i++) {
        obstacles[i]->respawn(width, height);
    }

    for (int i = 0; i < 2; i++) {
        bonuses[i]->respawn(width, height);
        arrows[i]->respawn(width, height);
    }

    // Скрываем все мотоциклы кроме текущего
    for (int i = 0; i < MOTORCYCLE_COUNT; i++) {
        motorcycles[i]->hide();
    }

    InvalidateRect(hwnd, NULL, FALSE);

    // Главный цикл
    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            checkKeyboardInput();
            Sleep(16);  // ~60 FPS (1000ms / 60fps ≈ 16.67ms)
        }
    }

    return (int)msg.wParam;
}