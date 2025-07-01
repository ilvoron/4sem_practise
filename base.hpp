#pragma once

// Структура хитбокса для определения коллизий
struct Hitbox {
    int left;    // Левая граница
    int right;   // Правая граница
    int top;     // Верхняя граница
    int bottom;  // Нижняя граница
};

class Location {
   public:
    Location(int initX, int initY);
    virtual ~Location() = default;

    // Общие операции для всех потомков
    int  getX() const;                     // Получить координату X
    int  getY() const;                     // Получить координату Y
    void setX(int newX);                   // Установить координату X
    void setY(int newY);                   // Установить координату Y
    void setPosition(int newX, int newY);  // Установить координаты X и Y

   protected:
    int x;  // Координата X объекта
    int y;  // Координата Y объекта
};