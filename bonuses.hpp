#pragma once

#include "abstract.hpp"

// Ремкомплект - восстанавливает здоровье
class RepairKit : public AbstractBonus {
   public:
    RepairKit(int initX, int initY);

    virtual void show() override;                                      // Показывает бонус на экране
    virtual void hide() override;                                      // Скрывает бонус с экрана
    virtual void updateHitbox() override;                              // Обновляет область столкновения
    virtual void applyBonus(AbstractMotorcycle* motorcycle) override;  // Применяет бонус к мотоциклу
};

// Канистра с топливом
class FuelCanister : public AbstractBonus {
   public:
    FuelCanister(int initX, int initY);

    virtual void show() override;                                      // Показывает бонус на экране
    virtual void hide() override;                                      // Скрывает бонус с экрана
    virtual void updateHitbox() override;                              // Обновляет область столкновения
    virtual void applyBonus(AbstractMotorcycle* motorcycle) override;  // Применяет бонус к мотоциклу
};
