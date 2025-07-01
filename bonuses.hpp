#pragma once

#include "abstract.hpp"

// Ремкомплект - восстанавливает здоровье
class RepairKit : public AbstractBonus {
   public:
    RepairKit(int initX, int initY);

    virtual void show() override;
    virtual void hide() override;
    virtual void updateHitbox() override;
    virtual void applyBonus(AbstractMotorcycle* motorcycle) override;
};

// Канистра с топливом
class FuelCanister : public AbstractBonus {
   public:
    FuelCanister(int initX, int initY);

    virtual void show() override;
    virtual void hide() override;
    virtual void updateHitbox() override;
    virtual void applyBonus(AbstractMotorcycle* motorcycle) override;
};
