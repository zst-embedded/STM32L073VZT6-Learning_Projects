#include "snake.hpp"

/**
 * Snake game
 *
 * @param width         Width of the screen
 * @param height        Height of the screen
 * @param draw_func     LCD draw function callback
 * @param score_func    LCD update scrore callback
 *
 * Pass the width, height of the screen.
 *
 * `draw_func` should draw the pixels on the screen ranging
 * from coordinates of 0 to (width-1) and 0 to (height-1).
 *
 * Scaling can be done by adjusting `width` and `height`,
 * but it must be handled manually in `draw_func`.
 *
 * `score_func` should update the score text on the screen.
 */
Snake::Snake(uint16_t width, uint16_t height,
        void (*draw_func)(uint16_t x, uint16_t y, uint16_t color),
        void (*score_func)(uint16_t score),
        void (*clear_func)()) {
    mWidth = width;
    mHeight = height;

    DrawPixelLCD = draw_func;
    UpdateScoreLCD = score_func;
    ClearLCD = clear_func;

    mDirection = DIR_RIGHT;

    // Set 3 pixel length as initial
    mCoordinates[0].x = mWidth / 2 + 1;
    mCoordinates[0].y = mHeight / 2;

    mCoordinates[1].x = mWidth / 2;
    mCoordinates[1].y = mHeight / 2;

    mCoordinates[2].x = mWidth / 2 - 1;
    mCoordinates[2].y = mHeight / 2;

    // Extend to respective length
    extend();
    extend();

    // Reset score and add food
    score = 0;
    putFood();
}

Snake::~Snake() {
    // delete mCoordinates;
}


/**
 * Change direction of snake.
 */
void Snake::setDirection(Direction_TypeDef d) {
    if (-d == mDirection) {
        // If change to opposite direction
        // of current direction, don't allow
        return;
    }
    mDirection = d;
}

/**
 * Return the number of coordinates of the snake.
 * Index of the last coordinate is n-1.
 */
uint16_t Snake::getCoordinatesCount() {
    for (int i = SNAKE_MAX_LENGTH; i > 0; i--) {
        Coordinates c = mCoordinates[i-1];
        if (c.x > 0 && c.y > 0) {
            return i;
        }
    }
    return 0;
}

/**
 * Return a Coordinates struct with the next move to make.
 */
Snake::Coordinates Snake::getNextCoordinates() {
    Coordinates c = mCoordinates[0];

    if (DIR_UP == mDirection) {
        c.y--;
    } else if (DIR_DOWN == mDirection) {
        c.y++;
    } else if (DIR_RIGHT == mDirection) {
        c.x++;
    } else if (DIR_LEFT == mDirection) {
        c.x--;
    }

    return c;
}

/**
 * Check if the next move is a valid move.
 * Invalid move if touching borders of the screen,
 * or if the snale eats itself
 */
bool Snake::hasNextMove() {
    Coordinates next = getNextCoordinates();

    // Touching borders
    // if at extreme corners of screen,
    if (atBorder(next)) {
        return false;
    }

    // Eat itself
    // if next move is within body coordinates
    for (int i = 0; i < SNAKE_MAX_LENGTH; i++) {
        Coordinates c = mCoordinates[i];
        if (next.x == c.x && next.y == c.y) {
            return false;
        }
        if (c.x < 0 && c.y < 0) {
            return i;
        }
    }
    return true;
}

/**
 * Check if the passed coordinate is at the border
 * of the screen.
 */
bool Snake::atBorder(Coordinates next) {
    return next.x == 0 ||
           next.y == 0 ||
           next.x == (mWidth-1) || // mWidth is 1-indexed
           next.y == (mHeight-1); // mHeight is 1-indexed
}

/**
 * Check if the passed coordinate has touched the food.
 */
bool Snake::atFood(Coordinates c) {
    return c.x == mFood.x && c.y == mFood.y;
}

/**
 * Randomly generate new food location.
 *
 * If generated location touches snake or border,
 * then regenerate again until successful.
 */
void Snake::putFood() {
    // random generation of food
    mFood.x = rand() % mWidth;
    mFood.y = rand() % mHeight;

    // check if conflict with border
    if (atBorder(mFood)) {
        // generate again!
        return putFood();
    }

    // check if conflict with snake
    for (int i = SNAKE_MAX_LENGTH; i > 0; i--) {
        Coordinates c = mCoordinates[i-1];
        if (atFood(c)) {
            // snake conflicts with new food location
            // generate again!
            return putFood();
        }
    }

    // Draw the food
    this->DrawPixelLCD(mFood.x, mFood.y, COLOR_FOOD);
}

/**
 *
 */
void Snake::move() {
    Coordinates new_pos = getNextCoordinates();

    if (atFood(new_pos)) {
        extend();
        putFood();
        return;
    }

    uint16_t count = getCoordinatesCount();

    // Blank out last position
    Coordinates last = mCoordinates[count - 1];
    this->DrawPixelLCD(last.x, last.y, LCD_COLOR_BLACK);

    // Shift all the coordinates down by one index
    for (int i = count - 1; i > 0; i--) {
        mCoordinates[i] = mCoordinates[i-1];
    }

    // Replace first position with the new movement
    mCoordinates[0] = new_pos;
    this->DrawPixelLCD(new_pos.x, new_pos.y, COLOR_SNAKE);
}

/**
 * Extend the snake by one length, also updates the score
 */
void Snake::extend() {
    // Update score
    UpdateScoreLCD(++score);

    Coordinates new_pos = getNextCoordinates();
    uint16_t count = getCoordinatesCount();

    // Shift all the coordinates down by one index
    // (add an extra position)
    for (int i = count; i > 0; i--) {
        mCoordinates[i] = mCoordinates[i-1];
    }

    // Replace first position with the new movement
    mCoordinates[0] = new_pos;
    this->DrawPixelLCD(new_pos.x, new_pos.y, COLOR_SNAKE);
}

/**
 * Draw the entire screen again (snake, border, food)
 * The LCD should be cleared prior to calling this.
 */
void Snake::invalidate() {
    int i = 0;

    // Clear Display
    ClearLCD();

    // Draw Snake
    while (true) {
        Coordinates coord = mCoordinates[i];
        if (coord.x < 0 || coord.y < 0) {
            break;
        }
        this->DrawPixelLCD(coord.x, coord.y, COLOR_SNAKE);
        i++;
    }

    // Draw Border (horizontal)
    for (int w = 0; w < mWidth; w++) {
        this->DrawPixelLCD(w, 0, COLOR_BORDER);
        this->DrawPixelLCD(w, mHeight - 1, COLOR_BORDER);
    }

    // Draw Border (vertical)
    for (int h = 0; h < mHeight; h++) {
        this->DrawPixelLCD(0, h, COLOR_BORDER);
        this->DrawPixelLCD(mWidth - 1, h, COLOR_BORDER);
    }

    // Draw Food
    this->DrawPixelLCD(mFood.x, mFood.y, COLOR_FOOD);
}
