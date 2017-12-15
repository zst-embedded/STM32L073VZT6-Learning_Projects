#pragma once

#include "stm32l073z_eval_lcd.h"
#include <stdbool.h>

class Snake {
public:

    const static uint16_t COLOR_TEXT = LCD_COLOR_WHITE;
    const static uint16_t COLOR_BACKGROUND = LCD_COLOR_BLACK;
    const static uint16_t COLOR_BORDER = LCD_COLOR_RED;
    const static uint16_t COLOR_SNAKE = LCD_COLOR_GREEN;
    const static uint16_t COLOR_FOOD = LCD_COLOR_WHITE;
    const static uint16_t SNAKE_MAX_LENGTH = 200;

    uint16_t score;

    typedef enum {
        DIR_UP = 1,
        DIR_DOWN = -1,
        DIR_LEFT = 2,
        DIR_RIGHT = -2,
    } Direction_TypeDef;

    typedef struct {
        int16_t x = -1;
        int16_t y = -1;
    } Coordinates;


    Snake(uint16_t width, uint16_t height,
            void (*draw_func)(uint16_t x, uint16_t y, uint16_t col),
            void (*score_func)(uint16_t score),
            void (*clear_func)());

    ~Snake();

    bool hasNextMove();
    void move();
    void extend();
    void invalidate();
    bool atFood(Coordinates);
    bool atBorder(Coordinates);
    void putFood();
    void setDirection(Direction_TypeDef);
    uint16_t getCoordinatesCount();
    Coordinates getNextCoordinates();

private:

    uint16_t mWidth;
    uint16_t mHeight;
    Direction_TypeDef mDirection;

    Coordinates mFood;
    Coordinates mCoordinates[SNAKE_MAX_LENGTH];

    void (*DrawPixelLCD)(uint16_t x, uint16_t y, uint16_t col) = NULL;
    void (*UpdateScoreLCD)(uint16_t score) = NULL;
    void (*ClearLCD)() = NULL;
};
