#pragma once

#include "base.hpp"
#include "interfaces.hpp"

class AbstractMotorcycle : public Location, public IDisplayable, public IMovable {
   public:
    AbstractMotorcycle(int initX, int initY);

    // Общие методы для всех мотоциклов
    int    getHealth() const;                 // Возвращает здоровье мотоцикла (0-100%)
    int    getFuel() const;                   // Возвращает топливо мотоцикла (0-100%)
    bool   isVisible() const;                 // Возвращает видимость мотоцикла
    bool   canMove() const;                   // Проверяет, может ли мотоцикл двигаться (здоровье и топливо > 0)
    int    getSpeed() const;                  // Возвращает скорость мотоцикла (в пикселях в секунду)
    void   takeDamage(int damage);            // Наносит урон мотоциклу (уменьшает здоровье)
    void   consumeFuel(float amount = 0.1f);  // Потребляет топливо (уменьшает его количество)
    void   repairHealth(int amount = 50);     // Восстанавливает здоровье мотоцикла (увеличивает его количество)
    void   refuel(int amount = 50);           // Заправляет мотоцикл топливом (увеличивает его количество)
    Hitbox getHitbox() const;                 // Возвращает хитбокс мотоцикла для коллизий
    void   updateHitbox();                    // Обновляет хитбокс мотоцикла на основе текущих координат

    // Реализация IMovable
    virtual void moveTo(int newX, int newY) override;      // Перемещает мотоцикл в новые координаты
    virtual void moveBy(int deltaX, int deltaY) override;  // Перемещает мотоцикл относительно текущих координат

    // Общие методы отображения
    virtual void show() override;  // Отображает мотоцикл на экране
    virtual void hide() override;  // Скрывает мотоцикл с экрана

    // ЧИСТО ВИРТУАЛЬНЫЕ МЕТОДЫ - должны быть реализованы в потомках
    virtual void showBody()      = 0;  // Отрисовка корпуса мотоцикла
    virtual void showWheels()    = 0;  // Отрисовка колес
    virtual void showHandlebar() = 0;  // Отрисовка руля
    virtual void showDetails()   = 0;  // Отрисовка специфичных деталей

    virtual void hideBody()      = 0;  // Скрытие корпуса
    virtual void hideWheels()    = 0;  // Скрытие колес
    virtual void hideHandlebar() = 0;  // Скрытие руля
    virtual void hideDetails()   = 0;  // Скрытие деталей

   protected:
    int    health;   // Здоровье мотоцикла (0-100%)
    float  fuel;     // Топливо мотоцикла (0-100%, с дробными значениями)
    bool   visible;  // Флаг видимости
    Hitbox hitbox;   // Хитбокс для коллизий

    void hideComponent(int deltaLeft, int deltaTop, int deltaRight, int deltaBottom);  // Скрыть компонент с заданными смещениями

   private:
    // Отрисовка статуса мотоцикла (здоровье и топливо)
    void showStatus();  // Отображает статус мотоцикла на экране
    void hideStatus();  // Скрывает статус мотоцикла с экрана
};

// Абстрактный класс для препятствий
class AbstractObstacle : public Location, public IDisplayable, public IRespawnable {
   public:
    AbstractObstacle(int initX, int initY);

    virtual void respawn(int screenWidth, int screenHeight) override;  // Перемещает препятствие в случайное место на экране
    Hitbox       getHitbox() const;                                    // Возвращает хитбокс препятствия для коллизий
    virtual void updateHitbox() = 0;                                   // Обновляет хитбокс препятствия на основе текущих координат
    virtual int  getDamage()    = 0;                                   // Урон, который наносит препятствие

   protected:
    bool   visible;  // Флаг видимости
    Hitbox hitbox;   // Хитбокс для коллизий
};

// Абстрактный класс для бонусов
class AbstractBonus : public Location, public IDisplayable, public IRespawnable {
   public:
    AbstractBonus(int initX, int initY);

    virtual void respawn(int screenWidth, int screenHeight) override;  // Перемещает бонус в случайное место на экране
    Hitbox       getHitbox() const;                                    // Возвращает хитбокс бонуса для коллизий
    virtual void updateHitbox()                             = 0;       // Обновляет хитбокс бонуса на основе текущих координат
    virtual void applyBonus(AbstractMotorcycle* motorcycle) = 0;       // Применяет бонус к мотоциклу

   protected:
    bool   visible;
    Hitbox hitbox;
};

// Абстрактный класс для стрелок переключения
class AbstractArrow : public Location, public IDisplayable, public IRespawnable {
   public:
    AbstractArrow(int initX, int initY);

    virtual void respawn(int screenWidth, int screenHeight) override;  // Перемещает стрелку в случайное место на экране
    Hitbox       getHitbox() const;                                    // Возвращает хитбокс стрелки для коллизий
    virtual void updateHitbox();                                       // Обновляет хитбокс стрелки на основе текущих координат
    virtual bool isClockwise() = 0;                                    // Направление поворота

   protected:
    bool   visible;
    Hitbox hitbox;
};