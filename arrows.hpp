#pragma once

#include "abstract.hpp"

// Стрелка по часовой стрелке
class ClockwiseArrow : public AbstractArrow {
   public:
    ClockwiseArrow(int initX, int initY);

    virtual void show() override;
    virtual void hide() override;
    virtual bool isClockwise() override;
};

// Стрелка против часовой стрелки
class CounterClockwiseArrow : public AbstractArrow {
   public:
    CounterClockwiseArrow(int initX, int initY);

    virtual void show() override;
    virtual void hide() override;
    virtual bool isClockwise() override;
};