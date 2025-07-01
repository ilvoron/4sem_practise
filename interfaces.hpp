#pragma once

// Интерфейс для объектов, которые могут отображаться на экране
class IDisplayable {
   public:
    virtual ~IDisplayable() = default;
    virtual void show()     = 0;  // Чисто виртуальный метод для отображения
    virtual void hide()     = 0;  // Чисто виртуальный метод для скрытия
};

// Интерфейс для движущихся объектов
class IMovable {
   public:
    virtual ~IMovable()                         = default;
    virtual void moveTo(int newX, int newY)     = 0;  // Перемещение в точку
    virtual void moveBy(int deltaX, int deltaY) = 0;  // Относительное перемещение
};

// Интерфейс для объектов, которые могут респавниться
class IRespawnable {
   public:
    virtual ~IRespawnable()                                 = default;
    virtual void respawn(int screenWidth, int screenHeight) = 0;  // Респавн в случайном месте
};