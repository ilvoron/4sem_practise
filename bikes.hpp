#pragma once

#include "abstract.hpp"

// Спортивный мотоцикл - быстрый и агрессивный дизайн
class SportBike : public AbstractMotorcycle {
   public:
    SportBike(int initX, int initY);

    virtual void showBody() override;       // Показать корпус
    virtual void showWheels() override;     // Показать колеса
    virtual void showHandlebar() override;  // Показать руль
    virtual void showDetails() override;    // Показать детали

    virtual void hideBody() override;       // Скрыть корпус
    virtual void hideWheels() override;     // Скрыть колеса
    virtual void hideHandlebar() override;  // Скрыть руль
    virtual void hideDetails() override;    // Скрыть детали
};

// Круизер - комфортный мотоцикл для дальних поездок
class Cruiser : public AbstractMotorcycle {
   public:
    Cruiser(int initX, int initY);

    virtual void showBody() override;       // Показать корпус
    virtual void showWheels() override;     // Показать колеса
    virtual void showHandlebar() override;  // Показать руль
    virtual void showDetails() override;    // Показать детали

    virtual void hideBody() override;       // Скрыть корпус
    virtual void hideWheels() override;     // Скрыть колеса
    virtual void hideHandlebar() override;  // Скрыть руль
    virtual void hideDetails() override;    // Скрыть детали
};

// Чоппер - кастомный мотоцикл с длинной вилкой
class Chopper : public AbstractMotorcycle {
   public:
    Chopper(int initX, int initY);

    virtual void showBody() override;       // Показать корпус
    virtual void showWheels() override;     // Показать колеса
    virtual void showHandlebar() override;  // Показать руль
    virtual void showDetails() override;    // Показать детали

    virtual void hideBody() override;       // Скрыть корпус
    virtual void hideWheels() override;     // Скрыть колеса
    virtual void hideHandlebar() override;  // Скрыть руль
    virtual void hideDetails() override;    // Скрыть детали
};

// Эндуро - внедорожный мотоцикл
class Enduro : public AbstractMotorcycle {
   public:
    Enduro(int initX, int initY);

    virtual void showBody() override;       // Показать корпус
    virtual void showWheels() override;     // Показать колеса
    virtual void showHandlebar() override;  // Показать руль
    virtual void showDetails() override;    // Показать детали

    virtual void hideBody() override;       // Скрыть корпус
    virtual void hideWheels() override;     // Скрыть колеса
    virtual void hideHandlebar() override;  // Скрыть руль
    virtual void hideDetails() override;    // Скрыть детали
};