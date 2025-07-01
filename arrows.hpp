#pragma once

#include "abstract.hpp"

// Стрелка по часовой стрелке
class ClockwiseArrow : public AbstractArrow {
   public:
    ClockwiseArrow(int initX, int initY);

    virtual void show() override;         // Отобразить стрелку
    virtual void hide() override;         // Скрыть стрелку
    virtual bool isClockwise() override;  // Проверить направление стрелки (по часовой стрелке)
};

// Стрелка против часовой стрелки
class CounterClockwiseArrow : public AbstractArrow {
   public:
    CounterClockwiseArrow(int initX, int initY);

    virtual void show() override;         // Отобразить стрелку
    virtual void hide() override;         // Скрыть стрелку
    virtual bool isClockwise() override;  // Проверить направление стрелки (против часовой стрелки)
};