// Поддержка Unicode для WinAPI
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <cstdlib>  // Для rand()
#include <ctime>    // Для time()
#include <cmath>    // Для математических функций

// ============================================================================
// КОНСТАНТЫ И ТИПЫ ДАННЫХ
// ============================================================================

// Константы программы
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;
const int MAX_INTERACTION_OBJECTS = 8;
const int MIN_SPAWN_DISTANCE = 100;

// Перечисление состояний мотоцикла (для матрицы переходов)
enum MotorcycleState {
    STATE_NORMAL = 0,      // Нормальное состояние
    STATE_DAMAGED = 1,     // Поврежденное состояние
    STATE_BOOSTED = 2,     // Ускоренное состояние
    STATE_REFUELING = 3    // Заправляется
};

// Перечисление типов мотоциклов
enum MotorcycleType {
    MOTO_SPORT = 0,
    MOTO_TOURING = 1
};

// Перечисление типов объектов взаимодействия
enum InteractionObjectType {
    OBJ_OBSTACLE = 0,      // Препятствие (повреждает)
    OBJ_FUEL_STATION = 1,  // Заправка (восстанавливает)
    OBJ_SPEED_BOOST = 2,   // Ускоритель (дает буст)
    OBJ_REPAIR_KIT = 3     // Ремкомплект (чинит)
};

// Структура для хитбокса (упрощенная, без отрисовки)
struct HitBox {
    int x, y, width, height;

    HitBox(int x = 0, int y = 0, int w = 0, int h = 0) : x(x), y(y), width(w), height(h) {}

    bool intersects(const HitBox& other) const {
        return !(x + width < other.x || other.x + other.width < x ||
                y + height < other.y || other.y + other.height < y);
    }

    void updatePosition(int newX, int newY) {
        x = newX;
        y = newY;
    }
};

// ============================================================================
// МАТРИЦА ПЕРЕХОДОВ СОСТОЯНИЙ
// ============================================================================

class StateTransitionMatrix {
private:
    // Матрица переходов [текущее_состояние][тип_объекта] = новое_состояние
    MotorcycleState transitionMatrix[4][4] = {
        // NORMAL      DAMAGED     BOOSTED     REFUELING
        { STATE_NORMAL, STATE_DAMAGED, STATE_BOOSTED, STATE_REFUELING }, // из NORMAL
        { STATE_NORMAL, STATE_DAMAGED, STATE_NORMAL,  STATE_REFUELING }, // из DAMAGED
        { STATE_NORMAL, STATE_DAMAGED, STATE_BOOSTED, STATE_NORMAL    }, // из BOOSTED
        { STATE_NORMAL, STATE_DAMAGED, STATE_NORMAL,  STATE_REFUELING }  // из REFUELING
    };

public:
    // Получить новое состояние после взаимодействия
    MotorcycleState getNewState(MotorcycleState currentState, InteractionObjectType objectType) {
        return transitionMatrix[currentState][objectType];
    }

    // Проверить, разрешен ли переход
    bool isTransitionAllowed(MotorcycleState from, MotorcycleState to) {
        // Всегда разрешаем переходы в данной демонстрации
        return true;
    }
};

// ============================================================================
// БАЗОВЫЕ ИНТЕРФЕЙСЫ
// ============================================================================

// Интерфейс для отрисовки
class IDrawable {
public:
    virtual void draw(HDC hdc) = 0;
    virtual ~IDrawable() = default;
};

// Интерфейс для объектов с коллизией
class ICollidable {
public:
    virtual HitBox getHitBox() const = 0;
    virtual ~ICollidable() = default;
};

// Интерфейс для взаимодействующих объектов
class IInteractable {
public:
    virtual InteractionObjectType getInteractionType() const = 0;
    virtual void onInteraction() = 0;  // Что происходит при взаимодействии
    virtual bool shouldRespawn() const = 0;  // Нужно ли создать новый объект после взаимодействия
    virtual ~IInteractable() = default;
};

// ============================================================================
// ОБЪЕКТЫ ВЗАИМОДЕЙСТВИЯ (демонстрация полиморфизма)
// ============================================================================

// Базовый класс для объектов взаимодействия
class InteractionObject : public IDrawable, public ICollidable, public IInteractable {
protected:
    int x, y;                           // Позиция
    int width, height;                  // Размеры
    InteractionObjectType objectType;   // Тип объекта
    bool active;                        // Активен ли объект

public:
    InteractionObject(int posX, int posY, int w, int h, InteractionObjectType type)
        : x(posX), y(posY), width(w), height(h), objectType(type), active(true) {}

    virtual ~InteractionObject() = default;

    // Реализация интерфейса ICollidable
    virtual HitBox getHitBox() const override {
        return HitBox(x, y, width, height);
    }

    // Реализация интерфейса IInteractable
    virtual InteractionObjectType getInteractionType() const override {
        return objectType;
    }

    virtual void onInteraction() override {
        active = false;  // Объект становится неактивным после взаимодействия
    }

    virtual bool shouldRespawn() const override {
        return !active;  // Респавн, если объект неактивен
    }

    // Геттеры
    bool isActive() const { return active; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

// Препятствие (повреждает мотоцикл)
class Obstacle : public InteractionObject {
public:
    Obstacle(int posX, int posY)
        : InteractionObject(posX, posY, 40, 40, OBJ_OBSTACLE) {}

    // Отрисовка препятствия (серый квадрат с крестом)
    virtual void draw(HDC hdc) override {
        if (!active) return;

        HBRUSH obstacleBrush = CreateSolidBrush(RGB(100, 100, 100));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, obstacleBrush);

        // Основной блок препятствия
        Rectangle(hdc, x, y, x + width, y + height);

        // Крест для обозначения опасности
        HPEN dangerPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, dangerPen);

        MoveToEx(hdc, x + 5, y + 5, NULL);
        LineTo(hdc, x + width - 5, y + height - 5);
        MoveToEx(hdc, x + width - 5, y + 5, NULL);
        LineTo(hdc, x + 5, y + height - 5);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(dangerPen);
        DeleteObject(obstacleBrush);
    }
};

// Заправочная станция (восстанавливает состояние)
class FuelStation : public InteractionObject {
public:
    FuelStation(int posX, int posY)
        : InteractionObject(posX, posY, 50, 50, OBJ_FUEL_STATION) {}

    // Отрисовка заправки (зеленый квадрат с символом топлива)
    virtual void draw(HDC hdc) override {
        if (!active) return;

        HBRUSH fuelBrush = CreateSolidBrush(RGB(0, 200, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, fuelBrush);

        // Основная станция
        Rectangle(hdc, x, y, x + width, y + height);

        // Символ топливного бака
        HBRUSH tankBrush = CreateSolidBrush(RGB(0, 100, 0));
        SelectObject(hdc, tankBrush);
        Ellipse(hdc, x + 10, y + 10, x + width - 10, y + height - 10);

        // Заправочный шланг
        HPEN hosePen = CreatePen(PS_SOLID, 2, RGB(0, 50, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, hosePen);

        MoveToEx(hdc, x + width/2, y + height/2, NULL);
        LineTo(hdc, x + width, y + height/2);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hosePen);
        DeleteObject(tankBrush);
        DeleteObject(fuelBrush);
    }
};

// Ускоритель (дает временный буст)
class SpeedBoost : public InteractionObject {
public:
    SpeedBoost(int posX, int posY)
        : InteractionObject(posX, posY, 45, 45, OBJ_SPEED_BOOST) {}

    // Отрисовка ускорителя (синий ромб со стрелками)
    virtual void draw(HDC hdc) override {
        if (!active) return;

        HBRUSH boostBrush = CreateSolidBrush(RGB(0, 100, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, boostBrush);

        // Ромбовидная форма ускорителя
        POINT diamond[] = {
            {x + width/2, y},
            {x + width, y + height/2},
            {x + width/2, y + height},
            {x, y + height/2}
        };
        Polygon(hdc, diamond, 4);

        // Стрелки ускорения
        HPEN arrowPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, arrowPen);

        // Первая стрелка
        MoveToEx(hdc, x + 10, y + height/2, NULL);
        LineTo(hdc, x + 20, y + height/2);
        LineTo(hdc, x + 17, y + height/2 - 3);
        MoveToEx(hdc, x + 20, y + height/2, NULL);
        LineTo(hdc, x + 17, y + height/2 + 3);

        // Вторая стрелка
        MoveToEx(hdc, x + 25, y + height/2, NULL);
        LineTo(hdc, x + 35, y + height/2);
        LineTo(hdc, x + 32, y + height/2 - 3);
        MoveToEx(hdc, x + 35, y + height/2, NULL);
        LineTo(hdc, x + 32, y + height/2 + 3);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(arrowPen);
        DeleteObject(boostBrush);
    }
};

// Ремонтный комплект (чинит повреждения)
class RepairKit : public InteractionObject {
public:
    RepairKit(int posX, int posY)
        : InteractionObject(posX, posY, 35, 35, OBJ_REPAIR_KIT) {}

    // Отрисовка ремкомплекта (желтый квадрат с гаечным ключом)
    virtual void draw(HDC hdc) override {
        if (!active) return;

        HBRUSH repairBrush = CreateSolidBrush(RGB(255, 255, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, repairBrush);

        // Основной ящик с инструментами
        Rectangle(hdc, x, y, x + width, y + height);

        // Символ гаечного ключа
        HPEN wrenchPen = CreatePen(PS_SOLID, 3, RGB(150, 150, 0));
        HPEN oldPen = (HPEN)SelectObject(hdc, wrenchPen);

        // Ручка ключа
        MoveToEx(hdc, x + 8, y + 8, NULL);
        LineTo(hdc, x + width - 8, y + height - 8);

        // Головка ключа
        Rectangle(hdc, x + width - 15, y + height - 15, x + width - 8, y + height - 8);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(wrenchPen);
        DeleteObject(repairBrush);
    }
};

// ============================================================================
// МОТОЦИКЛ (главный объект демонстрации)
// ============================================================================

class Motorcycle : public IDrawable, public ICollidable {
protected:
    float preciseX, preciseY;           // Точная позиция (дробные координаты для плавности)
    int x, y;                           // Целочисленная позиция для отрисовки
    int width, height;                  // Размеры
    MotorcycleType type;                // Тип мотоцикла
    MotorcycleState currentState;       // Текущее состояние
    StateTransitionMatrix* stateMatrix; // Матрица переходов состояний
    float moveSpeed;                    // Скорость движения (0.5 - 5.0 пикселей за кадр)

public:
    Motorcycle(int startX, int startY, MotorcycleType motoType)
        : preciseX((float)startX), preciseY((float)startY), x(startX), y(startY),
          width(80), height(40), type(motoType), currentState(STATE_NORMAL), moveSpeed(1.5f) {

        stateMatrix = new StateTransitionMatrix();
    }

    virtual ~Motorcycle() {
        delete stateMatrix;
    }

    // Реализация интерфейса ICollidable
    virtual HitBox getHitBox() const override {
        return HitBox(x, y, width, height);
    }

    // Чисто виртуальный метод - каждый тип мотоцикла рисует себя по-своему
    virtual void draw(HDC hdc) override = 0;

    // Обработка взаимодействия с объектом (демонстрация полиморфизма)
    void interact(InteractionObject* object) {
        if (!object || !object->isActive()) return;

        // Получаем новое состояние через матрицу переходов
        MotorcycleState newState = stateMatrix->getNewState(currentState, object->getInteractionType());

        // Применяем изменение состояния, если переход разрешен
        if (stateMatrix->isTransitionAllowed(currentState, newState)) {
            currentState = newState;
        }

        // Уведомляем объект о взаимодействии
        object->onInteraction();
    }

    // Движение с плавной интерполяцией и поддержкой диагоналей
    void move(bool up, bool down, bool left, bool right) {
        float deltaX = 0.0f, deltaY = 0.0f;

        // Вычисляем направление движения
        if (left) deltaX -= moveSpeed;
        if (right) deltaX += moveSpeed;
        if (up) deltaY -= moveSpeed;
        if (down) deltaY += moveSpeed;

        // Диагональное движение (нормализация для сохранения скорости)
        if ((left || right) && (up || down)) {
            // При диагональном движении применяем коэффициент √2/2 ≈ 0.707
            // чтобы общая скорость оставалась постоянной
            deltaX *= 0.707f;
            deltaY *= 0.707f;
        }

        // Обновляем точные координаты (дробные)
        float newPreciseX = preciseX + deltaX;
        float newPreciseY = preciseY + deltaY;

        // Проверяем границы экрана с учетом размеров мотоцикла
        if (newPreciseX >= 0.0f && newPreciseX <= (float)(WINDOW_WIDTH - width)) {
            preciseX = newPreciseX;
        }
        if (newPreciseY >= 0.0f && newPreciseY <= (float)(WINDOW_HEIGHT - height)) {
            preciseY = newPreciseY;
        }

        // Преобразуем точные координаты в целочисленные для отрисовки
        x = (int)(preciseX + 0.5f);  // Округление до ближайшего целого
        y = (int)(preciseY + 0.5f);
    }

    // Управление скоростью с плавной регулировкой
    void increaseSpeed() {
        if (moveSpeed < 5.0f) {
            moveSpeed += 0.3f;  // Плавное увеличение скорости
            if (moveSpeed > 5.0f) moveSpeed = 5.0f;  // Ограничиваем максимум
        }
    }

    void decreaseSpeed() {
        if (moveSpeed > 0.5f) {
            moveSpeed -= 0.3f;  // Плавное уменьшение скорости
            if (moveSpeed < 0.5f) moveSpeed = 0.5f;  // Ограничиваем минимум
        }
    }

    // Геттеры (обновленные для работы с дробными координатами)
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    MotorcycleState getState() const { return currentState; }
    float getMoveSpeed() const { return moveSpeed; }  // Возвращаем дробную скорость
    MotorcycleType getType() const { return type; }
};

// Спортивный мотоцикл (наследник с уникальной отрисовкой)
class SportMotorcycle : public Motorcycle {
public:
    SportMotorcycle(int startX, int startY)
        : Motorcycle(startX, startY, MOTO_SPORT) {}

    // Уникальная отрисовка спортивного мотоцикла с учетом состояния
    virtual void draw(HDC hdc) override {
        // Выбираем цвет в зависимости от состояния
        COLORREF bodyColor;
        switch (currentState) {
            case STATE_NORMAL:    bodyColor = RGB(255, 50, 50); break;   // Красный
            case STATE_DAMAGED:   bodyColor = RGB(150, 30, 30); break;   // Темно-красный
            case STATE_BOOSTED:   bodyColor = RGB(255, 100, 255); break; // Розовый (энергия)
            case STATE_REFUELING: bodyColor = RGB(255, 150, 50); break;  // Оранжевый
        }

        HBRUSH bodyBrush = CreateSolidBrush(bodyColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, bodyBrush);

        // Аэродинамический корпус
        Rectangle(hdc, x + 15, y + 5, x + 65, y + 25);

        // Спортивный обтекатель
        POINT fairingPoints[] = {
            {x + 10, y + 12},
            {x + 25, y + 8},
            {x + 25, y + 22},
            {x + 10, y + 18}
        };
        Polygon(hdc, fairingPoints, 4);

        // Колеса
        HBRUSH wheelBrush = CreateSolidBrush(RGB(30, 30, 30));
        SelectObject(hdc, wheelBrush);
        Ellipse(hdc, x - 2, y + 18, x + 22, y + 42);     // Переднее
        Ellipse(hdc, x + 58, y + 18, x + 82, y + 42);    // Заднее

        // Спортивные диски
        HBRUSH rimBrush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, rimBrush);
        Ellipse(hdc, x + 3, y + 23, x + 17, y + 37);
        Ellipse(hdc, x + 63, y + 23, x + 77, y + 37);

        // Выхлопная труба
        HBRUSH exhaustBrush = CreateSolidBrush(RGB(100, 100, 100));
        SelectObject(hdc, exhaustBrush);
        Rectangle(hdc, x + 65, y + 25, x + 75, y + 30);

        // Эффекты состояния
        if (currentState == STATE_BOOSTED) {
            // Эффект ускорения (пламя из выхлопной трубы)
            HBRUSH flameBrush = CreateSolidBrush(RGB(255, 200, 0));
            SelectObject(hdc, flameBrush);
            Rectangle(hdc, x + 75, y + 22, x + 85, y + 28);
            DeleteObject(flameBrush);
        }

        SelectObject(hdc, oldBrush);
        DeleteObject(bodyBrush);
        DeleteObject(wheelBrush);
        DeleteObject(rimBrush);
        DeleteObject(exhaustBrush);
    }
};

// Туристический мотоцикл (наследник с уникальной отрисовкой)
class TouringMotorcycle : public Motorcycle {
public:
    TouringMotorcycle(int startX, int startY)
        : Motorcycle(startX, startY, MOTO_TOURING) {}

    // Уникальная отрисовка туристического мотоцикла с учетом состояния
    virtual void draw(HDC hdc) override {
        // Выбираем цвет в зависимости от состояния
        COLORREF bodyColor;
        switch (currentState) {
            case STATE_NORMAL:    bodyColor = RGB(50, 50, 255); break;   // Синий
            case STATE_DAMAGED:   bodyColor = RGB(30, 30, 150); break;   // Темно-синий
            case STATE_BOOSTED:   bodyColor = RGB(100, 255, 255); break; // Голубой (энергия)
            case STATE_REFUELING: bodyColor = RGB(50, 150, 255); break;  // Светло-синий
        }

        HBRUSH bodyBrush = CreateSolidBrush(bodyColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, bodyBrush);

        // Большой комфортный корпус
        Rectangle(hdc, x + 15, y + 8, x + 75, y + 28);

        // Ветровое стекло
        HBRUSH windshieldBrush = CreateSolidBrush(RGB(200, 220, 255));
        SelectObject(hdc, windshieldBrush);
        Rectangle(hdc, x + 8, y + 2, x + 25, y + 15);

        // Багажные кофры
        HBRUSH luggageBrush = CreateSolidBrush(RGB(100, 100, 200));
        SelectObject(hdc, luggageBrush);
        Rectangle(hdc, x + 5, y + 15, x + 20, y + 28);   // Левый
        Rectangle(hdc, x + 70, y + 15, x + 85, y + 28);  // Правый

        // Комфортное сиденье
        HBRUSH seatBrush = CreateSolidBrush(RGB(139, 69, 19));
        SelectObject(hdc, seatBrush);
        Rectangle(hdc, x + 35, y + 5, x + 55, y + 12);

        // Колеса
        HBRUSH wheelBrush = CreateSolidBrush(RGB(50, 50, 50));
        SelectObject(hdc, wheelBrush);
        Ellipse(hdc, x, y + 20, x + 20, y + 40);
        Ellipse(hdc, x + 65, y + 20, x + 85, y + 40);

        SelectObject(hdc, oldBrush);
        DeleteObject(bodyBrush);
        DeleteObject(windshieldBrush);
        DeleteObject(luggageBrush);
        DeleteObject(seatBrush);
        DeleteObject(wheelBrush);
    }
};

// ============================================================================
// ГЛАВНЫЙ КЛАСС СИМУЛЯЦИИ
// ============================================================================

class MotorcycleSimulation {
private:
    Motorcycle* mainMotorcycle;                              // Главный мотоцикл
    InteractionObject* objects[MAX_INTERACTION_OBJECTS];     // Массив объектов взаимодействия
    int objectCount;                                         // Количество активных объектов
    HWND hwnd;                                              // Дескриптор окна
    HDC backBufferDC;                                       // Контекст для двойной буферизации
    HBITMAP backBufferBitmap;                               // Битмап для буферизации
    HBITMAP oldBitmap;                                      // Старый битмап

    // Состояния клавиш для диагонального движения
    bool keyUp, keyDown, keyLeft, keyRight;

public:
    MotorcycleSimulation(HWND windowHandle)
        : hwnd(windowHandle), objectCount(0),
          keyUp(false), keyDown(false), keyLeft(false), keyRight(false) {

        // Инициализируем генератор случайных чисел
        srand((unsigned int)time(nullptr));

        // Создаем главный мотоцикл (случайный тип)
        int motorcycleType = rand() % 2;
        if (motorcycleType == 0) {
            mainMotorcycle = new SportMotorcycle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        } else {
            mainMotorcycle = new TouringMotorcycle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }

        // Инициализируем массив объектов
        for (int i = 0; i < MAX_INTERACTION_OBJECTS; i++) {
            objects[i] = nullptr;
        }

        // Создаем начальные объекты взаимодействия
        spawnInitialObjects();

        // Инициализируем двойную буферизацию
        HDC mainDC = GetDC(hwnd);
        backBufferDC = CreateCompatibleDC(mainDC);
        backBufferBitmap = CreateCompatibleBitmap(mainDC, WINDOW_WIDTH, WINDOW_HEIGHT);
        oldBitmap = (HBITMAP)SelectObject(backBufferDC, backBufferBitmap);
        ReleaseDC(hwnd, mainDC);
    }

    ~MotorcycleSimulation() {
        delete mainMotorcycle;

        for (int i = 0; i < MAX_INTERACTION_OBJECTS; i++) {
            delete objects[i];
        }

        SelectObject(backBufferDC, oldBitmap);
        DeleteObject(backBufferBitmap);
        DeleteDC(backBufferDC);
    }

    // Создание начальных объектов в случайных позициях
    void spawnInitialObjects() {
        for (int i = 0; i < MAX_INTERACTION_OBJECTS; i++) {
            spawnRandomObject(i);
        }
    }

    // Создание случайного объекта в свободной позиции
    void spawnRandomObject(int index) {
        if (index >= MAX_INTERACTION_OBJECTS) return;

        // Удаляем старый объект, если он был
        delete objects[index];
        objects[index] = nullptr;

        int attempts = 0;
        const int maxAttempts = 50;

        while (attempts < maxAttempts) {
            // Генерируем случайную позицию
            int randX = rand() % (WINDOW_WIDTH - 60);
            int randY = rand() % (WINDOW_HEIGHT - 60);

            // Проверяем, что позиция не пересекается с мотоциклом
            HitBox testBox(randX, randY, 50, 50);
            if (!testBox.intersects(mainMotorcycle->getHitBox())) {

                // Проверяем, что позиция не пересекается с другими объектами
                bool positionFree = true;
                for (int j = 0; j < MAX_INTERACTION_OBJECTS; j++) {
                    if (j != index && objects[j] && objects[j]->isActive()) {
                        if (testBox.intersects(objects[j]->getHitBox())) {
                            positionFree = false;
                            break;
                        }
                    }
                }

                if (positionFree) {
                    // Создаем случайный тип объекта
                    int objectType = rand() % 4;

                    switch (objectType) {
                        case 0: objects[index] = new Obstacle(randX, randY); break;
                        case 1: objects[index] = new FuelStation(randX, randY); break;
                        case 2: objects[index] = new SpeedBoost(randX, randY); break;
                        case 3: objects[index] = new RepairKit(randX, randY); break;
                    }

                    return;  // Объект успешно создан
                }
            }

            attempts++;
        }

        // Если не удалось найти свободное место, оставляем объект пустым
        objects[index] = nullptr;
    }

    // Проверка коллизий и обработка взаимодействий
    void checkInteractions() {
        HitBox motorcycleBox = mainMotorcycle->getHitBox();

        for (int i = 0; i < MAX_INTERACTION_OBJECTS; i++) {
            if (objects[i] && objects[i]->isActive()) {
                if (motorcycleBox.intersects(objects[i]->getHitBox())) {
                    // Происходит взаимодействие (демонстрация полиморфизма)
                    mainMotorcycle->interact(objects[i]);

                    // Если объект требует респавна, создаем новый
                    if (objects[i]->shouldRespawn()) {
                        spawnRandomObject(i);
                    }
                }
            }
        }
    }

    // Отрисовка всей сцены
    void render() {
        // Очищаем буфер
        RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        FillRect(backBufferDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        // Рисуем все объекты взаимодействия (полиморфная отрисовка)
        for (int i = 0; i < MAX_INTERACTION_OBJECTS; i++) {
            if (objects[i] && objects[i]->isActive()) {
                objects[i]->draw(backBufferDC);
            }
        }

        // Рисуем главный мотоцикл (полиморфная отрисовка)
        mainMotorcycle->draw(backBufferDC);

        // Копируем готовый кадр на экран
        HDC mainDC = GetDC(hwnd);
        BitBlt(mainDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
               backBufferDC, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, mainDC);
    }

    // Обновление логики (движение, взаимодействия)
    void update() {
        // Обрабатываем движение мотоцикла
        mainMotorcycle->move(keyUp, keyDown, keyLeft, keyRight);

        // Проверяем взаимодействия
        checkInteractions();
    }

    // Обработка нажатия клавиш
    void handleKeyDown(WPARAM key) {
        switch (key) {
            case VK_UP:
            case 'W':
                keyUp = true;
                break;
            case VK_DOWN:
            case 'S':
                keyDown = true;
                break;
            case VK_LEFT:
            case 'A':
                keyLeft = true;
                break;
            case VK_RIGHT:
            case 'D':
                keyRight = true;
                break;
            case VK_ADD:      // + на Numpad
            case VK_OEM_PLUS: // + на основной клавиатуре
                mainMotorcycle->increaseSpeed();
                break;
            case VK_SUBTRACT:  // - на Numpad
            case VK_OEM_MINUS: // - на основной клавиатуре
                mainMotorcycle->decreaseSpeed();
                break;
        }
    }

    // Обработка отпускания клавиш
    void handleKeyUp(WPARAM key) {
        switch (key) {
            case VK_UP:
            case 'W':
                keyUp = false;
                break;
            case VK_DOWN:
            case 'S':
                keyDown = false;
                break;
            case VK_LEFT:
            case 'A':
                keyLeft = false;
                break;
            case VK_RIGHT:
            case 'D':
                keyRight = false;
                break;
        }
    }
};

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ И ОБРАБОТКА ОКНА
// ============================================================================

MotorcycleSimulation* g_simulation = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            g_simulation = new MotorcycleSimulation(hwnd);
            SetTimer(hwnd, 1, 50, NULL);  // 20 FPS
            return 0;

        case WM_TIMER:
            if (g_simulation) {
                g_simulation->update();
                g_simulation->render();
            }
            return 0;

        case WM_KEYDOWN:
            if (g_simulation) {
                g_simulation->handleKeyDown(wParam);
            }
            return 0;

        case WM_KEYUP:
            if (g_simulation) {
                g_simulation->handleKeyUp(wParam);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (g_simulation) {
                g_simulation->render();
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            KillTimer(hwnd, 1);
            delete g_simulation;
            g_simulation = nullptr;
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MotorcycleOOPDemo";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wc)) return -1;

    HWND hwnd = CreateWindowEx(
        0,
        L"MotorcycleOOPDemo",
        L"Демонстрация ООП: Мотоцикл и объекты взаимодействия",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH + 16, WINDOW_HEIGHT + 39,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return -1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}