#pragma once

#include "abstract.hpp"

// Камень - препятствие
class Rock : public AbstractObstacle {
   public:
    Rock(int initX, int initY);

    virtual void show() override;          // Отображение камня
    virtual void hide() override;          // Скрытие камня
    virtual void updateHitbox() override;  // Обновление хитбокса камня
    virtual int  getDamage() override;     // Возвращает урон, наносимый камнем
};

// Яма - препятствие
class Pothole : public AbstractObstacle {
   public:
    Pothole(int initX, int initY);

    virtual void show() override;          // Отображение ямы
    virtual void hide() override;          // Скрытие ямы
    virtual void updateHitbox() override;  // Обновление хитбокса ямы
    virtual int  getDamage() override;     // Возвращает урон, наносимый ямой
};